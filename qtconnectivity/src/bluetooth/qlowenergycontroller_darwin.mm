/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Copyright (C) 2016 Javier S. Pedro <maemo@javispedro.com>
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

#include "osx/osxbtutility_p.h"
#include "osx/uistrings_p.h"

#ifndef Q_OS_TVOS
#include "osx/osxbtperipheralmanager_p.h"
#endif // Q_OS_TVOS

#include "qlowenergycontroller_darwin_p.h"
#include "qlowenergyserviceprivate_p.h"
#include "osx/osxbtcentralmanager_p.h"

#include "qlowenergyservicedata.h"
#include "qbluetoothlocaldevice.h"
#include "qbluetoothdeviceinfo.h"
#include "qlowenergycontroller.h"
#include "qbluetoothuuid.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qsharedpointer.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qglobal.h>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

namespace {

typedef QSharedPointer<QLowEnergyServicePrivate> ServicePrivate;

// Convenience function, can return a smart pointer that 'isNull'.
ServicePrivate qt_createLEService(QLowEnergyControllerPrivateDarwin *controller, CBService *cbService, bool included)
{
    Q_ASSERT_X(controller, Q_FUNC_INFO, "invalid controller (null)");
    Q_ASSERT_X(cbService, Q_FUNC_INFO, "invalid service (nil)");

    CBUUID *const cbUuid = cbService.UUID;
    if (!cbUuid) {
        qCDebug(QT_BT_OSX) << "invalid service, UUID is nil";
        return ServicePrivate();
    }

    const QBluetoothUuid qtUuid(OSXBluetooth::qt_uuid(cbUuid));
    if (qtUuid.isNull()) // Conversion error is reported by qt_uuid.
        return ServicePrivate();

    ServicePrivate newService(new QLowEnergyServicePrivate);
    newService->uuid = qtUuid;
    newService->setController(controller);

    if (included)
        newService->type |= QLowEnergyService::IncludedService;

    // TODO: isPrimary is ... always 'NO' - to be investigated.
    /*
    if (!cbService.isPrimary) {
        // Our guess included/not was probably wrong.
        newService->type &= ~QLowEnergyService::PrimaryService;
        newService->type |= QLowEnergyService::IncludedService;
    }
    */
    return newService;
}

typedef QList<QBluetoothUuid> UUIDList;

UUIDList qt_servicesUuids(NSArray *services)
{
    QT_BT_MAC_AUTORELEASEPOOL;

    if (!services || !services.count)
        return UUIDList();

    UUIDList uuids;

    for (CBService *s in services)
        uuids.append(OSXBluetooth::qt_uuid(s.UUID));

    return uuids;
}

} // unnamed namespace

#ifndef Q_OS_TVOS
using ObjCPeripheralManager = QT_MANGLE_NAMESPACE(OSXBTPeripheralManager);
#endif // Q_OS_TVOS

using ObjCCentralManager = QT_MANGLE_NAMESPACE(OSXBTCentralManager);

QLowEnergyControllerPrivateDarwin::QLowEnergyControllerPrivateDarwin()
{
    void registerQLowEnergyControllerMetaType();
    registerQLowEnergyControllerMetaType();
    qRegisterMetaType<QLowEnergyHandle>("QLowEnergyHandle");
    qRegisterMetaType<QSharedPointer<QLowEnergyServicePrivate>>();
}

QLowEnergyControllerPrivateDarwin::~QLowEnergyControllerPrivateDarwin()
{
    if (const auto leQueue = OSXBluetooth::qt_LE_queue()) {
        if (role == QLowEnergyController::CentralRole) {
            const auto manager = centralManager.getAs<ObjCCentralManager>();
            dispatch_sync(leQueue, ^{
                [manager detach];
            });
        } else {
#ifndef Q_OS_TVOS
            const auto manager = peripheralManager.getAs<ObjCPeripheralManager>();
            dispatch_sync(leQueue, ^{
                [manager detach];
            });
#endif
        }
    }
}

bool QLowEnergyControllerPrivateDarwin::isValid() const
{
#ifdef Q_OS_TVOS
    return centralManager;
#else
    return centralManager || peripheralManager;
#endif
}

void QLowEnergyControllerPrivateDarwin::init()
{
    using OSXBluetooth::LECBManagerNotifier;

    QScopedPointer<LECBManagerNotifier> notifier(new LECBManagerNotifier);
    if (role == QLowEnergyController::PeripheralRole) {
#ifndef Q_OS_TVOS
        peripheralManager.reset([[ObjCPeripheralManager alloc] initWith:notifier.data()],
                                DarwinBluetooth::RetainPolicy::noInitialRetain);
        if (!peripheralManager) {
            qCWarning(QT_BT_OSX) << "failed to create a peripheral manager";
            return;
        }
#else
        qCWarning(QT_BT_OSX) << "the peripheral role is not supported on your platform";
        return;
#endif // Q_OS_TVOS
    } else {
        centralManager.reset([[ObjCCentralManager alloc] initWith:notifier.data()],
                             DarwinBluetooth::RetainPolicy::noInitialRetain);
        if (!centralManager) {
            qCWarning(QT_BT_OSX) << "failed to initialize a central manager";
            return;
        }
    }

    if (!connectSlots(notifier.data()))
        qCWarning(QT_BT_OSX) << "failed to connect to notifier's signal(s)";

    // Ownership was taken by central manager.
    notifier.take();
}

void QLowEnergyControllerPrivateDarwin::connectToDevice()
{
    Q_ASSERT_X(state == QLowEnergyController::UnconnectedState,
               Q_FUNC_INFO, "invalid state");

    if (!isValid()) {
        // init() had failed for was never called.
        return _q_CBManagerError(QLowEnergyController::UnknownError);
    }

    if (deviceUuid.isNull()) {
        // Wrong constructor was used or invalid UUID was provided.
        return _q_CBManagerError(QLowEnergyController::UnknownRemoteDeviceError);
    }

    // The logic enforcing the role is in the public class.
    Q_ASSERT_X(role != QLowEnergyController::PeripheralRole,
               Q_FUNC_INFO, "invalid role (peripheral)");

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    if (!leQueue) {
        qCWarning(QT_BT_OSX) << "no LE queue found";
        setErrorDescription(QLowEnergyController::UnknownError);
        return;
    }

    setErrorDescription(QLowEnergyController::NoError);
    setState(QLowEnergyController::ConnectingState);

    const QBluetoothUuid deviceUuidCopy(deviceUuid);
    ObjCCentralManager *manager = centralManager.getAs<ObjCCentralManager>();
    dispatch_async(leQueue, ^{
        [manager connectToDevice:deviceUuidCopy];
    });
}

void QLowEnergyControllerPrivateDarwin::disconnectFromDevice()
{
    if (role == QLowEnergyController::PeripheralRole) {
        // CoreBluetooth API intentionally does not provide any way of closing
        // a connection. All we can do here is to stop the advertisement.
        stopAdvertising();
        return;
    }

    if (isValid()) {
        const auto oldState = state;

        if (dispatch_queue_t leQueue = OSXBluetooth::qt_LE_queue()) {
            setState(QLowEnergyController::ClosingState);
            invalidateServices();

            auto manager = centralManager.getAs<ObjCCentralManager>();
            dispatch_async(leQueue, ^{
                [manager disconnectFromDevice];
            });

            if (oldState == QLowEnergyController::ConnectingState) {
                // With a pending connect attempt there is no
                // guarantee we'll ever have didDisconnect callback,
                // set the state here and now to make sure we still
                // can connect.
                setState(QLowEnergyController::UnconnectedState);
            }
        } else {
            qCCritical(QT_BT_OSX) << "qt LE queue is nil, "
                                     "can not dispatch 'disconnect'";
        }
    }
}

void QLowEnergyControllerPrivateDarwin::discoverServices()
{
    Q_ASSERT_X(state != QLowEnergyController::UnconnectedState,
               Q_FUNC_INFO, "not connected to peripheral");
    Q_ASSERT_X(role != QLowEnergyController::PeripheralRole,
               Q_FUNC_INFO, "invalid role (peripheral)");

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    Q_ASSERT_X(leQueue, Q_FUNC_INFO, "LE queue not found");

    setState(QLowEnergyController::DiscoveringState);

    ObjCCentralManager *manager = centralManager.getAs<ObjCCentralManager>();
    dispatch_async(leQueue, ^{
        [manager discoverServices];
    });
}

void QLowEnergyControllerPrivateDarwin::discoverServiceDetails(const QBluetoothUuid &serviceUuid)
{
    if (state != QLowEnergyController::DiscoveredState) {
        qCWarning(QT_BT_OSX) << "can not discover service details in the current state, "
                                "QLowEnergyController::DiscoveredState is expected";
        return;
    }

    if (!serviceList.contains(serviceUuid)) {
        qCWarning(QT_BT_OSX) << "unknown service: " << serviceUuid;
        return;
    }

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    Q_ASSERT(leQueue);

    ServicePrivate qtService(serviceList.value(serviceUuid));
    qtService->setState(QLowEnergyService::DiscoveringServices);
    // Copy objects ...
    ObjCCentralManager *manager = centralManager.getAs<ObjCCentralManager>();
    const QBluetoothUuid serviceUuidCopy(serviceUuid);
    dispatch_async(leQueue, ^{
        [manager discoverServiceDetails:serviceUuidCopy];
    });
}

void QLowEnergyControllerPrivateDarwin::requestConnectionUpdate(const QLowEnergyConnectionParameters &params)
{
    Q_UNUSED(params);
    // TODO: implement this, if possible.
    qCWarning(QT_BT_OSX) << "Connection update not implemented on your platform";
}

void QLowEnergyControllerPrivateDarwin::addToGenericAttributeList(const QLowEnergyServiceData &service,
                                                                  QLowEnergyHandle startHandle)
{
    Q_UNUSED(service);
    Q_UNUSED(startHandle);
    // TODO: check why I don't need this (apparently it is used in addServiceHelper
    // of the base class).
}

QLowEnergyService * QLowEnergyControllerPrivateDarwin::addServiceHelper(const QLowEnergyServiceData &service)
{
    // Three checks below should be removed, they are done in the q_ptr's class.
#ifdef Q_OS_TVOS
    Q_UNUSED(service);
    qCDebug(QT_BT_OSX, "peripheral role is not supported on tvOS");
#else
    if (role != QLowEnergyController::PeripheralRole) {
        qCWarning(QT_BT_OSX) << "not in peripheral role";
        return nullptr;
    }

    if (state != QLowEnergyController::UnconnectedState) {
        qCWarning(QT_BT_OSX) << "invalid state";
        return nullptr;
    }

    if (!service.isValid()) {
        qCWarning(QT_BT_OSX) << "invalid service";
        return nullptr;
    }

    for (auto includedService : service.includedServices())
        includedService->d_ptr->type |= QLowEnergyService::IncludedService;

    const auto manager = peripheralManager.getAs<ObjCPeripheralManager>();
    Q_ASSERT(manager);
    if (const auto servicePrivate = [manager addService:service]) {
        servicePrivate->setController(this);
        servicePrivate->state = QLowEnergyService::LocalService;
        localServices.insert(servicePrivate->uuid, servicePrivate);
        return new QLowEnergyService(servicePrivate);
    }
#endif // Q_OS_TVOS
    return nullptr;
}

void QLowEnergyControllerPrivateDarwin::_q_connected()
{
    setState(QLowEnergyController::ConnectedState);
    emit q_ptr->connected();
}

void QLowEnergyControllerPrivateDarwin::_q_disconnected()
{
    if (role == QLowEnergyController::CentralRole)
        invalidateServices();

    setState(QLowEnergyController::UnconnectedState);
    emit q_ptr->disconnected();
}

void QLowEnergyControllerPrivateDarwin::_q_serviceDiscoveryFinished()
{
    Q_ASSERT_X(state == QLowEnergyController::DiscoveringState,
               Q_FUNC_INFO, "invalid state");

    using namespace OSXBluetooth;

    QT_BT_MAC_AUTORELEASEPOOL;

    NSArray *const services = [centralManager.getAs<ObjCCentralManager>() peripheral].services;
    // Now we have to traverse the discovered services tree.
    // Essentially it's an iterative version of more complicated code from the
    // OSXBTCentralManager's code.
    // All Obj-C entities either auto-release, or guarded by ObjCScopedReferences.
    if (services && [services count]) {
        QMap<QBluetoothUuid, CBService *> discoveredCBServices;
        //1. The first pass - none of this services is 'included' yet (we'll discover 'included'
        //   during the pass 2); we also ignore duplicates (== services with the same UUID)
        // - since we do not have a way to distinguish them later
        //   (our API is using uuids when creating QLowEnergyServices).
        for (CBService *cbService in services) {
            const ServicePrivate newService(qt_createLEService(this, cbService, false));
            if (!newService.data())
                continue;
            if (serviceList.contains(newService->uuid)) {
                // It's a bit stupid we first created it ...
                qCDebug(QT_BT_OSX) << "discovered service with a duplicated UUID"
                                   << newService->uuid;
                continue;
            }
            serviceList.insert(newService->uuid, newService);
            discoveredCBServices.insert(newService->uuid, cbService);
        }

        ObjCStrongReference<NSMutableArray> toVisit([[NSMutableArray alloc] initWithArray:services], false);
        ObjCStrongReference<NSMutableArray> toVisitNext([[NSMutableArray alloc] init], false);
        ObjCStrongReference<NSMutableSet> visited([[NSMutableSet alloc] init], false);

        while (true) {
            for (NSUInteger i = 0, e = [toVisit count]; i < e; ++i) {
                CBService *const s = [toVisit objectAtIndex:i];
                if (![visited containsObject:s]) {
                    [visited addObject:s];
                    if (s.includedServices && s.includedServices.count)
                        [toVisitNext addObjectsFromArray:s.includedServices];
                }

                const QBluetoothUuid uuid(qt_uuid(s.UUID));
                if (serviceList.contains(uuid) && discoveredCBServices.value(uuid) == s) {
                    ServicePrivate qtService(serviceList.value(uuid));
                    // Add included UUIDs:
                    qtService->includedServices.append(qt_servicesUuids(s.includedServices));
                }// Else - we ignored this CBService object.
            }

            if (![toVisitNext count])
                break;

            for (NSUInteger i = 0, e = [toVisitNext count]; i < e; ++i) {
                CBService *const s = [toVisitNext objectAtIndex:i];
                const QBluetoothUuid uuid(qt_uuid(s.UUID));
                if (serviceList.contains(uuid)) {
                    if (discoveredCBServices.value(uuid) == s) {
                        ServicePrivate qtService(serviceList.value(uuid));
                        qtService->type |= QLowEnergyService::IncludedService;
                    } // Else this is the duplicate we ignored already.
                } else {
                    // Oh, we do not even have it yet???
                    ServicePrivate newService(qt_createLEService(this, s, true));
                    serviceList.insert(newService->uuid, newService);
                    discoveredCBServices.insert(newService->uuid, s);
                }
            }

            toVisit.resetWithoutRetain(toVisitNext.take());
            toVisitNext.resetWithoutRetain([[NSMutableArray alloc] init]);
        }
    } else {
        qCDebug(QT_BT_OSX) << "no services found";
    }


    state = QLowEnergyController::DiscoveredState;

    for (auto it = serviceList.constBegin(); it != serviceList.constEnd(); ++it) {
        QMetaObject::invokeMethod(q_ptr, "serviceDiscovered", Qt::QueuedConnection,
                                  Q_ARG(QBluetoothUuid, it.key()));
    }

    QMetaObject::invokeMethod(q_ptr, "stateChanged", Qt::QueuedConnection, Q_ARG(QLowEnergyController::ControllerState, state));
    QMetaObject::invokeMethod(q_ptr, "discoveryFinished", Qt::QueuedConnection);
}

void QLowEnergyControllerPrivateDarwin::_q_serviceDetailsDiscoveryFinished(QSharedPointer<QLowEnergyServicePrivate> service)
{
    QT_BT_MAC_AUTORELEASEPOOL;

    Q_ASSERT(service);

    if (!serviceList.contains(service->uuid)) {
        qCDebug(QT_BT_OSX) << "unknown service uuid:"
                           << service->uuid;
        return;
    }

    ServicePrivate qtService(serviceList.value(service->uuid));
    // Assert on handles?
    qtService->startHandle = service->startHandle;
    qtService->endHandle = service->endHandle;
    qtService->characteristicList = service->characteristicList;

    qtService->setState(QLowEnergyService::ServiceDiscovered);
}

void QLowEnergyControllerPrivateDarwin::_q_servicesWereModified()
{
    if (!(state == QLowEnergyController::DiscoveringState
          || state == QLowEnergyController::DiscoveredState)) {
        qCWarning(QT_BT_OSX) << "services were modified while controller is not in Discovered/Discovering state";
        return;
    }

    if (state == QLowEnergyController::DiscoveredState)
        invalidateServices();

    setState(QLowEnergyController::ConnectedState);
    q_ptr->discoverServices();
}

void QLowEnergyControllerPrivateDarwin::_q_characteristicRead(QLowEnergyHandle charHandle,
                                                              const QByteArray &value)
{
    Q_ASSERT_X(charHandle, Q_FUNC_INFO, "invalid characteristic handle(0)");

    ServicePrivate service(serviceForHandle(charHandle));
    if (service.isNull())
        return;

    QLowEnergyCharacteristic characteristic(characteristicForHandle(charHandle));
    if (!characteristic.isValid()) {
        qCWarning(QT_BT_OSX) << "unknown characteristic";
        return;
    }

    if (characteristic.properties() & QLowEnergyCharacteristic::Read)
        updateValueOfCharacteristic(charHandle, value, false);

    emit service->characteristicRead(characteristic, value);
}

void QLowEnergyControllerPrivateDarwin::_q_characteristicWritten(QLowEnergyHandle charHandle,
                                                                 const QByteArray &value)
{
    Q_ASSERT_X(charHandle, Q_FUNC_INFO, "invalid characteristic handle(0)");

    ServicePrivate service(serviceForHandle(charHandle));
    if (service.isNull()) {
        qCWarning(QT_BT_OSX) << "can not find service for characteristic handle"
                             << charHandle;
        return;
    }

    QLowEnergyCharacteristic characteristic(characteristicForHandle(charHandle));
    if (!characteristic.isValid()) {
        qCWarning(QT_BT_OSX) << "unknown characteristic";
        return;
    }

    if (characteristic.properties() & QLowEnergyCharacteristic::Read)
        updateValueOfCharacteristic(charHandle, value, false);

    emit service->characteristicWritten(characteristic, value);
}

void QLowEnergyControllerPrivateDarwin::_q_characteristicUpdated(QLowEnergyHandle charHandle,
                                                                 const QByteArray &value)
{
    // TODO: write/update notifications are quite similar (except asserts/warnings messages
    // and different signals emitted). Merge them into one function?
    Q_ASSERT_X(charHandle, Q_FUNC_INFO, "invalid characteristic handle(0)");

    ServicePrivate service(serviceForHandle(charHandle));
    if (service.isNull()) {
        // This can be an error (no characteristic found for this handle),
        // it can also be that we set notify value before the service
        // was reported (serviceDetailsDiscoveryFinished) - this happens,
        // if we read a descriptor (characteristic client configuration),
        // and it's (pre)set.
        return;
    }

    QLowEnergyCharacteristic characteristic(characteristicForHandle(charHandle));
    if (!characteristic.isValid()) {
        qCWarning(QT_BT_OSX) << "unknown characteristic";
        return;
    }

    if (characteristic.properties() & QLowEnergyCharacteristic::Read)
        updateValueOfCharacteristic(charHandle, value, false);

    emit service->characteristicChanged(characteristic, value);
}

void QLowEnergyControllerPrivateDarwin::_q_descriptorRead(QLowEnergyHandle dHandle,
                                                          const QByteArray &value)
{
    Q_ASSERT_X(dHandle, Q_FUNC_INFO, "invalid descriptor handle (0)");

    const QLowEnergyDescriptor qtDescriptor(descriptorForHandle(dHandle));
    if (!qtDescriptor.isValid()) {
        qCWarning(QT_BT_OSX) << "unknown descriptor" << dHandle;
        return;
    }

    ServicePrivate service(serviceForHandle(qtDescriptor.characteristicHandle()));
    updateValueOfDescriptor(qtDescriptor.characteristicHandle(), dHandle, value, false);
    emit service->descriptorRead(qtDescriptor, value);
}

void QLowEnergyControllerPrivateDarwin::_q_descriptorWritten(QLowEnergyHandle dHandle,
                                                             const QByteArray &value)
{
    Q_ASSERT_X(dHandle, Q_FUNC_INFO, "invalid descriptor handle (0)");

    const QLowEnergyDescriptor qtDescriptor(descriptorForHandle(dHandle));
    if (!qtDescriptor.isValid()) {
        qCWarning(QT_BT_OSX) << "unknown descriptor" << dHandle;
        return;
    }

    ServicePrivate service(serviceForHandle(qtDescriptor.characteristicHandle()));
    // TODO: test if this data is what we expected.
    updateValueOfDescriptor(qtDescriptor.characteristicHandle(), dHandle, value, false);
    emit service->descriptorWritten(qtDescriptor, value);
}

void QLowEnergyControllerPrivateDarwin::_q_notificationEnabled(QLowEnergyHandle charHandle,
                                                               bool enabled)
{
    // CoreBluetooth in peripheral role does not allow mutable descriptors,
    // in central we can only call setNotification:enabled/disabled.
    // But from Qt API's point of view, a central has to write into
    // client characteristic configuration descriptor. So here we emulate
    // such a write (we cannot say if it's a notification or indication and
    // report as both).

    Q_ASSERT_X(role == QLowEnergyController::PeripheralRole, Q_FUNC_INFO,
               "controller has an invalid role, 'peripheral' expected");
    Q_ASSERT_X(charHandle, Q_FUNC_INFO, "invalid characteristic handle (0)");

    const QLowEnergyCharacteristic qtChar(characteristicForHandle(charHandle));
    if (!qtChar.isValid()) {
        qCWarning(QT_BT_OSX) << "unknown characteristic" << charHandle;
        return;
    }

    const QLowEnergyDescriptor qtDescriptor =
        qtChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
    if (!qtDescriptor.isValid()) {
        qCWarning(QT_BT_OSX) << "characteristic" << charHandle
                             << "does not have a client characteristic "
                                "descriptor";
        return;
    }

    ServicePrivate service(serviceForHandle(charHandle));
    if (service.data()) {
        // It's a 16-bit value, the least significant bit is for notifications,
        // the next one - for indications (thus 1 means notifications enabled,
        // 2 - indications enabled).
        // 3 is the maximum value and it means both enabled.
        QByteArray value(2, 0);
        if (enabled)
            value[0] = 3;
        updateValueOfDescriptor(charHandle, qtDescriptor.handle(), value, false);
        emit service->descriptorWritten(qtDescriptor, value);
    }
}

void QLowEnergyControllerPrivateDarwin::_q_LEnotSupported()
{
    // Report as an error. But this should not be possible
    // actually: before connecting to any device, we have
    // to discover it, if it was discovered ... LE _must_
    // be supported.
}

void QLowEnergyControllerPrivateDarwin::_q_CBManagerError(QLowEnergyController::Error errorCode)
{
    // This function handles errors reported while connecting to a remote device
    // and also other errors in general.
    setError(errorCode);

    if (state == QLowEnergyController::ConnectingState)
        setState(QLowEnergyController::UnconnectedState);
    else if (state == QLowEnergyController::DiscoveringState)
        setState(QLowEnergyController::ConnectedState);

    // In any other case we stay in Discovered, it's
    // a service/characteristic - related error.
}

void QLowEnergyControllerPrivateDarwin::_q_CBManagerError(const QBluetoothUuid &serviceUuid,
                                                          QLowEnergyController::Error errorCode)
{
    // Errors reported while discovering service details etc.
    Q_UNUSED(errorCode) // TODO: setError?

    // We failed to discover any characteristics/descriptors.
    if (serviceList.contains(serviceUuid)) {
        ServicePrivate qtService(serviceList.value(serviceUuid));
        qtService->setState(QLowEnergyService::InvalidService);
    } else {
        qCDebug(QT_BT_OSX) << "error reported for unknown service"
                           << serviceUuid;
    }
}

void QLowEnergyControllerPrivateDarwin::_q_CBManagerError(const QBluetoothUuid &serviceUuid,
                                                          QLowEnergyService::ServiceError errorCode)
{
    if (!serviceList.contains(serviceUuid)) {
        qCDebug(QT_BT_OSX) << "unknown service uuid:"
                           << serviceUuid;
        return;
    }

    ServicePrivate service(serviceList.value(serviceUuid));
    service->setError(errorCode);
}

void QLowEnergyControllerPrivateDarwin::setNotifyValue(QSharedPointer<QLowEnergyServicePrivate> service,
                                                       QLowEnergyHandle charHandle,
                                                       const QByteArray &newValue)
{
    Q_ASSERT_X(!service.isNull(), Q_FUNC_INFO, "invalid service (null)");

    if (role == QLowEnergyController::PeripheralRole) {
        qCWarning(QT_BT_OSX) << "invalid role (peripheral)";
        service->setError(QLowEnergyService::DescriptorWriteError);
        return;
    }

    if (newValue.size() > 2) {
        // Qt's API requires an error on such write.
        // With Core Bluetooth we do not write any descriptor,
        // but instead call a special method. So it's better to
        // intercept wrong data size here:
        qCWarning(QT_BT_OSX) << "client characteristic configuration descriptor"
                                "is 2 bytes, but value size is: " << newValue.size();
        service->setError(QLowEnergyService::DescriptorWriteError);
        return;
    }

    if (!serviceList.contains(service->uuid)) {
        qCWarning(QT_BT_OSX) << "no service with uuid:" << service->uuid << "found";
        return;
    }

    if (!service->characteristicList.contains(charHandle)) {
        qCDebug(QT_BT_OSX) << "no characteristic with handle:"
                           << charHandle << "found";
        return;
    }

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    Q_ASSERT_X(leQueue, Q_FUNC_INFO, "no LE queue found");

    ObjCCentralManager *manager = centralManager.getAs<ObjCCentralManager>();
    const QBluetoothUuid serviceUuid(service->uuid);
    const QByteArray newValueCopy(newValue);
    dispatch_async(leQueue, ^{
        [manager setNotifyValue:newValueCopy
                 forCharacteristic:charHandle
                 onService:serviceUuid];
    });
}

void QLowEnergyControllerPrivateDarwin::readCharacteristic(const QSharedPointer<QLowEnergyServicePrivate> service,
                                                           const QLowEnergyHandle charHandle)
{
    Q_ASSERT_X(!service.isNull(), Q_FUNC_INFO, "invalid service (null)");

    if (role == QLowEnergyController::PeripheralRole) {
        qCWarning(QT_BT_OSX) << "invalid role (peripheral)";
        return;
    }

    if (!serviceList.contains(service->uuid)) {
        qCWarning(QT_BT_OSX) << "no service with uuid:"
                             << service->uuid << "found";
        return;
    }

    if (!service->characteristicList.contains(charHandle)) {
        qCDebug(QT_BT_OSX) << "no characteristic with handle:"
                           << charHandle << "found";
        return;
    }

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    Q_ASSERT_X(leQueue, Q_FUNC_INFO, "no LE queue found");

    // Attention! We have to copy UUID.
    ObjCCentralManager *manager = centralManager.getAs<ObjCCentralManager>();
    const QBluetoothUuid serviceUuid(service->uuid);
    dispatch_async(leQueue, ^{
        [manager readCharacteristic:charHandle onService:serviceUuid];
    });
}

void QLowEnergyControllerPrivateDarwin::writeCharacteristic(const QSharedPointer<QLowEnergyServicePrivate> service,
                                                            const QLowEnergyHandle charHandle, const QByteArray &newValue,
                                                            QLowEnergyService::WriteMode mode)
{
    Q_ASSERT_X(!service.isNull(), Q_FUNC_INFO, "invalid service (null)");

    // We can work only with services found on a given peripheral
    // (== created by the given LE controller).

    if (!serviceList.contains(service->uuid) && !localServices.contains(service->uuid)) {
        qCWarning(QT_BT_OSX) << "no service with uuid:"
                             << service->uuid << " found";
        return;
    }

    if (!service->characteristicList.contains(charHandle)) {
        qCDebug(QT_BT_OSX) << "no characteristic with handle:"
                           << charHandle << " found";
        return;
    }

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    Q_ASSERT_X(leQueue, Q_FUNC_INFO, "no LE queue found");
    // Attention! We have to copy objects!
    const QByteArray newValueCopy(newValue);
    if (role == QLowEnergyController::CentralRole) {
        const QBluetoothUuid serviceUuid(service->uuid);
        const auto manager = centralManager.getAs<ObjCCentralManager>();
        dispatch_async(leQueue, ^{
            [manager write:newValueCopy
                charHandle:charHandle
                 onService:serviceUuid
                 withResponse:mode == QLowEnergyService::WriteWithResponse];
        });
    } else {
#ifndef Q_OS_TVOS
        const auto manager = peripheralManager.getAs<ObjCPeripheralManager>();
        dispatch_async(leQueue, ^{
            [manager write:newValueCopy charHandle:charHandle];
        });
#else
        qCWarning(QT_BT_OSX) << "peripheral role is not supported on your platform";
#endif
    }
}

quint16 QLowEnergyControllerPrivateDarwin::updateValueOfCharacteristic(QLowEnergyHandle charHandle,
                                                                       const QByteArray &value,
                                                                       bool appendValue)
{
    QSharedPointer<QLowEnergyServicePrivate> service = serviceForHandle(charHandle);
    if (!service.isNull()) {
        CharacteristicDataMap::iterator charIt = service->characteristicList.find(charHandle);
        if (charIt != service->characteristicList.end()) {
            QLowEnergyServicePrivate::CharData &charData = charIt.value();
            if (appendValue)
                charData.value += value;
            else
                charData.value = value;

            return charData.value.size();
        }
    }

    return 0;
}

void QLowEnergyControllerPrivateDarwin::readDescriptor(const QSharedPointer<QLowEnergyServicePrivate> service,
                                                       const QLowEnergyHandle charHandle,
                                                       const QLowEnergyHandle descriptorHandle)
{
    Q_UNUSED(charHandle) // Hehe, yes!

    Q_ASSERT_X(!service.isNull(), Q_FUNC_INFO, "invalid service (null)");

    if (role == QLowEnergyController::PeripheralRole) {
        qCWarning(QT_BT_OSX) << "invalid role (peripheral)";
        return;
    }

    if (!serviceList.contains(service->uuid)) {
        qCWarning(QT_BT_OSX) << "no service with uuid:"
                             << service->uuid << "found";
        return;
    }

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    if (!leQueue) {
        qCWarning(QT_BT_OSX) << "no LE queue found";
        return;
    }
    // Attention! Copy objects!
    const QBluetoothUuid serviceUuid(service->uuid);
    ObjCCentralManager * const manager = centralManager.getAs<ObjCCentralManager>();
    dispatch_async(leQueue, ^{
        [manager readDescriptor:descriptorHandle
                 onService:serviceUuid];
    });
}

void QLowEnergyControllerPrivateDarwin::writeDescriptor(const QSharedPointer<QLowEnergyServicePrivate> service,
                                                        const QLowEnergyHandle charHandle,
                                                        const QLowEnergyHandle descriptorHandle,
                                                        const QByteArray &newValue)
{
    Q_UNUSED(charHandle)

    Q_ASSERT_X(!service.isNull(), Q_FUNC_INFO, "invalid service (null)");

    if (role == QLowEnergyController::PeripheralRole) {
        qCWarning(QT_BT_OSX) << "invalid role (peripheral)";
        return;
    }

    // We can work only with services found on a given peripheral
    // (== created by the given LE controller),
    // otherwise we can not write anything at all.
    if (!serviceList.contains(service->uuid)) {
        qCWarning(QT_BT_OSX) << "no service with uuid:"
                             << service->uuid << " found";
        return;
    }

    dispatch_queue_t leQueue(OSXBluetooth::qt_LE_queue());
    Q_ASSERT_X(leQueue, Q_FUNC_INFO, "no LE queue found");
    // Attention! Copy objects!
    const QBluetoothUuid serviceUuid(service->uuid);
    ObjCCentralManager * const manager = centralManager.getAs<ObjCCentralManager>();
    const QByteArray newValueCopy(newValue);
    dispatch_async(leQueue, ^{
        [manager write:newValueCopy
                 descHandle:descriptorHandle
                 onService:serviceUuid];
    });
}

quint16 QLowEnergyControllerPrivateDarwin::updateValueOfDescriptor(QLowEnergyHandle charHandle, QLowEnergyHandle descHandle,
                                                                   const QByteArray &value, bool appendValue)
{
    QSharedPointer<QLowEnergyServicePrivate> service = serviceForHandle(charHandle);
    if (!service.isNull()) {
        CharacteristicDataMap::iterator charIt = service->characteristicList.find(charHandle);
        if (charIt != service->characteristicList.end()) {
            QLowEnergyServicePrivate::CharData &charData = charIt.value();

            DescriptorDataMap::iterator descIt = charData.descriptorList.find(descHandle);
            if (descIt != charData.descriptorList.end()) {
                QLowEnergyServicePrivate::DescData &descDetails = descIt.value();

                if (appendValue)
                    descDetails.value += value;
                else
                    descDetails.value = value;

                return descDetails.value.size();
            }
        }
    }

    return 0;
}

void QLowEnergyControllerPrivateDarwin::setErrorDescription(QLowEnergyController::Error errorCode)
{
    // This function does not emit!
    // TODO: well, it is not a reason to duplicate a significant part of
    // setError though!

    error = errorCode;

    switch (error) {
    case QLowEnergyController::NoError:
        errorString.clear();
        break;
    case QLowEnergyController::UnknownRemoteDeviceError:
        errorString = QLowEnergyController::tr("Remote device cannot be found");
        break;
    case QLowEnergyController::InvalidBluetoothAdapterError:
        errorString = QLowEnergyController::tr("Cannot find local adapter");
        break;
    case QLowEnergyController::NetworkError:
        errorString = QLowEnergyController::tr("Error occurred during connection I/O");
        break;
    case QLowEnergyController::ConnectionError:
        errorString = QLowEnergyController::tr("Error occurred trying to connect to remote device.");
        break;
    case QLowEnergyController::AdvertisingError:
        errorString = QLowEnergyController::tr("Error occurred trying to start advertising");
        break;
    case QLowEnergyController::UnknownError:
    default:
        errorString = QLowEnergyController::tr("Unknown Error");
        break;
    }
}

bool QLowEnergyControllerPrivateDarwin::connectSlots(OSXBluetooth::LECBManagerNotifier *notifier)
{
    using OSXBluetooth::LECBManagerNotifier;

    Q_ASSERT_X(notifier, Q_FUNC_INFO, "invalid notifier object (null)");

    bool ok = connect(notifier, &LECBManagerNotifier::connected,
                      this, &QLowEnergyControllerPrivateDarwin::_q_connected);
    ok = ok && connect(notifier, &LECBManagerNotifier::disconnected,
                       this, &QLowEnergyControllerPrivateDarwin::_q_disconnected);
    ok = ok && connect(notifier, &LECBManagerNotifier::serviceDiscoveryFinished,
                       this, &QLowEnergyControllerPrivateDarwin::_q_serviceDiscoveryFinished);
    ok = ok && connect(notifier, &LECBManagerNotifier::servicesWereModified,
                       this, &QLowEnergyControllerPrivateDarwin::_q_servicesWereModified);
    ok = ok && connect(notifier, &LECBManagerNotifier::serviceDetailsDiscoveryFinished,
                       this, &QLowEnergyControllerPrivateDarwin::_q_serviceDetailsDiscoveryFinished);
    ok = ok && connect(notifier, &LECBManagerNotifier::characteristicRead,
                       this, &QLowEnergyControllerPrivateDarwin::_q_characteristicRead);
    ok = ok && connect(notifier, &LECBManagerNotifier::characteristicWritten,
                       this, &QLowEnergyControllerPrivateDarwin::_q_characteristicWritten);
    ok = ok && connect(notifier, &LECBManagerNotifier::characteristicUpdated,
                       this, &QLowEnergyControllerPrivateDarwin::_q_characteristicUpdated);
    ok = ok && connect(notifier, &LECBManagerNotifier::descriptorRead,
                       this, &QLowEnergyControllerPrivateDarwin::_q_descriptorRead);
    ok = ok && connect(notifier, &LECBManagerNotifier::descriptorWritten,
                       this, &QLowEnergyControllerPrivateDarwin::_q_descriptorWritten);
    ok = ok && connect(notifier, &LECBManagerNotifier::notificationEnabled,
                       this, &QLowEnergyControllerPrivateDarwin::_q_notificationEnabled);
    ok = ok && connect(notifier, &LECBManagerNotifier::LEnotSupported,
                       this, &QLowEnergyControllerPrivateDarwin::_q_LEnotSupported);
    ok = ok && connect(notifier, SIGNAL(CBManagerError(QLowEnergyController::Error)),
                       this, SLOT(_q_CBManagerError(QLowEnergyController::Error)));
    ok = ok && connect(notifier, SIGNAL(CBManagerError(const QBluetoothUuid &, QLowEnergyController::Error)),
                       this, SLOT(_q_CBManagerError(const QBluetoothUuid &, QLowEnergyController::Error)));
    ok = ok && connect(notifier, SIGNAL(CBManagerError(const QBluetoothUuid &, QLowEnergyService::ServiceError)),
                       this, SLOT(_q_CBManagerError(const QBluetoothUuid &, QLowEnergyService::ServiceError)));

    if (!ok)
        notifier->disconnect();

    return ok;
}

void QLowEnergyControllerPrivateDarwin::startAdvertising(const QLowEnergyAdvertisingParameters &params,
                                                         const QLowEnergyAdvertisingData &advertisingData,
                                                         const QLowEnergyAdvertisingData &scanResponseData)
{
#ifdef Q_OS_TVOS
    Q_UNUSED(params)
    Q_UNUSED(advertisingData)
    Q_UNUSED(scanResponseData)
    qCWarning(QT_BT_OSX) << "advertising is not supported on your platform";
#else

    if (!isValid())
        return _q_CBManagerError(QLowEnergyController::UnknownError);

    if (role != QLowEnergyController::PeripheralRole) {
        qCWarning(QT_BT_OSX) << "controller is not a peripheral, cannot start advertising";
        return;
    }

    if (state != QLowEnergyController::UnconnectedState) {
        qCWarning(QT_BT_OSX) << "invalid state" << state;
        return;
    }

    auto leQueue(OSXBluetooth::qt_LE_queue());
    if (!leQueue) {
        qCWarning(QT_BT_OSX) << "no LE queue found";
        setErrorDescription(QLowEnergyController::UnknownError);
        return;
    }

    const auto manager = peripheralManager.getAs<ObjCPeripheralManager>();
    [manager setParameters:params data:advertisingData scanResponse:scanResponseData];

    setState(QLowEnergyController::AdvertisingState);

    dispatch_async(leQueue, ^{
        [manager startAdvertising];
    });
#endif
}

void QLowEnergyControllerPrivateDarwin::stopAdvertising()
{
#ifdef Q_OS_TVOS
    qCWarning(QT_BT_OSX) << "advertising is not supported on your platform";
#else
    if (!isValid())
        return _q_CBManagerError(QLowEnergyController::UnknownError);

    if (state != QLowEnergyController::AdvertisingState) {
        qCDebug(QT_BT_OSX) << "cannot stop advertising, called in state" << state;
        return;
    }

    if (const auto leQueue = OSXBluetooth::qt_LE_queue()) {
        const auto manager = peripheralManager.getAs<ObjCPeripheralManager>();
        dispatch_sync(leQueue, ^{
            [manager stopAdvertising];
        });

        setState(QLowEnergyController::UnconnectedState);
    } else {
        qCWarning(QT_BT_OSX) << "no LE queue found";
        setErrorDescription(QLowEnergyController::UnknownError);
        return;
    }
#endif
}

QT_END_NAMESPACE

