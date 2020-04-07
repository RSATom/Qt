/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Copyright (C) 2014 Denis Shienkov <denis.shienkov@gmail.com>
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

#include "qbluetoothlocaldevice.h"
#include "qbluetoothaddress.h"

#include "qbluetoothlocaldevice_p.h"

#include <QtCore/QLoggingCategory>

#include <qt_windows.h>
#include <bluetoothapis.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_BT_WINDOWS)

QBluetoothLocalDevice::QBluetoothLocalDevice(QObject *parent) :
    QObject(parent),
    d_ptr(new QBluetoothLocalDevicePrivate(this))
{
}

QBluetoothLocalDevice::QBluetoothLocalDevice(
        const QBluetoothAddress &address, QObject *parent) :
    QObject(parent),
    d_ptr(new QBluetoothLocalDevicePrivate(this, address))
{
}

QString QBluetoothLocalDevice::name() const
{
    Q_D(const QBluetoothLocalDevice);
    return d->deviceName;
}

QBluetoothAddress QBluetoothLocalDevice::address() const
{
    Q_D(const QBluetoothLocalDevice);
    return d->deviceAddress;
}

void QBluetoothLocalDevice::powerOn()
{
    if (hostMode() != HostPoweredOff)
        return;

    setHostMode(HostConnectable);
}

void QBluetoothLocalDevice::setHostMode(
        QBluetoothLocalDevice::HostMode requestedMode)
{
    if (!isValid()) {
        qCWarning(QT_BT_WINDOWS) << "The local device is not initialized correctly";
        return;
    }

    if (requestedMode == HostDiscoverableLimitedInquiry)
        requestedMode = HostDiscoverable;

    if (requestedMode == hostMode())
        return;

    if (requestedMode == QBluetoothLocalDevice::HostPoweredOff) {
        if (::BluetoothIsDiscoverable(nullptr)
                && !::BluetoothEnableDiscovery(nullptr, FALSE)) {
            qCWarning(QT_BT_WINDOWS) << "Unable to disable the discoverable mode";
            emit error(QBluetoothLocalDevice::UnknownError);
            return;
        }
        if (::BluetoothIsConnectable(nullptr)
                && !::BluetoothEnableIncomingConnections(nullptr, FALSE)) {
            qCWarning(QT_BT_WINDOWS) << "Unable to disable the connectable mode";
            emit error(QBluetoothLocalDevice::UnknownError);
            return;
        }
    } else if (requestedMode == QBluetoothLocalDevice::HostConnectable) {
        if (!::BluetoothIsConnectable(nullptr)
                && !::BluetoothEnableIncomingConnections(nullptr, TRUE)) {
            qCWarning(QT_BT_WINDOWS) << "Unable to enable the connectable mode";
            emit error(QBluetoothLocalDevice::UnknownError);
            return;
        }
    } else if (requestedMode == QBluetoothLocalDevice::HostDiscoverable
               || requestedMode == QBluetoothLocalDevice::HostDiscoverableLimitedInquiry) {
        if (!::BluetoothIsConnectable(nullptr)
                && !::BluetoothEnableIncomingConnections(nullptr, TRUE)) {
            qCWarning(QT_BT_WINDOWS) << "Unable to enable the connectable mode";
            emit error(QBluetoothLocalDevice::UnknownError);
            return;
        }
        if (!::BluetoothEnableDiscovery(nullptr, TRUE)) {
            qCWarning(QT_BT_WINDOWS) << "Unable to enable the discoverable mode";
            emit error(QBluetoothLocalDevice::UnknownError);
            return;
        }
    }

    emit hostModeStateChanged(requestedMode);
}

QBluetoothLocalDevice::HostMode QBluetoothLocalDevice::hostMode() const
{
    if (!isValid()) {
        qCWarning(QT_BT_WINDOWS) << "The local device is not initialized correctly";
        return HostPoweredOff;
    }

    if (::BluetoothIsDiscoverable(nullptr))
        return HostDiscoverable;
    if (::BluetoothIsConnectable(nullptr))
        return HostConnectable;
    return HostPoweredOff;
}

QList<QBluetoothAddress> QBluetoothLocalDevice::connectedDevices() const
{
    return QList<QBluetoothAddress>();
}

QList<QBluetoothHostInfo> QBluetoothLocalDevice::allDevices()
{
    return QBluetoothLocalDevicePrivate::localAdapters();
}

void QBluetoothLocalDevice::requestPairing(const QBluetoothAddress &address, Pairing pairing)
{
    Q_UNUSED(address);
    Q_UNUSED(pairing);
    Q_UNIMPLEMENTED();
}

QBluetoothLocalDevice::Pairing QBluetoothLocalDevice::pairingStatus(
    const QBluetoothAddress &address) const
{
    Q_UNUSED(address);
    Q_UNIMPLEMENTED();
    return Unpaired;
}

void QBluetoothLocalDevice::pairingConfirmation(bool confirmation)
{
    Q_UNUSED(confirmation);
    Q_UNIMPLEMENTED();
}

QBluetoothLocalDevicePrivate::QBluetoothLocalDevicePrivate(
        QBluetoothLocalDevice *q, const QBluetoothAddress &address)
    : deviceValid(false)
    , q_ptr(q)
{
    initialize(address);
}

QBluetoothLocalDevicePrivate::~QBluetoothLocalDevicePrivate()
{
}

bool QBluetoothLocalDevicePrivate::isValid() const
{
    return deviceValid;
}

void QBluetoothLocalDevicePrivate::initialize(const QBluetoothAddress &address)
{
    Q_Q(QBluetoothLocalDevice);

    const QList<QBluetoothHostInfo> adapterInfos = QBluetoothLocalDevicePrivate::localAdapters();
    for (const QBluetoothHostInfo &adapterInfo : adapterInfos) {
        if (address == QBluetoothAddress()
             || address == adapterInfo.address()) {
            deviceAddress = adapterInfo.address();
            deviceName = adapterInfo.name();
            deviceValid = true;
            return;
        }
    }

    qCWarning(QT_BT_WINDOWS) << "Unable to find classic local radio: " << address;
    QMetaObject::invokeMethod(q, "error", Qt::QueuedConnection,
                              Q_ARG(QBluetoothLocalDevice::Error,
                                    QBluetoothLocalDevice::UnknownError));
}

QList<QBluetoothHostInfo> QBluetoothLocalDevicePrivate::localAdapters()
{
    BLUETOOTH_FIND_RADIO_PARAMS params;
    ::ZeroMemory(&params, sizeof(params));
    params.dwSize = sizeof(params);

    QList<QBluetoothHostInfo> foundAdapters;

    HANDLE hRadio = nullptr;
    if (const HBLUETOOTH_RADIO_FIND hSearch = ::BluetoothFindFirstRadio(&params, &hRadio)) {
        for (;;) {
            BLUETOOTH_RADIO_INFO radio;
            ::ZeroMemory(&radio, sizeof(radio));
            radio.dwSize = sizeof(radio);

            const DWORD retval = ::BluetoothGetRadioInfo(hRadio, &radio);
            ::CloseHandle(hRadio);

            if (retval != ERROR_SUCCESS)
                break;

            QBluetoothHostInfo adapterInfo;
            adapterInfo.setAddress(QBluetoothAddress(radio.address.ullLong));
            adapterInfo.setName(QString::fromWCharArray(radio.szName));

            foundAdapters << adapterInfo;

            if (!::BluetoothFindNextRadio(hSearch, &hRadio))
                break;
        }

        ::BluetoothFindRadioClose(hSearch);
    }

    return foundAdapters;
}

QT_END_NAMESPACE
