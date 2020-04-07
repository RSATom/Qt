/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qbluetoothserver.h"
#include "qbluetoothserver_p.h"
#include "qbluetoothsocket.h"
#include "qbluetoothlocaldevice.h"

#include <QtCore/QLoggingCategory>
#include <QtCore/QSocketNotifier>

#include <winsock2.h>
#include <ws2bth.h>
#include <bluetoothapis.h>
#include <errno.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_BT_WINDOWS)

QBluetoothServerPrivate::QBluetoothServerPrivate(QBluetoothServiceInfo::Protocol sType,
                                                 QBluetoothServer *parent)
    : maxPendingConnections(1), serverType(sType), q_ptr(parent),
      m_lastError(QBluetoothServer::NoError)
{
    Q_Q(QBluetoothServer);
    Q_ASSERT(sType == QBluetoothServiceInfo::RfcommProtocol);
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, q);
}

QBluetoothServerPrivate::~QBluetoothServerPrivate()
{
}

void QBluetoothServerPrivate::_q_newConnection()
{
    // disable socket notifier until application calls nextPendingConnection().
    socketNotifier->setEnabled(false);

    emit q_ptr->newConnection();
}

void QBluetoothServer::close()
{
    Q_D(QBluetoothServer);

    delete d->socketNotifier;
    d->socketNotifier = nullptr;

    d->socket->close();
}

bool QBluetoothServer::listen(const QBluetoothAddress &address, quint16 port)
{
    Q_D(QBluetoothServer);

    if (d->serverType != QBluetoothServiceInfo::RfcommProtocol) {
        qCWarning(QT_BT_WINDOWS) << "Protocol is not supported.";
        d->m_lastError = QBluetoothServer::UnsupportedProtocolError;
        emit error(d->m_lastError);
        return false;
    }

    if (d->socket->state() == QBluetoothSocket::ListeningState) {
        qCWarning(QT_BT_WINDOWS) << "Socket already in listen mode, close server first";
        return false;
    }

    const QBluetoothLocalDevice device(address);
    if (!device.isValid()) {
        qCWarning(QT_BT_WINDOWS) << "Device does not support Bluetooth or"
                                 << address.toString() << "is not a valid local adapter";
        d->m_lastError = QBluetoothServer::UnknownError;
        emit error(d->m_lastError);
        return false;
    }

    const QBluetoothLocalDevice::HostMode hostMode = device.hostMode();
    if (hostMode == QBluetoothLocalDevice::HostPoweredOff) {
        d->m_lastError = QBluetoothServer::PoweredOffError;
        emit error(d->m_lastError);
        qCWarning(QT_BT_WINDOWS) << "Bluetooth device is powered off";
        return false;
    }

    int sock = d->socket->socketDescriptor();
    if (sock < 0) {
        /* Negative socket descriptor is not always an error case.
         * Another cause could be a call to close()/abort().
         * Check whether we can recover by re-creating the socket.
         */
        delete d->socket;
        d->socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this);
        sock = d->socket->socketDescriptor();
        if (sock < 0) {
            d->m_lastError = InputOutputError;
            emit error(d->m_lastError);
            return false;
        }
    }

    if (sock < 0)
        return false;

    SOCKADDR_BTH addr = {};
    addr.addressFamily = AF_BTH;
    addr.port = (port == 0) ? BT_PORT_ANY : port;
    addr.btAddr = address.toUInt64();

    if (::bind(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(SOCKADDR_BTH)) < 0) {
        if (errno == EADDRINUSE)
            d->m_lastError = ServiceAlreadyRegisteredError;
        else
            d->m_lastError = InputOutputError;
        emit error(d->m_lastError);
        return false;
    }

    if (::listen(sock, d->maxPendingConnections) < 0) {
        d->m_lastError = InputOutputError;
        emit error(d->m_lastError);
        return false;
    }

    d->socket->setSocketState(QBluetoothSocket::ListeningState);

    if (!d->socketNotifier) {
        d->socketNotifier = new QSocketNotifier(d->socket->socketDescriptor(),
                                                QSocketNotifier::Read, this);
        connect(d->socketNotifier, &QSocketNotifier::activated, this, [d](){
            d->_q_newConnection();
        });
    }

    return true;
}

void QBluetoothServer::setMaxPendingConnections(int numConnections)
{
    Q_UNUSED(numConnections);
}

bool QBluetoothServer::hasPendingConnections() const
{
    Q_D(const QBluetoothServer);

    if (!d || !d->socketNotifier)
        return false;

    // if the socket notifier is disabled there is a pending connection waiting for us to accept.
    return !d->socketNotifier->isEnabled();
}

QBluetoothSocket *QBluetoothServer::nextPendingConnection()
{
    Q_D(QBluetoothServer);

    if (!hasPendingConnections())
        return nullptr;

    if (d->serverType != QBluetoothServiceInfo::RfcommProtocol)
        return nullptr;

    SOCKADDR_BTH addr = {};
    int length = sizeof(SOCKADDR_BTH);
    int pending = ::accept(d->socket->socketDescriptor(),
                           reinterpret_cast<sockaddr *>(&addr), &length);

    QBluetoothSocket *newSocket = nullptr;

    if (pending >= 0) {
        newSocket = new QBluetoothSocket();
        newSocket->setSocketDescriptor(pending, QBluetoothServiceInfo::RfcommProtocol);
    }

    d->socketNotifier->setEnabled(true);
    return newSocket;
}

QBluetoothAddress QBluetoothServer::serverAddress() const
{
    Q_D(const QBluetoothServer);

    return d->socket->localAddress();
}

quint16 QBluetoothServer::serverPort() const
{
    Q_D(const QBluetoothServer);

    return d->socket->localPort();
}

void QBluetoothServer::setSecurityFlags(QBluetooth::SecurityFlags security)
{
    Q_UNUSED(security);
}

QBluetooth::SecurityFlags QBluetoothServer::securityFlags() const
{
    return QBluetooth::NoSecurity;
}

QT_END_NAMESPACE
