/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#ifdef CLASSIC_APP_BUILD
#define Q_OS_WINRT
#endif
#include <QtCore/qfunctions_winrt.h>

#include <robuffer.h>
#include <windows.devices.bluetooth.h>
#include <wrl.h>

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Devices::Bluetooth;
using namespace ABI::Windows::Devices::Enumeration;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

QT_BEGIN_NAMESPACE

template <class DeviceStatics, class OpResult, class Device, class Device2>
ComPtr<IDeviceInformationPairing> getPairingInfo(ComPtr<DeviceStatics> deviceStatics,
                                                  const QBluetoothAddress &address)
{
    ComPtr<IAsyncOperation<OpResult *>> op;
    if (!deviceStatics)
        return nullptr;
    HRESULT hr = deviceStatics->FromBluetoothAddressAsync(address.toUInt64(), &op);
    RETURN_IF_FAILED("Could not obtain device from address", return nullptr);
    ComPtr<Device> device;
    hr = QWinRTFunctions::await(op, device.GetAddressOf(),
                                QWinRTFunctions::ProcessMainThreadEvents, 5000);
    if (FAILED(hr) || !device) {
        qErrnoWarning("Could not obtain device from address");
        return nullptr;
    }
    ComPtr<Device2> device2;
    hr = device.As(&device2);
    RETURN_IF_FAILED("Could not cast device", return nullptr);
    ComPtr<IDeviceInformation> deviceInfo;
    hr = device2->get_DeviceInformation(&deviceInfo);
    if (FAILED(hr) || !deviceInfo) {
        qErrnoWarning("Could not obtain device information");
        return nullptr;
    }
    ComPtr<IDeviceInformation2> deviceInfo2;
    hr = deviceInfo.As(&deviceInfo2);
    RETURN_IF_FAILED("Could not cast device information", return nullptr);
    ComPtr<IDeviceInformationPairing> pairingInfo;
    hr = deviceInfo2->get_Pairing(&pairingInfo);
    RETURN_IF_FAILED("Could not obtain pairing information", return nullptr);
    return pairingInfo;
}

QBluetoothLocalDevice::QBluetoothLocalDevice(QObject *parent) :
    QObject(parent),
    d_ptr(new QBluetoothLocalDevicePrivate(this, QBluetoothAddress()))
{
    registerQBluetoothLocalDeviceMetaType();
}

QBluetoothLocalDevice::QBluetoothLocalDevice(const QBluetoothAddress &address, QObject *parent) :
    QObject(parent),
    d_ptr(new QBluetoothLocalDevicePrivate(this, address))
{
    registerQBluetoothLocalDeviceMetaType();
}

QBluetoothLocalDevicePrivate::QBluetoothLocalDevicePrivate(QBluetoothLocalDevice *q, QBluetoothAddress)
    : q_ptr(q)
{
    GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Devices_Bluetooth_BluetoothLEDevice).Get(), &mLEStatics);
    GetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Devices_Bluetooth_BluetoothDevice).Get(), &mStatics);
}

QBluetoothLocalDevicePrivate::~QBluetoothLocalDevicePrivate() = default;

bool QBluetoothLocalDevicePrivate::isValid() const
{
    return (mStatics != nullptr && mLEStatics != nullptr);
}

void QBluetoothLocalDevice::requestPairing(const QBluetoothAddress &address, Pairing pairing)
{
    Q_UNUSED(address);
    Q_UNUSED(pairing);
    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection,
                              Q_ARG(QBluetoothLocalDevice::Error,
                                    QBluetoothLocalDevice::PairingError));
}

QBluetoothLocalDevice::Pairing QBluetoothLocalDevice::pairingStatus(
    const QBluetoothAddress &address) const
{
    if (!isValid() || address.isNull())
        return QBluetoothLocalDevice::Unpaired;

    ComPtr<IDeviceInformationPairing> pairingInfo = getPairingInfo<IBluetoothLEDeviceStatics,
            BluetoothLEDevice, IBluetoothLEDevice, IBluetoothLEDevice2>(d_ptr->mLEStatics, address);
    if (!pairingInfo)
        pairingInfo = getPairingInfo<IBluetoothDeviceStatics, BluetoothDevice,
                IBluetoothDevice, IBluetoothDevice2>(d_ptr->mStatics, address);
    if (!pairingInfo)
        return QBluetoothLocalDevice::Unpaired;
    boolean isPaired;
    HRESULT hr = pairingInfo->get_IsPaired(&isPaired);
    RETURN_IF_FAILED("Could not obtain device pairing", return QBluetoothLocalDevice::Unpaired);
    if (!isPaired)
        return QBluetoothLocalDevice::Unpaired;

    ComPtr<IDeviceInformationPairing2> pairingInfo2;
    hr = pairingInfo.As(&pairingInfo2);
    RETURN_IF_FAILED("Could not cast pairing info", return QBluetoothLocalDevice::Paired);
    DevicePairingProtectionLevel protection = DevicePairingProtectionLevel_None;
    hr = pairingInfo2->get_ProtectionLevel(&protection);
    RETURN_IF_FAILED("Could not obtain pairing protection level", return QBluetoothLocalDevice::Paired);
    if (protection == DevicePairingProtectionLevel_Encryption
            || protection == DevicePairingProtectionLevel_EncryptionAndAuthentication)
        return QBluetoothLocalDevice::AuthorizedPaired;
    return QBluetoothLocalDevice::Paired;
}

void QBluetoothLocalDevice::pairingConfirmation(bool confirmation)
{
    Q_UNUSED(confirmation);
}

void QBluetoothLocalDevice::setHostMode(QBluetoothLocalDevice::HostMode mode)
{
    Q_UNUSED(mode);
}

QBluetoothLocalDevice::HostMode QBluetoothLocalDevice::hostMode() const
{
    return HostConnectable;
}

QList<QBluetoothAddress> QBluetoothLocalDevice::connectedDevices() const
{
    return QList<QBluetoothAddress>();
}

void QBluetoothLocalDevice::powerOn()
{
}

QString QBluetoothLocalDevice::name() const
{
    return QString();
}

QBluetoothAddress QBluetoothLocalDevice::address() const
{
    return QBluetoothAddress();
}

QList<QBluetoothHostInfo> QBluetoothLocalDevice::allDevices()
{
    QList<QBluetoothHostInfo> localDevices;
    return localDevices;
}

QT_END_NAMESPACE
