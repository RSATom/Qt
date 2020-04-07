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

#include "qbluetoothdevicediscoveryagent_p.h"
#include "qbluetoothdevicediscoveryagent.h"

#include "osx/osxbtledeviceinquiry_p.h"
#ifdef Q_OS_MACOS
#include "osx/osxbtdeviceinquiry_p.h"
#include "osx/osxbtsdpinquiry_p.h"
#endif // Q_OS_MACOS
#include "qbluetoothdeviceinfo.h"
#include "osx/osxbtnotifier_p.h"
#include "osx/osxbtutility_p.h"
#include "osx/osxbluetooth_p.h"
#include "osx/uistrings_p.h"
#include "qbluetoothhostinfo.h"
#include "qbluetoothaddress.h"
#include "osx/uistrings_p.h"
#include "qbluetoothuuid.h"
#include "osx/btraii_p.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qvector.h>
#include <QtCore/qglobal.h>
#include <QtCore/qstring.h>
#include <QtCore/qdebug.h>

#include <Foundation/Foundation.h>

QT_BEGIN_NAMESPACE

namespace
{

void registerQDeviceDiscoveryMetaType()
{
    static bool initDone = false;
    if (!initDone) {
        qRegisterMetaType<QBluetoothDeviceInfo>();
        qRegisterMetaType<QBluetoothDeviceDiscoveryAgent::Error>();
        initDone = true;
    }
}
#ifdef Q_OS_MACOS
using InquiryObjC = QT_MANGLE_NAMESPACE(OSXBTDeviceInquiry);
#endif // Q_OS_MACOS

using LEInquiryObjC = QT_MANGLE_NAMESPACE(OSXBTLEDeviceInquiry);

} //namespace

QBluetoothDeviceDiscoveryAgentPrivate::QBluetoothDeviceDiscoveryAgentPrivate(const QBluetoothAddress &adapter,
                                                                             QBluetoothDeviceDiscoveryAgent *q) :
    inquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry),
    lastError(QBluetoothDeviceDiscoveryAgent::NoError),
    agentState(NonActive),
    adapterAddress(adapter),
    startPending(false),
    stopPending(false),
    lowEnergySearchTimeout(OSXBluetooth::defaultLEScanTimeoutMS),
#ifdef Q_OS_MACOS
    requestedMethods(QBluetoothDeviceDiscoveryAgent::ClassicMethod | QBluetoothDeviceDiscoveryAgent::LowEnergyMethod),
#else
    requestedMethods(QBluetoothDeviceDiscoveryAgent::ClassicMethod),
#endif // Q_OS_MACOS
    q_ptr(q)
{
    registerQDeviceDiscoveryMetaType();

    Q_ASSERT_X(q != nullptr, Q_FUNC_INFO, "invalid q_ptr (null)");

#ifdef Q_OS_MACOS
    IOBluetoothHostController *hostController = [IOBluetoothHostController defaultController];
    if (!hostController || [hostController powerState] != kBluetoothHCIPowerStateON) {
        qCCritical(QT_BT_OSX) << "no default host controller or adapter is off";
        return;
    }
    controller.reset(hostController, DarwinBluetooth::RetainPolicy::doInitialRetain);
#endif
}

QBluetoothDeviceDiscoveryAgentPrivate::~QBluetoothDeviceDiscoveryAgentPrivate()
{
    if (inquiryLE && agentState != NonActive) {
        // We want the LE scan to stop as soon as possible.
        if (dispatch_queue_t leQueue = OSXBluetooth::qt_LE_queue()) {
            // Local variable to be retained ...
            LEInquiryObjC *inq = inquiryLE.getAs<LEInquiryObjC>();
            dispatch_sync(leQueue, ^{
                [inq stop];
            });
        }
    }
}

bool QBluetoothDeviceDiscoveryAgentPrivate::isActive() const
{
    if (startPending)
        return true;

    if (stopPending)
        return false;

    return agentState != NonActive;
}

void QBluetoothDeviceDiscoveryAgentPrivate::start(QBluetoothDeviceDiscoveryAgent::DiscoveryMethods methods)
{
    Q_ASSERT(!isActive());
    Q_ASSERT(lastError != QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError);
    Q_ASSERT(methods & (QBluetoothDeviceDiscoveryAgent::ClassicMethod
                        | QBluetoothDeviceDiscoveryAgent::LowEnergyMethod));

#ifdef Q_OS_MACOS
    if (!controller) {
        setError(QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError);
        emit q_ptr->error(lastError);
        return;
    }
#endif // Q_OS_MACOS

    requestedMethods = methods;

    if (stopPending) {
        startPending = true;
        return;
    }

    // This function (re)starts the scan(s) from the scratch;
    // starting from Classic if it's in 'methods' (or LE scan if not).

    agentState = NonActive;
    discoveredDevices.clear();
    setError(QBluetoothDeviceDiscoveryAgent::NoError);
#ifdef Q_OS_MACOS
    if (requestedMethods & QBluetoothDeviceDiscoveryAgent::ClassicMethod)
        return startClassic();
#endif // Q_OS_MACOS

    startLE();
}

#ifdef Q_OS_MACOS

void QBluetoothDeviceDiscoveryAgentPrivate::startClassic()
{
    Q_ASSERT(!isActive());
    Q_ASSERT(lastError == QBluetoothDeviceDiscoveryAgent::NoError);
    Q_ASSERT(requestedMethods & QBluetoothDeviceDiscoveryAgent::ClassicMethod);
    Q_ASSERT(agentState == NonActive);

    OSXBluetooth::qt_test_iobluetooth_runloop();

    if (!inquiry) {
        // The first Classic scan for this DDA.
        inquiry.reset([[InquiryObjC alloc] initWithDelegate:this],
                      DarwinBluetooth::RetainPolicy::noInitialRetain);

        if (!inquiry) {
            qCCritical(QT_BT_OSX) << "failed to initialize an Classic device inquiry";
            setError(QBluetoothDeviceDiscoveryAgent::UnknownError,
                     QCoreApplication::translate(DEV_DISCOVERY, DD_NOT_STARTED));
            emit q_ptr->error(lastError);
            return;
        }
    }

    agentState = ClassicScan;

    const IOReturn res = [inquiry.getAs<InquiryObjC>() start];
    if (res != kIOReturnSuccess) {
        setError(res, QCoreApplication::translate(DEV_DISCOVERY, DD_NOT_STARTED));
        agentState = NonActive;
        emit q_ptr->error(lastError);
    }
}

#endif // Q_OS_MACOS

void QBluetoothDeviceDiscoveryAgentPrivate::startLE()
{
    Q_ASSERT(lastError != QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError);
    Q_ASSERT(requestedMethods & QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    using namespace OSXBluetooth;

    QScopedPointer<LECBManagerNotifier> notifier(new LECBManagerNotifier);
    // Connections:
    using ErrMemFunPtr = void (LECBManagerNotifier::*)(QBluetoothDeviceDiscoveryAgent::Error);
    notifier->connect(notifier.data(), ErrMemFunPtr(&LECBManagerNotifier::CBManagerError),
                      this, &QBluetoothDeviceDiscoveryAgentPrivate::LEinquiryError);
    notifier->connect(notifier.data(), &LECBManagerNotifier::LEnotSupported,
                      this, &QBluetoothDeviceDiscoveryAgentPrivate::LEnotSupported);
    notifier->connect(notifier.data(), &LECBManagerNotifier::discoveryFinished,
                      this, &QBluetoothDeviceDiscoveryAgentPrivate::LEinquiryFinished);
    using DeviceMemFunPtr = void (QBluetoothDeviceDiscoveryAgentPrivate::*)(const QBluetoothDeviceInfo &);
    notifier->connect(notifier.data(), &LECBManagerNotifier::deviceDiscovered,
                      this, DeviceMemFunPtr(&QBluetoothDeviceDiscoveryAgentPrivate::deviceFound));

    // Check queue and create scanner:
    inquiryLE.reset([[LEInquiryObjC alloc] initWithNotifier:notifier.data()],
                    DarwinBluetooth::RetainPolicy::noInitialRetain);
    if (inquiryLE)
        notifier.take(); // Whatever happens next, inquiryLE is already the owner ...

    dispatch_queue_t leQueue(qt_LE_queue());
    if (!leQueue || !inquiryLE) {
        setError(QBluetoothDeviceDiscoveryAgent::UnknownError,
                 QCoreApplication::translate(DEV_DISCOVERY, DD_NOT_STARTED_LE));
        agentState = NonActive;
        emit q_ptr->error(lastError);
        return;
    }

    // Now start in on LE queue:
    agentState = LEScan;
    // We need the local variable so that it's retained ...
    LEInquiryObjC *inq = inquiryLE.getAs<LEInquiryObjC>();
    dispatch_async(leQueue, ^{
        [inq startWithTimeout:lowEnergySearchTimeout];
    });
}

void QBluetoothDeviceDiscoveryAgentPrivate::stop()
{
    Q_ASSERT_X(isActive(), Q_FUNC_INFO, "called whithout active inquiry");
    Q_ASSERT_X(lastError != QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError,
               Q_FUNC_INFO, "called with invalid bluetooth adapter");

    using namespace OSXBluetooth;

    const bool prevStart = startPending;
    startPending = false;
    stopPending = true;

    setError(QBluetoothDeviceDiscoveryAgent::NoError);

#ifdef Q_OS_MACOS
    if (agentState == ClassicScan) {
        const IOReturn res = [inquiry.getAs<InquiryObjC>() stop];
        if (res != kIOReturnSuccess) {
            qCWarning(QT_BT_OSX) << "failed to stop";
            startPending = prevStart;
            stopPending = false;
            setError(res, QCoreApplication::translate(DEV_DISCOVERY, DD_NOT_STOPPED));
            emit q_ptr->error(lastError);
        }
    } else {
#else
    {
        Q_UNUSED(prevStart)
#endif // Q_OS_MACOS
        dispatch_queue_t leQueue(qt_LE_queue());
        Q_ASSERT(leQueue);
        // We need the local variable so that it's retained ...
        LEInquiryObjC *inq = inquiryLE.getAs<LEInquiryObjC>();
        dispatch_sync(leQueue, ^{
            [inq stop];
        });
        // We consider LE scan to be stopped immediately and
        // do not care about this LEDeviceInquiry object anymore.
        LEinquiryFinished();
    }
}

#ifdef Q_OS_MACOS

void QBluetoothDeviceDiscoveryAgentPrivate::inquiryFinished()
{
    // The subsequent start(LE) function (if any)
    // will (re)set the correct state.
    agentState = NonActive;

    if (stopPending && !startPending) {
        stopPending = false;
        emit q_ptr->canceled();
    } else if (startPending) {
        startPending = false;
        stopPending = false;
        start(requestedMethods);
    } else {
        // We can be here _only_ if a classic scan
        // finished in a normal way (not cancelled)
        // and requestedMethods includes LowEnergyMethod.
        // startLE() will take care of old devices
        // not supporting Bluetooth 4.0.
        if (requestedMethods & QBluetoothDeviceDiscoveryAgent::LowEnergyMethod)
            startLE();
        else
            emit q_ptr->finished();
    }
}

void QBluetoothDeviceDiscoveryAgentPrivate::error(IOReturn error)
{
    startPending = false;
    stopPending = false;

    setError(error);

    emit q_ptr->error(lastError);
}

void QBluetoothDeviceDiscoveryAgentPrivate::classicDeviceFound(void *obj)
{
    auto device = static_cast<IOBluetoothDevice *>(obj);
    Q_ASSERT_X(device, Q_FUNC_INFO, "invalid IOBluetoothDevice (nil)");

    Q_ASSERT_X(agentState == ClassicScan, Q_FUNC_INFO,
               "invalid agent state (expected classic scan)");

    QT_BT_MAC_AUTORELEASEPOOL;

    // Let's collect some info about this device:
    const QBluetoothAddress deviceAddress(OSXBluetooth::qt_address([device getAddress]));
    if (deviceAddress.isNull()) {
        qCWarning(QT_BT_OSX) << "invalid Bluetooth address";
        return;
    }

    QString deviceName;
    if (device.name)
        deviceName = QString::fromNSString(device.name);

    const auto classOfDevice = qint32(device.classOfDevice);

    QBluetoothDeviceInfo deviceInfo(deviceAddress, deviceName, classOfDevice);
    deviceInfo.setCoreConfigurations(QBluetoothDeviceInfo::BaseRateCoreConfiguration);
    deviceInfo.setRssi(device.RSSI);

    const QVector<QBluetoothUuid> uuids(OSXBluetooth::extract_services_uuids(device));
    deviceInfo.setServiceUuids(uuids);

    deviceFound(deviceInfo);
}

void QBluetoothDeviceDiscoveryAgentPrivate::setError(IOReturn error, const QString &text)
{
    if (error == kIOReturnSuccess)
        setError(QBluetoothDeviceDiscoveryAgent::NoError, text);
    else if (error == kIOReturnNoPower)
        setError(QBluetoothDeviceDiscoveryAgent::PoweredOffError, text);
    else
        setError(QBluetoothDeviceDiscoveryAgent::UnknownError, text);
}

#endif // Q_OS_MACOS

void QBluetoothDeviceDiscoveryAgentPrivate::setError(QBluetoothDeviceDiscoveryAgent::Error error, const QString &text)
{
    lastError = error;

    if (text.length() > 0) {
        errorString = text;
    } else {
        switch (lastError) {
        case QBluetoothDeviceDiscoveryAgent::NoError:
            errorString = QString();
            break;
        case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
            errorString = QCoreApplication::translate(DEV_DISCOVERY, DD_POWERED_OFF);
            break;
        case QBluetoothDeviceDiscoveryAgent::InvalidBluetoothAdapterError:
            errorString = QCoreApplication::translate(DEV_DISCOVERY, DD_INVALID_ADAPTER);
            break;
        case QBluetoothDeviceDiscoveryAgent::InputOutputError:
            errorString = QCoreApplication::translate(DEV_DISCOVERY, DD_IO);
            break;
        case QBluetoothDeviceDiscoveryAgent::UnsupportedPlatformError:
            errorString = QCoreApplication::translate(DEV_DISCOVERY, DD_NOTSUPPORTED);
            break;
        case QBluetoothDeviceDiscoveryAgent::UnknownError:
        default:
            errorString = QCoreApplication::translate(DEV_DISCOVERY, DD_UNKNOWN_ERROR);
        }
    }
}

void QBluetoothDeviceDiscoveryAgentPrivate::LEinquiryError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    Q_ASSERT(error == QBluetoothDeviceDiscoveryAgent::PoweredOffError
             || error == QBluetoothDeviceDiscoveryAgent::UnsupportedDiscoveryMethod);

    inquiryLE.reset();

    startPending = false;
    stopPending = false;
    agentState = NonActive;
    setError(error);
    emit q_ptr->error(lastError);
}

void QBluetoothDeviceDiscoveryAgentPrivate::LEnotSupported()
{
    qCDebug(QT_BT_OSX) << "no Bluetooth LE support";

#ifdef Q_OS_MACOS
    if (requestedMethods & QBluetoothDeviceDiscoveryAgent::ClassicMethod) {
        // Having both Classic | LE means this is not an error.
        LEinquiryFinished();
    } else {
        // In the past this was never an error, that's why we have
        // LEnotSupported as a special method. But now, since
        // we can have separate Classic/LE scans, we have to report it
        // as UnsupportedDiscoveryMethod.
        LEinquiryError(QBluetoothDeviceDiscoveryAgent::UnsupportedDiscoveryMethod);
    }
#else
    inquiryLE.reset();
    startPending = false;
    stopPending = false;
    setError(QBluetoothDeviceDiscoveryAgent::UnsupportedPlatformError);
    emit q_ptr->error(lastError);
#endif
}

void QBluetoothDeviceDiscoveryAgentPrivate::LEinquiryFinished()
{
    // The same logic as in inquiryFinished, but does not start LE scan.
    agentState = NonActive;
    inquiryLE.reset();

    if (stopPending && !startPending) {
        stopPending = false;
        emit q_ptr->canceled();
    } else if (startPending) {
        startPending = false;
        stopPending = false;
        start(requestedMethods); //Start again.
    } else {
        emit q_ptr->finished();
    }
}

void QBluetoothDeviceDiscoveryAgentPrivate::deviceFound(const QBluetoothDeviceInfo &newDeviceInfo)
{
    // Core Bluetooth does not allow us to access addresses, we have to use uuid instead.
    // This uuid has nothing to do with uuids in Bluetooth in general (it's generated by
    // Apple's framework using some algorithm), but it's a 128-bit uuid after all.
    const bool isLE =
#ifdef Q_OS_MACOS
            newDeviceInfo.coreConfigurations() == QBluetoothDeviceInfo::LowEnergyCoreConfiguration;
#else
            true;
#endif // Q_OS_MACOS
    for (int i = 0, e = discoveredDevices.size(); i < e; ++i) {
        if (isLE) {
            if (discoveredDevices[i].deviceUuid() == newDeviceInfo.deviceUuid()) {
                QBluetoothDeviceInfo::Fields updatedFields = QBluetoothDeviceInfo::Field::None;
                if (discoveredDevices[i].rssi() != newDeviceInfo.rssi()) {
                    qCDebug(QT_BT_OSX) << "Updating RSSI for" << newDeviceInfo.address()
                                       << newDeviceInfo.rssi();
                    discoveredDevices[i].setRssi(newDeviceInfo.rssi());
                    updatedFields.setFlag(QBluetoothDeviceInfo::Field::RSSI);
                }

                if (discoveredDevices[i].manufacturerData() != newDeviceInfo.manufacturerData()) {
                    qCDebug(QT_BT_OSX) << "Updating manufacturer data for" << newDeviceInfo.address();
                    const QVector<quint16> keys = newDeviceInfo.manufacturerIds();
                    for (auto key: keys)
                        discoveredDevices[i].setManufacturerData(key, newDeviceInfo.manufacturerData(key));
                    updatedFields.setFlag(QBluetoothDeviceInfo::Field::ManufacturerData);
                }

                if (lowEnergySearchTimeout > 0) {
                    if (discoveredDevices[i] != newDeviceInfo) {
                        discoveredDevices.replace(i, newDeviceInfo);
                        emit q_ptr->deviceDiscovered(newDeviceInfo);
                    } else {
                        if (!updatedFields.testFlag(QBluetoothDeviceInfo::Field::None))
                            emit q_ptr->deviceUpdated(discoveredDevices[i], updatedFields);
                    }

                    return;
                }

                discoveredDevices.replace(i, newDeviceInfo);
                emit q_ptr->deviceDiscovered(newDeviceInfo);

                if (!updatedFields.testFlag(QBluetoothDeviceInfo::Field::None))
                    emit q_ptr->deviceUpdated(discoveredDevices[i], updatedFields);

                return;
            }
        } else {
#ifdef Q_OS_MACOS
            if (discoveredDevices[i].address() == newDeviceInfo.address()) {
                if (discoveredDevices[i] == newDeviceInfo)
                    return;

                discoveredDevices.replace(i, newDeviceInfo);
                emit q_ptr->deviceDiscovered(newDeviceInfo);
                return;
            }
#else
            Q_UNREACHABLE();
#endif // Q_OS_MACOS
        }
    }

    discoveredDevices.append(newDeviceInfo);
    emit q_ptr->deviceDiscovered(newDeviceInfo);
}

QBluetoothDeviceDiscoveryAgent::DiscoveryMethods QBluetoothDeviceDiscoveryAgent::supportedDiscoveryMethods()
{
#ifdef Q_OS_MACOS
    return ClassicMethod | LowEnergyMethod;
#else
    return LowEnergyMethod;
#endif // Q_OS_MACOS
}

QT_END_NAMESPACE
