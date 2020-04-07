/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "osx/osxbtservicerecord_p.h"
#include "qbluetoothserviceinfo_p.h"
#include "qbluetoothserviceinfo.h"
#include "qbluetoothdeviceinfo.h"
#include "qbluetoothserver_p.h"
#include "osx/osxbtutility_p.h"
#include "osx/osxbluetooth_p.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qvariant.h>
#include <QtCore/qglobal.h>
#include <QtCore/qmutex.h>
#include <QtCore/qmap.h>
#include <QtCore/qurl.h>

#include <Foundation/Foundation.h>

QT_BEGIN_NAMESPACE

namespace {

using DarwinBluetooth::RetainPolicy;
using ServiceInfo = QBluetoothServiceInfo;

// Alas, since there is no d_ptr<->q_ptr link (which is not that bad in itself),
// I need these getters duplicated here:
ServiceInfo::Protocol socket_protocol(const QBluetoothServiceInfoPrivate &privateInfo)
{
    ServiceInfo::Sequence parameters = privateInfo.protocolDescriptor(QBluetoothUuid::Rfcomm);
    if (!parameters.isEmpty())
        return ServiceInfo::RfcommProtocol;

    parameters = privateInfo.protocolDescriptor(QBluetoothUuid::L2cap);
    if (!parameters.isEmpty())
        return ServiceInfo::L2capProtocol;

    return ServiceInfo::UnknownProtocol;
}

int channel_or_psm(const QBluetoothServiceInfoPrivate &privateInfo, QBluetoothUuid::ProtocolUuid uuid)
{
    const auto parameters = privateInfo.protocolDescriptor(uuid);
    if (parameters.isEmpty())
        return -1;
    else if (parameters.count() == 1)
        return 0;

    return parameters.at(1).toInt();
}

} // unnamed namespace

QBluetoothServiceInfoPrivate::QBluetoothServiceInfoPrivate()
{
}

QBluetoothServiceInfoPrivate::~QBluetoothServiceInfoPrivate()
{
}

bool QBluetoothServiceInfoPrivate::registerService(const QBluetoothAddress &localAddress)
{
    Q_UNUSED(localAddress);
    return false;
}

bool QBluetoothServiceInfoPrivate::registerService(const QBluetoothServiceInfo &info)
{
    using namespace OSXBluetooth;

    if (isRegistered())
        return false;

    using namespace OSXBluetooth;

    ObjCStrongReference<NSMutableDictionary> serviceDict(iobluetooth_service_dictionary(info));
    if (!serviceDict) {
        qCWarning(QT_BT_OSX) << "failed to create a service dictionary";
        return false;
    }

    Q_ASSERT(!registered);
    Q_ASSERT_X(!serviceRecord, Q_FUNC_INFO, "not registered, but serviceRecord is not nil");

    SDPRecord newRecord;
    newRecord.reset([IOBluetoothSDPServiceRecord
                     publishedServiceRecordWithDictionary:serviceDict], RetainPolicy::doInitialRetain);
    if (!newRecord) {
        qCWarning(QT_BT_OSX) << "failed to register a service record";
        return false;
    }

    BluetoothSDPServiceRecordHandle newRecordHandle = 0;
    auto *ioSDPRecord = newRecord.getAs<IOBluetoothSDPServiceRecord>();
    if ([ioSDPRecord getServiceRecordHandle:&newRecordHandle] != kIOReturnSuccess) {
        qCWarning(QT_BT_OSX) << "failed to register a service record";
        [ioSDPRecord removeServiceRecord];
        return false;
    }

    const ServiceInfo::Protocol type = info.socketProtocol();
    quint16 realPort = 0;
    QBluetoothServerPrivate *server = nullptr;
    bool configured = false;

    if (type == QBluetoothServiceInfo::L2capProtocol) {
        BluetoothL2CAPPSM psm = 0;
        server = QBluetoothServerPrivate::registeredServer(info.protocolServiceMultiplexer(), type);
        if ([ioSDPRecord getL2CAPPSM:&psm] == kIOReturnSuccess) {
            configured = true;
            realPort = psm;
        }
    } else if (type == QBluetoothServiceInfo::RfcommProtocol) {
        BluetoothRFCOMMChannelID channelID = 0;
        server = QBluetoothServerPrivate::registeredServer(info.serverChannel(), type);
        if ([ioSDPRecord getRFCOMMChannelID:&channelID] == kIOReturnSuccess) {
            configured = true;
            realPort = channelID;
        }
    }

    if (!configured) {
        [ioSDPRecord removeServiceRecord];
        qCWarning(QT_BT_OSX) << "failed to register a service record";
        return false;
    }

    registered = true;
    serviceRecord.swap(newRecord);
    serviceRecordHandle = newRecordHandle;

    if (server)
        server->startListener(realPort);

    return true;
}

bool QBluetoothServiceInfoPrivate::isRegistered() const
{
    return registered;
}

bool QBluetoothServiceInfoPrivate::unregisterService()
{
    if (!registered)
        return false;

    Q_ASSERT_X(serviceRecord, Q_FUNC_INFO, "service registered, but serviceRecord is nil");

    auto *nativeRecord = serviceRecord.getAs<IOBluetoothSDPServiceRecord>();
    [nativeRecord removeServiceRecord];
    serviceRecord.reset();

    const ServiceInfo::Protocol type = socket_protocol(*this);
    QBluetoothServerPrivate *server = nullptr;

    const QMutexLocker lock(&QBluetoothServerPrivate::channelMapMutex());
    if (type == ServiceInfo::RfcommProtocol)
        server = QBluetoothServerPrivate::registeredServer(channel_or_psm(*this, QBluetoothUuid::Rfcomm), type);
    else if (type == ServiceInfo::L2capProtocol)
        server = QBluetoothServerPrivate::registeredServer(channel_or_psm(*this, QBluetoothUuid::L2cap), type);

    if (server)
        server->stopListener();

    registered = false;
    serviceRecordHandle = 0;

    return true;
}

QT_END_NAMESPACE
