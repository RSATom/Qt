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

#include "qbluetoothdevicediscoveryagent.h"
#include "qbluetoothuuid.h"
#include "qbluetoothdevicediscoveryagent_p.h"
#include "qbluetoothlocaldevice_p.h"

#include <QtCore/qmutex.h>
#include <QtCore/QThread>
#include <QtCore/QLoggingCategory>

#include <qt_windows.h>
#include <setupapi.h>
#include <bluetoothapis.h>

#include <algorithm>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_BT_WINDOWS)

struct LeDeviceEntry {
    QString devicePath;
    QBluetoothAddress deviceAddress;
};

Q_GLOBAL_STATIC(QVector<LeDeviceEntry>, cachedLeDeviceEntries)
Q_GLOBAL_STATIC(QMutex, cachedLeDeviceEntriesGuard)

static QString devicePropertyString(
        HDEVINFO hDeviceInfo,
        const PSP_DEVINFO_DATA deviceInfoData,
        DWORD registryProperty)
{
    DWORD propertyRegDataType = 0;
    DWORD propertyBufferSize = 0;
    QByteArray propertyBuffer;

    while (!::SetupDiGetDeviceRegistryProperty(
               hDeviceInfo,
               deviceInfoData,
               registryProperty,
               &propertyRegDataType,
               propertyBuffer.isEmpty() ? nullptr : reinterpret_cast<PBYTE>(propertyBuffer.data()),
               propertyBuffer.size(),
               &propertyBufferSize)) {

        const DWORD error = ::GetLastError();
        if (error != ERROR_INSUFFICIENT_BUFFER
                || (propertyRegDataType != REG_SZ
                    && propertyRegDataType != REG_EXPAND_SZ)) {
            return QString();
        }

        // add +2 byte to allow to successfully convert to qstring
        propertyBuffer.fill(0, propertyBufferSize + sizeof(wchar_t));
    }

    return QString::fromWCharArray(reinterpret_cast<const wchar_t *>(
                                       propertyBuffer.constData()));
}

static QString deviceName(HDEVINFO hDeviceInfo, PSP_DEVINFO_DATA deviceInfoData)
{
    return devicePropertyString(hDeviceInfo, deviceInfoData, SPDRP_FRIENDLYNAME);
}

static QString deviceSystemPath(const PSP_INTERFACE_DEVICE_DETAIL_DATA detailData)
{
    return QString::fromWCharArray(detailData->DevicePath);
}

static QBluetoothAddress deviceAddress(const QString &devicePath)
{
    const int firstbound = devicePath.indexOf(QStringLiteral("dev_"));
    const int lastbound = devicePath.indexOf(QLatin1Char('#'), firstbound);
    const QString hex = devicePath.mid(firstbound + 4, lastbound - firstbound - 4);
    bool ok = false;
    return QBluetoothAddress(hex.toULongLong(&ok, 16));
}

static QBluetoothDeviceInfo createClassicDeviceInfo(const BLUETOOTH_DEVICE_INFO &foundDevice)
{
    QBluetoothDeviceInfo deviceInfo(
                QBluetoothAddress(foundDevice.Address.ullLong),
                QString::fromWCharArray(foundDevice.szName),
                foundDevice.ulClassofDevice);

    deviceInfo.setCoreConfigurations(QBluetoothDeviceInfo::BaseRateCoreConfiguration);

    if (foundDevice.fRemembered)
        deviceInfo.setCached(true);
    return deviceInfo;
}

static QBluetoothDeviceInfo findFirstClassicDevice(
        DWORD *systemErrorCode, HBLUETOOTH_DEVICE_FIND *hSearch)
{
    BLUETOOTH_DEVICE_SEARCH_PARAMS searchParams = {};
    searchParams.dwSize = sizeof(searchParams);
    searchParams.cTimeoutMultiplier = 10; // 12.8 sec
    searchParams.fIssueInquiry = TRUE;
    searchParams.fReturnAuthenticated = TRUE;
    searchParams.fReturnConnected = TRUE;
    searchParams.fReturnRemembered = TRUE;
    searchParams.fReturnUnknown = TRUE;
    searchParams.hRadio = nullptr;

    BLUETOOTH_DEVICE_INFO deviceInfo = {};
    deviceInfo.dwSize = sizeof(deviceInfo);

    const HBLUETOOTH_DEVICE_FIND hFind = ::BluetoothFindFirstDevice(
                &searchParams, &deviceInfo);

    QBluetoothDeviceInfo foundDevice;
    if (hFind) {
        *hSearch = hFind;
        *systemErrorCode = NO_ERROR;
        foundDevice = createClassicDeviceInfo(deviceInfo);
    } else {
        *systemErrorCode = int(::GetLastError());
    }

    return foundDevice;
}

static QBluetoothDeviceInfo findNextClassicDevice(
        DWORD *systemErrorCode, HBLUETOOTH_DEVICE_FIND hSearch)
{
    BLUETOOTH_DEVICE_INFO deviceInfo = {};
    deviceInfo.dwSize = sizeof(deviceInfo);

    QBluetoothDeviceInfo foundDevice;
    if (!::BluetoothFindNextDevice(hSearch, &deviceInfo)) {
        *systemErrorCode = int(::GetLastError());
    } else {
        *systemErrorCode = NO_ERROR;
        foundDevice = createClassicDeviceInfo(deviceInfo);
    }

    return foundDevice;
}

static void closeClassicSearch(HBLUETOOTH_DEVICE_FIND hSearch)
{
    if (hSearch)
        ::BluetoothFindDeviceClose(hSearch);
}

static QVector<QBluetoothDeviceInfo> enumerateLeDevices(
        DWORD *systemErrorCode)
{
    // GUID_BLUETOOTHLE_DEVICE_INTERFACE
    const QUuid deviceInterfaceGuid("781aee18-7733-4ce4-add0-91f41c67b592");
    const HDEVINFO hDeviceInfo = ::SetupDiGetClassDevs(
                reinterpret_cast<const GUID *>(&deviceInterfaceGuid),
                nullptr,
                nullptr,
                DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

    if (hDeviceInfo == INVALID_HANDLE_VALUE) {
        *systemErrorCode = int(::GetLastError());
        return QVector<QBluetoothDeviceInfo>();
    }

    QVector<QBluetoothDeviceInfo> foundDevices;
    DWORD index = 0;

    QVector<LeDeviceEntry> cachedEntries;

    for (;;) {
        SP_DEVICE_INTERFACE_DATA deviceInterfaceData = {};
        deviceInterfaceData.cbSize = sizeof(deviceInterfaceData);

        if (!::SetupDiEnumDeviceInterfaces(
                    hDeviceInfo,
                    nullptr,
                    reinterpret_cast<const GUID *>(&deviceInterfaceGuid),
                    index++,
                    &deviceInterfaceData)) {
            *systemErrorCode = int(::GetLastError());
            break;
        }

        DWORD deviceInterfaceDetailDataSize = 0;
        if (!::SetupDiGetDeviceInterfaceDetail(
                    hDeviceInfo,
                    &deviceInterfaceData,
                    nullptr,
                    deviceInterfaceDetailDataSize,
                    &deviceInterfaceDetailDataSize,
                    nullptr)) {
            if (::GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
                *systemErrorCode = int(::GetLastError());
                break;
            }
        }

        SP_DEVINFO_DATA deviceInfoData = {};
        deviceInfoData.cbSize = sizeof(deviceInfoData);

        QByteArray deviceInterfaceDetailDataBuffer(
                    deviceInterfaceDetailDataSize, 0);

        PSP_INTERFACE_DEVICE_DETAIL_DATA deviceInterfaceDetailData =
                reinterpret_cast<PSP_INTERFACE_DEVICE_DETAIL_DATA>
                (deviceInterfaceDetailDataBuffer.data());

        deviceInterfaceDetailData->cbSize =
                sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

        if (!::SetupDiGetDeviceInterfaceDetail(
                    hDeviceInfo,
                    &deviceInterfaceData,
                    deviceInterfaceDetailData,
                    deviceInterfaceDetailDataBuffer.size(),
                    &deviceInterfaceDetailDataSize,
                    &deviceInfoData)) {
            *systemErrorCode = int(::GetLastError());
            break;
        }

        const QString systemPath = deviceSystemPath(deviceInterfaceDetailData);
        const QBluetoothAddress address = deviceAddress(systemPath);
        if (address.isNull())
            continue;
        const QString name = deviceName(hDeviceInfo, &deviceInfoData);

        QBluetoothDeviceInfo deviceInfo(address, name, QBluetoothDeviceInfo::MiscellaneousDevice);
        deviceInfo.setCoreConfigurations(QBluetoothDeviceInfo::LowEnergyCoreConfiguration);
        deviceInfo.setCached(true);

        foundDevices << deviceInfo;
        cachedEntries << LeDeviceEntry{systemPath, address};
    }

    QMutexLocker locker(cachedLeDeviceEntriesGuard());
    cachedLeDeviceEntries()->swap(cachedEntries);

    ::SetupDiDestroyDeviceInfoList(hDeviceInfo);
    return foundDevices;
}

struct DiscoveryResult {
    QVector<QBluetoothDeviceInfo> devices;
    DWORD systemErrorCode;
    HBLUETOOTH_DEVICE_FIND hSearch; // Used only for classic devices
};

QString QBluetoothDeviceDiscoveryAgentPrivate::discoveredLeDeviceSystemPath(
        const QBluetoothAddress &deviceAddress)
{
    // update LE devices cache
    DWORD dummyErrorCode;
    enumerateLeDevices(&dummyErrorCode);

    QMutexLocker locker(cachedLeDeviceEntriesGuard());
    for (const LeDeviceEntry &e: *cachedLeDeviceEntries) {
        if (e.deviceAddress == deviceAddress)
            return e.devicePath;
    }
    return QString();
}

QBluetoothDeviceDiscoveryAgentPrivate::QBluetoothDeviceDiscoveryAgentPrivate(
        const QBluetoothAddress &deviceAdapter,
        QBluetoothDeviceDiscoveryAgent *parent)
    : inquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry)
    , lastError(QBluetoothDeviceDiscoveryAgent::NoError)
    , adapterAddress(deviceAdapter)
    , pendingCancel(false)
    , pendingStart(false)
    , active(false)
    , lowEnergySearchTimeout(-1) // remains -1 -> timeout not supported
    , q_ptr(parent)
{
    threadLE = new QThread;
    threadWorkerLE = new ThreadWorkerDeviceDiscovery;
    threadWorkerLE->moveToThread(threadLE);
    connect(threadWorkerLE, &ThreadWorkerDeviceDiscovery::discoveryCompleted, this, &QBluetoothDeviceDiscoveryAgentPrivate::completeLeDevicesDiscovery);
    connect(threadLE, &QThread::finished, threadWorkerLE, &ThreadWorkerDeviceDiscovery::deleteLater);
    connect(threadLE, &QThread::finished, threadLE, &QThread::deleteLater);
    threadLE->start();

    threadClassic = new QThread;
    threadWorkerClassic = new ThreadWorkerDeviceDiscovery;
    threadWorkerClassic->moveToThread(threadClassic);
    connect(threadWorkerClassic, &ThreadWorkerDeviceDiscovery::discoveryCompleted, this, &QBluetoothDeviceDiscoveryAgentPrivate::completeClassicDevicesDiscovery);
    connect(threadClassic, &QThread::finished, threadWorkerClassic, &ThreadWorkerDeviceDiscovery::deleteLater);
    connect(threadClassic, &QThread::finished, threadClassic, &QThread::deleteLater);
    threadClassic->start();
}

QBluetoothDeviceDiscoveryAgentPrivate::~QBluetoothDeviceDiscoveryAgentPrivate()
{
    if (active)
        stop();
    if (threadLE)
        threadLE->quit();
    if (threadClassic)
        threadClassic->quit();
}

bool QBluetoothDeviceDiscoveryAgentPrivate::isActive() const
{
    if (pendingStart)
        return true;
    if (pendingCancel)
        return false;
    return active;
}

QBluetoothDeviceDiscoveryAgent::DiscoveryMethods QBluetoothDeviceDiscoveryAgent::supportedDiscoveryMethods()
{
    return (LowEnergyMethod | ClassicMethod);
}

void QBluetoothDeviceDiscoveryAgentPrivate::start(QBluetoothDeviceDiscoveryAgent::DiscoveryMethods methods)
{
    requestedMethods = methods;

    if (pendingCancel == true) {
        pendingStart = true;
        return;
    }

    const QList<QBluetoothHostInfo> foundLocalAdapters =
            QBluetoothLocalDevicePrivate::localAdapters();

    Q_Q(QBluetoothDeviceDiscoveryAgent);

    if (foundLocalAdapters.isEmpty()) {
        qCWarning(QT_BT_WINDOWS) << "Device does not support Bluetooth";
        lastError = QBluetoothDeviceDiscoveryAgent::InputOutputError;
        errorString = QBluetoothDeviceDiscoveryAgent::tr("Device does not support Bluetooth");
        emit q->error(lastError);
        return;
    }

    // Check for the local adapter address.
    auto equals = [this](const QBluetoothHostInfo &adapterInfo) {
        return adapterAddress == QBluetoothAddress()
                || adapterAddress == adapterInfo.address();
    };
    const auto end = foundLocalAdapters.cend();
    const auto it = std::find_if(foundLocalAdapters.cbegin(), end, equals);
    if (it == end) {
        qCWarning(QT_BT_WINDOWS) << "Incorrect local adapter passed.";
        lastError = QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError;
        errorString = QBluetoothDeviceDiscoveryAgent::tr("Passed address is not a local device.");
        emit q->error(lastError);
        return;
    }

    discoveredDevices.clear();
    active = true;

    // We run LE search first, as it is fast on windows.
    if (requestedMethods & QBluetoothDeviceDiscoveryAgent::LowEnergyMethod)
        startLeDevicesDiscovery();
    else if (requestedMethods & QBluetoothDeviceDiscoveryAgent::ClassicMethod)
        startClassicDevicesDiscovery();
}

void QBluetoothDeviceDiscoveryAgentPrivate::stop()
{
    if (!active)
        return;

    pendingCancel = true;
    pendingStart = false;
}

void QBluetoothDeviceDiscoveryAgentPrivate::cancelDiscovery()
{
    Q_Q(QBluetoothDeviceDiscoveryAgent);
    active = false;
    pendingCancel = false;
    emit q->canceled();
}

void QBluetoothDeviceDiscoveryAgentPrivate::restartDiscovery()
{
    pendingStart = false;
    pendingCancel = false;
    start(requestedMethods);
}

void QBluetoothDeviceDiscoveryAgentPrivate::finishDiscovery(QBluetoothDeviceDiscoveryAgent::Error errorCode, const QString &errorText)
{
    active = false;
    pendingStart = false;
    pendingCancel = false;
    lastError = errorCode;
    errorString = errorText;

    Q_Q(QBluetoothDeviceDiscoveryAgent);
    if (errorCode == QBluetoothDeviceDiscoveryAgent::NoError)
        emit q->finished();
    else
        emit q->error(lastError);
}

void QBluetoothDeviceDiscoveryAgentPrivate::startLeDevicesDiscovery()
{
    const auto threadWorker = threadWorkerLE;
    QMetaObject::invokeMethod(threadWorkerLE, [threadWorker]()
    {
        DiscoveryResult result; // Do not use hSearch here!
        result.systemErrorCode = NO_ERROR;
        result.devices = enumerateLeDevices(&result.systemErrorCode);
        emit threadWorker->discoveryCompleted(QVariant::fromValue(result));
    }, Qt::QueuedConnection);
}

void QBluetoothDeviceDiscoveryAgentPrivate::completeLeDevicesDiscovery(const QVariant &res)
{
    if (pendingCancel && !pendingStart) {
        cancelDiscovery();
    } else if (pendingStart) {
        restartDiscovery();
    } else {
        const DiscoveryResult result = res.value<DiscoveryResult>();
        if (result.systemErrorCode == NO_ERROR || result.systemErrorCode == ERROR_NO_MORE_ITEMS) {
            for (const QBluetoothDeviceInfo &device : result.devices)
                processDiscoveredDevice(device);

            // We run classic search at second, as it is slow on windows.
            if (requestedMethods & QBluetoothDeviceDiscoveryAgent::ClassicMethod)
                startClassicDevicesDiscovery();
            else
                finishDiscovery(QBluetoothDeviceDiscoveryAgent::NoError, qt_error_string(NO_ERROR));
        } else {
            const QBluetoothDeviceDiscoveryAgent::Error error = (result.systemErrorCode == ERROR_INVALID_HANDLE)
                    ? QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError
                    : QBluetoothDeviceDiscoveryAgent::InputOutputError;
            finishDiscovery(error, qt_error_string(result.systemErrorCode));
        }
    }
}

void QBluetoothDeviceDiscoveryAgentPrivate::startClassicDevicesDiscovery(Qt::HANDLE hSearch)
{
    const auto threadWorker = threadWorkerClassic;
    QMetaObject::invokeMethod(threadWorker, [threadWorker, hSearch]()
    {
        DiscoveryResult result;
        result.hSearch = hSearch;
        result.systemErrorCode = NO_ERROR;

        const QBluetoothDeviceInfo device = hSearch
                ? findNextClassicDevice(&result.systemErrorCode, result.hSearch)
                : findFirstClassicDevice(&result.systemErrorCode, &result.hSearch);

        result.devices.append(device);
        emit threadWorker->discoveryCompleted(QVariant::fromValue(result));
    }, Qt::QueuedConnection);
}

void QBluetoothDeviceDiscoveryAgentPrivate::completeClassicDevicesDiscovery(const QVariant &res)
{
    const DiscoveryResult result = res.value<DiscoveryResult>();
    if (pendingCancel && !pendingStart) {
        closeClassicSearch(result.hSearch);
        cancelDiscovery();
    } else if (pendingStart) {
        closeClassicSearch(result.hSearch);
        restartDiscovery();
    } else {
        if (result.systemErrorCode == ERROR_NO_MORE_ITEMS) {
            closeClassicSearch(result.hSearch);
            finishDiscovery(QBluetoothDeviceDiscoveryAgent::NoError, QString());
        } else if (result.systemErrorCode == NO_ERROR) {
            if (result.hSearch) {
                for (const QBluetoothDeviceInfo &device : result.devices)
                    processDiscoveredDevice(device);

                startClassicDevicesDiscovery(result.hSearch);
            } else {
                finishDiscovery(QBluetoothDeviceDiscoveryAgent::NoError, QString());
            }
        } else {
            closeClassicSearch(result.hSearch);
            const QBluetoothDeviceDiscoveryAgent::Error error = (result.systemErrorCode == ERROR_INVALID_HANDLE)
                    ? QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError
                    : QBluetoothDeviceDiscoveryAgent::InputOutputError;
            finishDiscovery(error, qt_error_string(result.systemErrorCode));
        }
    }
}

void QBluetoothDeviceDiscoveryAgentPrivate::processDiscoveredDevice(
        const QBluetoothDeviceInfo &foundDevice)
{
    Q_Q(QBluetoothDeviceDiscoveryAgent);

    auto equalAddress = [foundDevice](const QBluetoothDeviceInfo &targetDevice) {
        return foundDevice.address() == targetDevice.address(); };
    auto end = discoveredDevices.end();
    auto deviceIt = std::find_if(discoveredDevices.begin(), end, equalAddress);
    if (deviceIt == end) {
        qCDebug(QT_BT_WINDOWS) << "Found device: " << foundDevice.name() << foundDevice.address();
        discoveredDevices.append(foundDevice);
        emit q->deviceDiscovered(foundDevice);
    } else {
        qCDebug(QT_BT_WINDOWS) << "Updating device:" << deviceIt->name() << deviceIt->address();
        // merge service uuids
        auto uuids = deviceIt->serviceUuids().toVector();
        uuids.append(foundDevice.serviceUuids().toVector());
        std::sort(uuids.begin(), uuids.end());
        uuids.erase(std::unique(uuids.begin(), uuids.end()), uuids.end());
        if (deviceIt->serviceUuids().count() != uuids.count())
            deviceIt->setServiceUuids(uuids);
        if (deviceIt->coreConfigurations() != foundDevice.coreConfigurations())
            deviceIt->setCoreConfigurations(
                        QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration);
    }
}

QT_END_NAMESPACE

Q_DECLARE_METATYPE(DiscoveryResult)
