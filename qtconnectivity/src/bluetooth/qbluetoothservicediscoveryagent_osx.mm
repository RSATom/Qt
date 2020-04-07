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

#include "qbluetoothservicediscoveryagent_p.h"
#include "qbluetoothservicediscoveryagent.h"
#include "qbluetoothdevicediscoveryagent.h"
#include "qbluetoothlocaldevice.h"
#include "osx/osxbtsdpinquiry_p.h"
#include "qbluetoothhostinfo.h"
#include "osx/osxbtutility_p.h"
#include "osx/osxbluetooth_p.h"
#include "osx/uistrings_p.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qstring.h>
#include <QtCore/qglobal.h>
#include <QtCore/qdebug.h>
#include <QtCore/qlist.h>

#include <Foundation/Foundation.h>

QT_BEGIN_NAMESPACE

namespace {

using DarwinBluetooth::RetainPolicy;
using ObjCServiceInquiry = QT_MANGLE_NAMESPACE(OSXBTSDPInquiry);

}

QBluetoothServiceDiscoveryAgentPrivate::QBluetoothServiceDiscoveryAgentPrivate(
    QBluetoothServiceDiscoveryAgent *qp, const QBluetoothAddress &localAddress) :

    error(QBluetoothServiceDiscoveryAgent::NoError),
    m_deviceAdapterAddress(localAddress),
    state(Inactive),
    mode(QBluetoothServiceDiscoveryAgent::MinimalDiscovery),
    singleDevice(false),
    q_ptr(qp)

{
    Q_ASSERT(q_ptr);
    serviceInquiry.reset([[ObjCServiceInquiry alloc] initWithDelegate:this], RetainPolicy::noInitialRetain);
}

QBluetoothServiceDiscoveryAgentPrivate::~QBluetoothServiceDiscoveryAgentPrivate()
{
}

void QBluetoothServiceDiscoveryAgentPrivate::start(const QBluetoothAddress &deviceAddress)
{
    QT_BT_MAC_AUTORELEASEPOOL;

    if (deviceAddress.isNull()) {
        // This can happen: LE scan works with CoreBluetooth, but CBPeripherals
        // do not expose hardware addresses.
        // Pop the current QBluetoothDeviceInfo and decide what to do next.
        return _q_serviceDiscoveryFinished();
    }

    // Autoreleased object.
    IOBluetoothHostController *const hc = [IOBluetoothHostController defaultController];
    if (![hc powerState]) {
        discoveredDevices.clear();
        if (singleDevice) {
            error = QBluetoothServiceDiscoveryAgent::PoweredOffError;
            errorString = QCoreApplication::translate(SERVICE_DISCOVERY, SD_LOCAL_DEV_OFF);
            emit q_ptr->error(error);
        }

        return _q_serviceDiscoveryFinished();
    }

    if (DiscoveryMode() == QBluetoothServiceDiscoveryAgent::MinimalDiscovery) {
        performMinimalServiceDiscovery(deviceAddress);
    } else {
        IOReturn result = kIOReturnSuccess;
        auto nativeInquiry = serviceInquiry.getAs<ObjCServiceInquiry>();
        if (uuidFilter.size())
            result = [nativeInquiry performSDPQueryWithDevice:deviceAddress filters:uuidFilter];
        else
            result = [nativeInquiry performSDPQueryWithDevice:deviceAddress];

        if (result != kIOReturnSuccess) {
            // Failed immediately to perform an SDP inquiry on IOBluetoothDevice:
            SDPInquiryError(nil, result);
        }
    }
}

void QBluetoothServiceDiscoveryAgentPrivate::stop()
{
    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    discoveredDevices.clear();

    // "Stops" immediately.
    [serviceInquiry.getAs<ObjCServiceInquiry>() stopSDPQuery];

    emit q_ptr->canceled();
}

void QBluetoothServiceDiscoveryAgentPrivate::SDPInquiryFinished(void *generic)
{
    auto device = static_cast<IOBluetoothDevice *>(generic);
    Q_ASSERT_X(device, Q_FUNC_INFO, "invalid IOBluetoothDevice (nil)");

    if (state == Inactive)
        return;

    QT_BT_MAC_AUTORELEASEPOOL;

    NSArray *const records = device.services;
    for (IOBluetoothSDPServiceRecord *record in records) {
        QBluetoothServiceInfo serviceInfo;
        Q_ASSERT_X(discoveredDevices.size() >= 1, Q_FUNC_INFO, "invalid number of devices");

        serviceInfo.setDevice(discoveredDevices.at(0));
        OSXBluetooth::extract_service_record(record, serviceInfo);

        if (!serviceInfo.isValid())
            continue;

        if (!isDuplicatedService(serviceInfo)) {
            discoveredServices.append(serviceInfo);
            emit q_ptr->serviceDiscovered(serviceInfo);
            // Here a user code can ... interrupt us by calling
            // stop. Check this.
            if (state == Inactive)
                break;
        }
    }

    _q_serviceDiscoveryFinished();
}

void QBluetoothServiceDiscoveryAgentPrivate::SDPInquiryError(void *device, IOReturn errorCode)
{
    Q_UNUSED(device)

    qCWarning(QT_BT_OSX) << "inquiry failed with IOKit code:" << int(errorCode);

    discoveredDevices.clear();
    // TODO: find a better mapping from IOReturn to QBluetoothServiceDiscoveryAgent::Error.
    if (singleDevice) {
        error = QBluetoothServiceDiscoveryAgent::UnknownError;
        errorString = QCoreApplication::translate(DEV_DISCOVERY, DD_UNKNOWN_ERROR);
        emit q_ptr->error(error);
    }

    _q_serviceDiscoveryFinished();
}

void QBluetoothServiceDiscoveryAgentPrivate::performMinimalServiceDiscovery(const QBluetoothAddress &deviceAddress)
{
    Q_ASSERT_X(!deviceAddress.isNull(), Q_FUNC_INFO, "invalid device address");

    QT_BT_MAC_AUTORELEASEPOOL;

    const BluetoothDeviceAddress iobtAddress = OSXBluetooth::iobluetooth_address(deviceAddress);
    IOBluetoothDevice *const device = [IOBluetoothDevice deviceWithAddress:&iobtAddress];
    if (!device || !device.services) {
        if (singleDevice) {
            error = QBluetoothServiceDiscoveryAgent::UnknownError;
            errorString = QCoreApplication::translate(SERVICE_DISCOVERY, SD_MINIMAL_FAILED);
            emit q_ptr->error(error);
        }
    } else {

        NSArray *const records = device.services;
        for (IOBluetoothSDPServiceRecord *record in records) {
            QBluetoothServiceInfo serviceInfo;
            Q_ASSERT_X(discoveredDevices.size() >= 1, Q_FUNC_INFO,
                       "invalid number of devices");

            serviceInfo.setDevice(discoveredDevices.at(0));
            OSXBluetooth::extract_service_record(record, serviceInfo);

            if (!serviceInfo.isValid())
                continue;

            if (!uuidFilter.isEmpty() && !serviceHasMatchingUuid(serviceInfo))
                continue;

            if (!isDuplicatedService(serviceInfo)) {
                discoveredServices.append(serviceInfo);
                emit q_ptr->serviceDiscovered(serviceInfo);
            }
        }
    }

    _q_serviceDiscoveryFinished();
}

bool QBluetoothServiceDiscoveryAgentPrivate::serviceHasMatchingUuid(const QBluetoothServiceInfo &serviceInfo) const
{
    for (const auto &requestedUuid : uuidFilter) {
        if (serviceInfo.serviceUuid() == requestedUuid)
            return true;
        if (serviceInfo.serviceClassUuids().contains(requestedUuid))
            return true;
    }
    return false;
}

QT_END_NAMESPACE
