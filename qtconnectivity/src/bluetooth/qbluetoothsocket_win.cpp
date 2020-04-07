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

#include "qbluetoothsocket.h"
#include "qbluetoothsocket_win_p.h"

#include <QtCore/qloggingcategory.h>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtCore/QSocketNotifier>

#include <winsock2.h>
#include <ws2bth.h>
#include <bluetoothapis.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(QT_BT_WINDOWS)

QBluetoothSocketPrivateWin::QBluetoothSocketPrivateWin()
    : QBluetoothSocketBasePrivate()
{
    WSAData wsadata = {};
    ::WSAStartup(MAKEWORD(2, 0), &wsadata);
}

QBluetoothSocketPrivateWin::~QBluetoothSocketPrivateWin()
{
    abort();
}

bool QBluetoothSocketPrivateWin::ensureNativeSocket(QBluetoothServiceInfo::Protocol type)
{
    Q_Q(QBluetoothSocket);

    if (static_cast<SOCKET>(socket) != INVALID_SOCKET) {
        if (socketType == type)
            return true;
        abort();
    }
    socketType = type;

    if (type != QBluetoothServiceInfo::RfcommProtocol) {
        socket = int(INVALID_SOCKET);
        errorString = QBluetoothSocket::tr("Unsupported protocol. Win32 only supports RFCOMM sockets");
        q->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return false;
    }

    socket = ::socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

    if (static_cast<SOCKET>(socket) == INVALID_SOCKET) {
        const int error = ::WSAGetLastError();
        qCWarning(QT_BT_WINDOWS) << "Failed to create socket:" << error << qt_error_string(error);
        errorString = QBluetoothSocket::tr("Failed to create socket");
        q->setSocketError(QBluetoothSocket::UnknownSocketError);
        return false;
    }

    if (!createNotifiers())
        return false;

    return true;
}

void QBluetoothSocketPrivateWin::connectToServiceHelper(const QBluetoothAddress &address, quint16 port, QIODevice::OpenMode openMode)
{
    Q_Q(QBluetoothSocket);

    if (static_cast<SOCKET>(socket) == INVALID_SOCKET && !ensureNativeSocket(socketType))
        return;

    if (!configureSecurity())
        return;

    SOCKADDR_BTH addr = {};
    addr.addressFamily = AF_BTH;
    addr.port = port;
    addr.btAddr = address.toUInt64();

    switch (socketType) {
    case QBluetoothServiceInfo::RfcommProtocol:
        addr.serviceClassId = RFCOMM_PROTOCOL_UUID;
        break;
    default:
        errorString = QBluetoothSocket::tr("Socket type not handled: %1").arg(socketType);
        q->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return;
    }

    connectWriteNotifier->setEnabled(true);
    readNotifier->setEnabled(true);
    exceptNotifier->setEnabled(true);

    const int result = ::connect(socket, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));

    const int error = ::WSAGetLastError();
    if (result != SOCKET_ERROR || error == WSAEWOULDBLOCK) {
        q->setSocketState(QBluetoothSocket::ConnectingState);
        q->setOpenMode(openMode);
    } else {
        errorString = qt_error_string(error);
        q->setSocketError(QBluetoothSocket::UnknownSocketError);
    }
}

void QBluetoothSocketPrivateWin::connectToService(
        const QBluetoothServiceInfo &service, QIODevice::OpenMode openMode)
{
    Q_Q(QBluetoothSocket);

    if (q->state() != QBluetoothSocket::UnconnectedState
            && q->state() != QBluetoothSocket::ServiceLookupState) {
        //qCWarning(QT_BT_WINDOWS) << "QBluetoothSocketPrivateWIN::connectToService called on busy socket";
        errorString = QBluetoothSocket::tr("Trying to connect while connection is in progress");
        q->setSocketError(QBluetoothSocket::OperationError);
        return;
    }

    // we are checking the service protocol and not socketType()
    // socketType will change in ensureNativeSocket()
    if (service.socketProtocol() != QBluetoothServiceInfo::RfcommProtocol) {
        qCWarning(QT_BT_WINDOWS) << "QBluetoothSocket::connectToService called with unsupported protocol";
        errorString = QBluetoothSocket::tr("Socket type not supported");
        q->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return;
    }

    if (service.serverChannel() > 0) {
        if (!ensureNativeSocket(QBluetoothServiceInfo::RfcommProtocol)) {
            errorString = QBluetoothSocket::tr("Unknown socket error");
            q->setSocketError(QBluetoothSocket::UnknownSocketError);
            return;
        }
        connectToServiceHelper(service.device().address(), service.serverChannel(), openMode);
    } else {
        // try doing service discovery to see if we can find the socket
        if (service.serviceUuid().isNull()
                && !service.serviceClassUuids().contains(QBluetoothUuid::SerialPort)) {
            qCWarning(QT_BT_WINDOWS) << "No port, no PSM, and no UUID provided. Unable to connect";
            return;
        }
        qCDebug(QT_BT_WINDOWS) << "Need a port/psm, doing discovery";
        q->doDeviceDiscovery(service, openMode);
    }
}

void QBluetoothSocketPrivateWin::_q_writeNotify()
{
    Q_Q(QBluetoothSocket);

    if (state == QBluetoothSocket::ConnectingState) {
        q->setSocketState(QBluetoothSocket::ConnectedState);
        connectWriteNotifier->setEnabled(false);
    } else {
        if (txBuffer.isEmpty()) {
            connectWriteNotifier->setEnabled(false);
            return;
        }

        char buf[1024];
        const int size = txBuffer.read(&buf[0], sizeof(buf));
        const int writtenBytes = ::send(socket, &buf[0], size, 0);
        if (writtenBytes == SOCKET_ERROR) {
            // every other case returns error
            const int error = ::WSAGetLastError();
            errorString = QBluetoothSocket::tr("Network Error: %1").arg(qt_error_string(error));
            q->setSocketError(QBluetoothSocket::NetworkError);
        } else if (writtenBytes <= size) {
            // add remainder back to buffer
            const char *remainder = &buf[writtenBytes];
            txBuffer.ungetBlock(remainder, size - writtenBytes);
            if (writtenBytes > 0)
                emit q->bytesWritten(writtenBytes);
        } else {
            errorString = QBluetoothSocket::tr("Logic error: more bytes sent than passed to ::send");
            q->setSocketError(QBluetoothSocket::NetworkError);
        }

        if (!txBuffer.isEmpty()) {
            connectWriteNotifier->setEnabled(true);
        } else if (state == QBluetoothSocket::ClosingState) {
            connectWriteNotifier->setEnabled(false);
            this->close();
        }
    }
}

void QBluetoothSocketPrivateWin::_q_readNotify()
{
    Q_Q(QBluetoothSocket);

    char *writePointer = buffer.reserve(QPRIVATELINEARBUFFER_BUFFERSIZE);
    const int bytesRead = ::recv(socket, writePointer, QPRIVATELINEARBUFFER_BUFFERSIZE, 0);
    if (bytesRead == SOCKET_ERROR) {
        const int error = ::WSAGetLastError();
        buffer.chop(QPRIVATELINEARBUFFER_BUFFERSIZE);
        readNotifier->setEnabled(false);
        connectWriteNotifier->setEnabled(false);
        errorString = qt_error_string(error);
        qCWarning(QT_BT_WINDOWS) << Q_FUNC_INFO << socket << "error:" << error << errorString;
        switch (error) {
        case WSAEHOSTDOWN:
            q->setSocketError(QBluetoothSocket::HostNotFoundError);
            break;
        case WSAECONNRESET:
            q->setSocketError(QBluetoothSocket::RemoteHostClosedError);
            break;
        default:
            q->setSocketError(QBluetoothSocket::UnknownSocketError);
            break;
        }

        q->disconnectFromService();
    } else if (bytesRead == 0) {
        q->setSocketError(QBluetoothSocket::RemoteHostClosedError);
        q->setSocketState(QBluetoothSocket::UnconnectedState);
    } else {
        const int unusedBytes = QPRIVATELINEARBUFFER_BUFFERSIZE -  bytesRead;
        buffer.chop(unusedBytes);
        if (bytesRead > 0)
            emit q->readyRead();
    }
}

void QBluetoothSocketPrivateWin::_q_exceptNotify()
{
    Q_Q(QBluetoothSocket);

    const int error = ::WSAGetLastError();
    errorString = qt_error_string(error);
    q->setSocketError(QBluetoothSocket::UnknownSocketError);

    if (state == QBluetoothSocket::ConnectingState)
        abort();
}

void QBluetoothSocketPrivateWin::connectToService(
        const QBluetoothAddress &address, const QBluetoothUuid &uuid,
        QIODevice::OpenMode openMode)
{
    Q_Q(QBluetoothSocket);

    if (q->state() != QBluetoothSocket::UnconnectedState) {
        qCWarning(QT_BT_WINDOWS) << "QBluetoothSocketPrivateWin::connectToService called on busy socket";
        errorString = QBluetoothSocket::tr("Trying to connect while connection is in progress");
        q->setSocketError(QBluetoothSocket::OperationError);
        return;
    }

    if (q->socketType() == QBluetoothServiceInfo::UnknownProtocol) {
        qCWarning(QT_BT_WINDOWS) << "QBluetoothSocketPrivateWin::connectToService cannot "
                                  "connect with 'UnknownProtocol' (type provided by given service)";
        errorString = QBluetoothSocket::tr("Socket type not supported");
        q->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return;
    }

    QBluetoothServiceInfo service;
    QBluetoothDeviceInfo device(address, QString(), QBluetoothDeviceInfo::MiscellaneousDevice);
    service.setDevice(device);
    service.setServiceUuid(uuid);
    q->doDeviceDiscovery(service, openMode);
}

void QBluetoothSocketPrivateWin::connectToService(
        const QBluetoothAddress &address, quint16 port, QIODevice::OpenMode openMode)
{
    Q_Q(QBluetoothSocket);

    if (q->socketType() == QBluetoothServiceInfo::UnknownProtocol) {
        qCWarning(QT_BT_WINDOWS) << "QBluetoothSocketPrivateWin::connectToService cannot "
                                  "connect with 'UnknownProtocol' (type provided by given service)";
        errorString = QBluetoothSocket::tr("Socket type not supported");
        q->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return;
    }

    if (q->state() != QBluetoothSocket::UnconnectedState) {
        qCWarning(QT_BT_WINDOWS) << "QBluetoothSocketPrivateWin::connectToService called on busy socket";
        errorString = QBluetoothSocket::tr("Trying to connect while connection is in progress");
        q->setSocketError(QBluetoothSocket::OperationError);
        return;
    }

    q->setOpenMode(openMode);
    connectToServiceHelper(address, port, openMode);
}

void QBluetoothSocketPrivateWin::abort()
{
    delete readNotifier;
    readNotifier = nullptr;
    delete connectWriteNotifier;
    connectWriteNotifier = nullptr;
    delete exceptNotifier;
    exceptNotifier = nullptr;

    // We don't transition through Closing for abort, so
    // we don't call disconnectFromService or QBluetoothSocket::close
    ::closesocket(socket);
    socket = int(INVALID_SOCKET);

    Q_Q(QBluetoothSocket);

    const bool wasConnected = q->state() == QBluetoothSocket::ConnectedState;
    q->setSocketState(QBluetoothSocket::UnconnectedState);
    if (wasConnected) {
        q->setOpenMode(QIODevice::NotOpen);
        emit q->readChannelFinished();
    }
}

QString QBluetoothSocketPrivateWin::localName() const
{
    const QBluetoothAddress localAddr = localAddress();
    if (localAddr == QBluetoothAddress())
        return {};
    const QBluetoothLocalDevice device(localAddr);
    return device.name();
}

QBluetoothAddress QBluetoothSocketPrivateWin::localAddress() const
{
    if (static_cast<SOCKET>(socket) == INVALID_SOCKET)
        return {};
    SOCKADDR_BTH localAddr = {};
    int localAddrLength = sizeof(localAddr);
    const int localResult = ::getsockname(socket, reinterpret_cast<sockaddr *>(&localAddr), &localAddrLength);
    if (localResult == SOCKET_ERROR) {
        const int error = ::WSAGetLastError();
        qCWarning(QT_BT_WINDOWS) << "Error getting local address" << error << qt_error_string(error);
        return {};
    }
    return QBluetoothAddress(localAddr.btAddr);
}

quint16 QBluetoothSocketPrivateWin::localPort() const
{
    if (static_cast<SOCKET>(socket) == INVALID_SOCKET)
        return {};
    SOCKADDR_BTH localAddr = {};
    int localAddrLength = sizeof(localAddr);
    const int localResult = ::getsockname(socket, reinterpret_cast<sockaddr *>(&localAddr), &localAddrLength);
    if (localResult == SOCKET_ERROR) {
        const int error = ::WSAGetLastError();
        qCWarning(QT_BT_WINDOWS) << "Error getting local port" << error << qt_error_string(error);
        return {};
    }
    return localAddr.port;
}

QString QBluetoothSocketPrivateWin::peerName() const
{
    const QBluetoothAddress peerAddr = peerAddress();
    if (peerAddr == QBluetoothAddress())
        return {};
    BLUETOOTH_DEVICE_INFO bdi = {};
    bdi.dwSize = sizeof(bdi);
    bdi.Address.ullLong = peerAddr.toUInt64();
    const DWORD res = ::BluetoothGetDeviceInfo(nullptr, &bdi);
    if (res != ERROR_SUCCESS) {
        qCWarning(QT_BT_WINDOWS) << "Error calling BluetoothGetDeviceInfo" << res << qt_error_string(res);
        return {};
    }
    return QString::fromWCharArray(&bdi.szName[0]);
}

QBluetoothAddress QBluetoothSocketPrivateWin::peerAddress() const
{
    if (static_cast<SOCKET>(socket) == INVALID_SOCKET)
        return {};
    SOCKADDR_BTH peerAddr = {};
    int peerAddrLength = sizeof(peerAddr);
    const int peerResult = ::getpeername(socket, reinterpret_cast<sockaddr *>(&peerAddr), &peerAddrLength);
    if (peerResult == SOCKET_ERROR) {
        const int error = ::WSAGetLastError();
        qCWarning(QT_BT_WINDOWS) << "Error getting peer address and port" << error << qt_error_string(error);
        return {};
    }
    return QBluetoothAddress(peerAddr.btAddr);
}

quint16 QBluetoothSocketPrivateWin::peerPort() const
{
    if (static_cast<SOCKET>(socket) == INVALID_SOCKET)
        return {};
    SOCKADDR_BTH peerAddr = {};
    int peerAddrLength = sizeof(peerAddr);
    const int peerResult = ::getpeername(socket, reinterpret_cast<sockaddr *>(&peerAddr), &peerAddrLength);
    if (peerResult == SOCKET_ERROR) {
        const int error = ::WSAGetLastError();
        qCWarning(QT_BT_WINDOWS) << "Error getting peer address and port" << error << qt_error_string(error);
        return {};
    }
    return peerAddr.port;
}

qint64 QBluetoothSocketPrivateWin::writeData(const char *data, qint64 maxSize)
{
    Q_Q(QBluetoothSocket);

    if (state != QBluetoothSocket::ConnectedState) {
        errorString = QBluetoothSocket::tr("Cannot write while not connected");
        q->setSocketError(QBluetoothSocket::OperationError);
        return -1;
    }

    if (q->openMode() & QIODevice::Unbuffered) {
        const int bytesWritten = ::send(socket, data, maxSize, 0);

        if (bytesWritten == SOCKET_ERROR) {
            const int error = ::WSAGetLastError();
            errorString = QBluetoothSocket::tr("Network Error: %1").arg(qt_error_string(error));
            q->setSocketError(QBluetoothSocket::NetworkError);
        }

        if (bytesWritten > 0)
            emit q->bytesWritten(bytesWritten);

        return bytesWritten;
    } else {

        if (!connectWriteNotifier)
            return -1;

        if (txBuffer.isEmpty()) {
            connectWriteNotifier->setEnabled(true);
            QMetaObject::invokeMethod(this, "_q_writeNotify", Qt::QueuedConnection);
        }

        char *txbuf = txBuffer.reserve(maxSize);
        ::memcpy(txbuf, data, maxSize);

        return maxSize;
    }
}

qint64 QBluetoothSocketPrivateWin::readData(char *data, qint64 maxSize)
{
    Q_Q(QBluetoothSocket);

    if (state != QBluetoothSocket::ConnectedState) {
        errorString = QBluetoothSocket::tr("Cannot read while not connected");
        q->setSocketError(QBluetoothSocket::OperationError);
        return -1;
    }

    const int bytesRead = buffer.read(data, maxSize);
    return bytesRead;
}

void QBluetoothSocketPrivateWin::close()
{
    if (txBuffer.isEmpty())
        abort();
    else
        connectWriteNotifier->setEnabled(true);
}

bool QBluetoothSocketPrivateWin::setSocketDescriptor(int socketDescriptor,
                                                     QBluetoothServiceInfo::Protocol protocol,
                                                     QBluetoothSocket::SocketState socketState,
                                                     QBluetoothSocket::OpenMode openMode)
{
    Q_Q(QBluetoothSocket);

    abort();

    socketType = protocol;
    socket = socketDescriptor;

    if (!createNotifiers())
        return false;
    q->setSocketState(socketState);
    q->setOpenMode(openMode);
    if (socketState == QBluetoothSocket::ConnectedState) {
        connectWriteNotifier->setEnabled(true);
        readNotifier->setEnabled(true);
        exceptNotifier->setEnabled(true);
    }

    return true;
}

qint64 QBluetoothSocketPrivateWin::bytesAvailable() const
{
    return buffer.size();
}

bool QBluetoothSocketPrivateWin::canReadLine() const
{
    return buffer.canReadLine();
}

qint64 QBluetoothSocketPrivateWin::bytesToWrite() const
{
    return txBuffer.size();
}

bool QBluetoothSocketPrivateWin::createNotifiers()
{
    Q_Q(QBluetoothSocket);

    ULONG mode = 1;  // 1 to enable non-blocking socket
    const int result = ::ioctlsocket(socket, FIONBIO, &mode);

    if (result == SOCKET_ERROR) {
        const int error = ::WSAGetLastError();
        errorString = qt_error_string(error);
        q->setSocketError(QBluetoothSocket::UnknownSocketError);
        qCWarning(QT_BT_WINDOWS) << "Error setting socket to non-blocking" << error << errorString;
        abort();
        return false;
    }
    readNotifier = new QSocketNotifier(socket, QSocketNotifier::Read);
    QObject::connect(readNotifier, &QSocketNotifier::activated, this, &QBluetoothSocketPrivateWin::_q_readNotify);
    connectWriteNotifier = new QSocketNotifier(socket, QSocketNotifier::Write, q);
    QObject::connect(connectWriteNotifier, &QSocketNotifier::activated, this, &QBluetoothSocketPrivateWin::_q_writeNotify);
    exceptNotifier = new QSocketNotifier(socket, QSocketNotifier::Exception, q);
    QObject::connect(exceptNotifier, &QSocketNotifier::activated, this, &QBluetoothSocketPrivateWin::_q_exceptNotify);

    connectWriteNotifier->setEnabled(false);
    readNotifier->setEnabled(false);
    exceptNotifier->setEnabled(false);
    return true;
}

bool QBluetoothSocketPrivateWin::configureSecurity()
{
    Q_Q(QBluetoothSocket);

    if (secFlags & QBluetooth::Authorization) {
        ULONG authenticate = TRUE;
        const int result = ::setsockopt(socket, SOL_RFCOMM, SO_BTH_AUTHENTICATE, reinterpret_cast<const char*>(&authenticate), sizeof(authenticate));
        if (result == SOCKET_ERROR) {
            const int error = ::WSAGetLastError();
            qCWarning(QT_BT_WINDOWS) << "Failed to set socket option, closing socket for safety" << error;
            qCWarning(QT_BT_WINDOWS) << "Error: " << qt_error_string(error);
            errorString = QBluetoothSocket::tr("Cannot set connection security level");
            q->setSocketError(QBluetoothSocket::UnknownSocketError);
            return false;
        }
    }

    if (secFlags & QBluetooth::Encryption) {
        ULONG encrypt = TRUE;
        const int result = ::setsockopt(socket, SOL_RFCOMM, SO_BTH_ENCRYPT, reinterpret_cast<const char*>(&encrypt), sizeof(encrypt));
        if (result == SOCKET_ERROR) {
            const int error = ::WSAGetLastError();
            qCWarning(QT_BT_WINDOWS) << "Failed to set socket option, closing socket for safety" << error;
            qCWarning(QT_BT_WINDOWS) << "Error: " << qt_error_string(error);
            errorString = QBluetoothSocket::tr("Cannot set connection security level");
            q->setSocketError(QBluetoothSocket::UnknownSocketError);
            return false;
        }
    }
    return true;
}
QT_END_NAMESPACE
