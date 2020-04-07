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

#include "osx/osxbtsocketlistener_p.h"
#include "qbluetoothserver_p.h"

// The order is important: a workround for
// a private header included by private header
// (incorrectly handled dependencies).
#include "qbluetoothsocketbase_p.h"
#include "qbluetoothsocket_osx_p.h"

#include "qbluetoothlocaldevice.h"
#include "osx/osxbtutility_p.h"
#include "osx/osxbluetooth_p.h"
#include "qbluetoothserver.h"
#include "qbluetoothsocket.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qscopedpointer.h>
#include <QtCore/qvariant.h>
#include <QtCore/qglobal.h>
#include <QtCore/qmutex.h>

#include <Foundation/Foundation.h>

#include <limits>

QT_BEGIN_NAMESPACE

namespace {

using DarwinBluetooth::RetainPolicy;
using ServiceInfo = QBluetoothServiceInfo;
using ObjCListener = QT_MANGLE_NAMESPACE(OSXBTSocketListener);

QMap<quint16, QBluetoothServerPrivate *> &busyPSMs()
{
    static QMap<quint16, QBluetoothServerPrivate *> psms;
    return psms;
}

QMap<quint16, QBluetoothServerPrivate *> &busyChannels()
{
    static QMap<quint16, QBluetoothServerPrivate *> channels;
    return channels;
}

typedef QMap<quint16, QBluetoothServerPrivate *>::iterator ServerMapIterator;

}


QBluetoothServerPrivate::QBluetoothServerPrivate(ServiceInfo::Protocol type,
                                                 QBluetoothServer *parent)
                            : socket(nullptr),
                              maxPendingConnections(1),
                              securityFlags(QBluetooth::NoSecurity),
                              serverType(type),
                              q_ptr(parent),
                              m_lastError(QBluetoothServer::NoError),
                              port(0)
{
    if (serverType == ServiceInfo::UnknownProtocol)
        qCWarning(QT_BT_OSX) << "unknown protocol";
}

QBluetoothServerPrivate::~QBluetoothServerPrivate()
{
    const QMutexLocker lock(&channelMapMutex());
    unregisterServer(this);
}

bool QBluetoothServerPrivate::startListener(quint16 realPort)
{
    Q_ASSERT_X(realPort, Q_FUNC_INFO, "invalid port");

    if (serverType == ServiceInfo::UnknownProtocol) {
        qCWarning(QT_BT_OSX) << "invalid protocol";
        return false;
    }

    if (!listener) {
        listener.reset([[ObjCListener alloc] initWithListener:this],
                       RetainPolicy::noInitialRetain);
    }

    bool result = false;
    if (serverType == ServiceInfo::RfcommProtocol)
        result = [listener.getAs<ObjCListener>() listenRFCOMMConnectionsWithChannelID:realPort];
    else
        result = [listener.getAs<ObjCListener>() listenL2CAPConnectionsWithPSM:realPort];

    if (!result)
        listener.reset();

    return result;
}

bool QBluetoothServerPrivate::isListening() const
{
    if (serverType == ServiceInfo::UnknownProtocol)
        return false;

    const QMutexLocker lock(&QBluetoothServerPrivate::channelMapMutex());
    return QBluetoothServerPrivate::registeredServer(q_ptr->serverPort(), serverType);
}

void QBluetoothServerPrivate::stopListener()
{
    listener.reset();
}

void QBluetoothServerPrivate::openNotifyRFCOMM(void *generic)
{
    auto channel = static_cast<IOBluetoothRFCOMMChannel *>(generic);

    Q_ASSERT_X(listener, Q_FUNC_INFO, "invalid listener (nil)");
    Q_ASSERT_X(channel, Q_FUNC_INFO, "invalid channel (nil)");
    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    PendingConnection newConnection(channel, RetainPolicy::doInitialRetain);
    pendingConnections.append(newConnection);

    emit q_ptr->newConnection();
}

void QBluetoothServerPrivate::openNotifyL2CAP(void *generic)
{
    auto channel = static_cast<IOBluetoothL2CAPChannel *>(generic);

    Q_ASSERT_X(listener, Q_FUNC_INFO, "invalid listener (nil)");
    Q_ASSERT_X(channel, Q_FUNC_INFO, "invalid channel (nil)");
    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    PendingConnection newConnection(channel, RetainPolicy::doInitialRetain);
    pendingConnections.append(newConnection);

    emit q_ptr->newConnection();
}

QMutex &QBluetoothServerPrivate::channelMapMutex()
{
    static QMutex mutex;
    return mutex;
}

bool QBluetoothServerPrivate::channelIsBusy(quint16 channelID)
{
    // External lock is required.
    return busyChannels().contains(channelID);
}

quint16 QBluetoothServerPrivate::findFreeChannel()
{
    // External lock is required.
    for (quint16 i = 1; i <= 30; ++i) {
        if (!busyChannels().contains(i))
            return i;
    }

    return 0; //Invalid port.
}

bool QBluetoothServerPrivate::psmIsBusy(quint16 psm)
{
    // External lock is required.
    return busyPSMs().contains(psm);
}

quint16 QBluetoothServerPrivate::findFreePSM()
{
    // External lock is required.
    for (quint16 i = 1, e = std::numeric_limits<qint16>::max(); i < e; i += 2) {
        if (!psmIsBusy(i))
            return i;
    }

    return 0; // Invalid PSM.
}

void QBluetoothServerPrivate::registerServer(QBluetoothServerPrivate *server, quint16 port)
{
    // External lock is required + port must be free.
    Q_ASSERT_X(server, Q_FUNC_INFO, "invalid server (null)");

    const ServiceInfo::Protocol type = server->serverType;
    if (type == ServiceInfo::RfcommProtocol) {
        Q_ASSERT_X(!channelIsBusy(port), Q_FUNC_INFO, "port is busy");
        busyChannels()[port] = server;
    } else if (type == ServiceInfo::L2capProtocol) {
        Q_ASSERT_X(!psmIsBusy(port), Q_FUNC_INFO, "port is busy");
        busyPSMs()[port] = server;
    } else {
        qCWarning(QT_BT_OSX) << "can not register a server "
                                "with unknown protocol type";
    }
}

QBluetoothServerPrivate *QBluetoothServerPrivate::registeredServer(quint16 port, QBluetoothServiceInfo::Protocol protocol)
{
    // Eternal lock is required.
    if (protocol == ServiceInfo::RfcommProtocol) {
        ServerMapIterator it = busyChannels().find(port);
        if (it != busyChannels().end())
            return it.value();
    } else if (protocol == ServiceInfo::L2capProtocol) {
        ServerMapIterator it = busyPSMs().find(port);
        if (it != busyPSMs().end())
            return it.value();
    } else {
        qCWarning(QT_BT_OSX) << "invalid protocol";
    }

    return nullptr;
}

void QBluetoothServerPrivate::unregisterServer(QBluetoothServerPrivate *server)
{
    // External lock is required.
    const ServiceInfo::Protocol type = server->serverType;
    const quint16 port = server->port;

    if (type == ServiceInfo::RfcommProtocol) {
        ServerMapIterator it = busyChannels().find(port);
        if (it != busyChannels().end()) {
            busyChannels().erase(it);
        } else {
            qCWarning(QT_BT_OSX) << "server is not registered";
        }
    } else if (type == ServiceInfo::L2capProtocol) {
        ServerMapIterator it = busyPSMs().find(port);
        if (it != busyPSMs().end()) {
            busyPSMs().erase(it);
        } else {
            qCWarning(QT_BT_OSX) << "server is not registered";
        }
    } else {
        qCWarning(QT_BT_OSX) << "invalid protocol";
    }
}

void QBluetoothServer::close()
{
    d_ptr->listener.reset();

    // Needs a lock :(
    const QMutexLocker lock(&d_ptr->channelMapMutex());
    d_ptr->unregisterServer(d_ptr);
    d_ptr->port = 0;
}

bool QBluetoothServer::listen(const QBluetoothAddress &address, quint16 port)
{
    OSXBluetooth::qt_test_iobluetooth_runloop();

    if (d_ptr->listener) {
        qCWarning(QT_BT_OSX) << "already in listen mode, close server first";
        return false;
    }

    const QBluetoothLocalDevice device(address);
    if (!device.isValid()) {
        qCWarning(QT_BT_OSX) << "device does not support Bluetooth or"
                             << address.toString()
                             << "is not a valid local adapter";
        d_ptr->m_lastError = UnknownError;
        emit error(UnknownError);
        return false;
    }

    const QBluetoothLocalDevice::HostMode hostMode = device.hostMode();
    if (hostMode == QBluetoothLocalDevice::HostPoweredOff) {
        qCWarning(QT_BT_OSX) << "Bluetooth device is powered off";
        d_ptr->m_lastError = PoweredOffError;
        emit error(PoweredOffError);
        return false;
    }

    const ServiceInfo::Protocol type = d_ptr->serverType;

    if (type == ServiceInfo::UnknownProtocol) {
        qCWarning(QT_BT_OSX) << "invalid protocol";
        d_ptr->m_lastError = UnsupportedProtocolError;
        emit error(d_ptr->m_lastError);
        return false;
    }

    d_ptr->m_lastError = QBluetoothServer::NoError;

    // Now we have to register a (fake) port, doing a proper (?) lock.
    const QMutexLocker lock(&d_ptr->channelMapMutex());

    if (port) {
        if (type == ServiceInfo::RfcommProtocol) {
            if (d_ptr->channelIsBusy(port)) {
                qCWarning(QT_BT_OSX) << "server port:" << port
                                     << "already registered";
                d_ptr->m_lastError = ServiceAlreadyRegisteredError;
            }
        } else {
            if (d_ptr->psmIsBusy(port)) {
                qCWarning(QT_BT_OSX) << "server port:" << port
                                     << "already registered";
                d_ptr->m_lastError = ServiceAlreadyRegisteredError;
            }
        }
    } else {
        type == ServiceInfo::RfcommProtocol ? port = d_ptr->findFreeChannel()
                                       : port = d_ptr->findFreePSM();
    }

    if (d_ptr->m_lastError != QBluetoothServer::NoError) {
        emit error(d_ptr->m_lastError);
        return false;
    }

    if (!port) {
        qCWarning(QT_BT_OSX) << "all ports are busy";
        d_ptr->m_lastError = ServiceAlreadyRegisteredError;
        emit error(d_ptr->m_lastError);
        return false;
    }

    // It's a fake port, the real one will be different
    // (provided after a service was registered).
    d_ptr->port = port;
    d_ptr->registerServer(d_ptr, port);
    d_ptr->listener.reset([[ObjCListener alloc] initWithListener:d_ptr],
                          RetainPolicy::noInitialRetain);

    return true;
}

void QBluetoothServer::setMaxPendingConnections(int numConnections)
{
    d_ptr->maxPendingConnections = numConnections;
}

bool QBluetoothServer::hasPendingConnections() const
{
    return d_ptr->pendingConnections.size();
}

QBluetoothSocket *QBluetoothServer::nextPendingConnection()
{
    if (!d_ptr->pendingConnections.size())
        return nullptr;

    QScopedPointer<QBluetoothSocket> newSocket(new QBluetoothSocket);
    QBluetoothServerPrivate::PendingConnection channel(d_ptr->pendingConnections.front());

    // Remove it even if we have some errors below.
    d_ptr->pendingConnections.pop_front();

    if (d_ptr->serverType == ServiceInfo::RfcommProtocol) {
        if (!static_cast<QBluetoothSocketPrivate *>(newSocket->d_ptr)->setRFCOMChannel(channel.getAs<IOBluetoothRFCOMMChannel>()))
            return nullptr;
    } else {
        if (!static_cast<QBluetoothSocketPrivate *>(newSocket->d_ptr)->setL2CAPChannel(channel.getAs<IOBluetoothL2CAPChannel>()))
            return nullptr;
    }

    return newSocket.take();
}

QBluetoothAddress QBluetoothServer::serverAddress() const
{
    return QBluetoothLocalDevice().address();
}

quint16 QBluetoothServer::serverPort() const
{
    return d_ptr->port;
}

void QBluetoothServer::setSecurityFlags(QBluetooth::SecurityFlags security)
{
    Q_UNUSED(security)
    Q_UNIMPLEMENTED();
}

QBluetooth::SecurityFlags QBluetoothServer::securityFlags() const
{
    Q_UNIMPLEMENTED();
    return QBluetooth::NoSecurity;
}

QT_END_NAMESPACE
