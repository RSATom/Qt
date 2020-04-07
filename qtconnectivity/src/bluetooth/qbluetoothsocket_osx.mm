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

#include "qbluetoothservicediscoveryagent.h"
// The order is important (the first header contains
// the base class for a private socket) - workaround for
// dependencies problem.
#include "qbluetoothsocketbase_p.h"
#include "qbluetoothsocket_osx_p.h"

#include "osx/osxbtrfcommchannel_p.h"
#include "osx/osxbtl2capchannel_p.h"
#include "qbluetoothlocaldevice.h"
#include "qbluetoothdeviceinfo.h"
#include "osx/osxbtutility_p.h"
#include "osx/uistrings_p.h"
#include "qbluetoothsocket.h"

#include <QtCore/qloggingcategory.h>
#include <QtCore/qmetaobject.h>

#include <algorithm>
#include <limits>

QT_BEGIN_NAMESPACE

namespace {

using DarwinBluetooth::RetainPolicy;
using ObjCL2CAPChannel = QT_MANGLE_NAMESPACE(OSXBTL2CAPChannel);
using ObjCRFCOMMChannel = QT_MANGLE_NAMESPACE(OSXBTRFCOMMChannel);

} // unnamed namespace

QBluetoothSocketPrivate::QBluetoothSocketPrivate()
  : writeChunk(std::numeric_limits<UInt16>::max())
{
    q_ptr = nullptr;
}

QBluetoothSocketPrivate::~QBluetoothSocketPrivate()
{
}

bool QBluetoothSocketPrivate::ensureNativeSocket(QBluetoothServiceInfo::Protocol type)
{
    // For now - very simplistic, we don't call it in this file, public class
    // only calls it in a ctor, setting the protocol RFCOMM (in case of Android)
    // or, indeed, doing, socket-related initialization in BlueZ backend.
    Q_ASSERT(socketType == QBluetoothServiceInfo::UnknownProtocol);
    socketType = type;
    return type != QBluetoothServiceInfo::UnknownProtocol;
}

QString QBluetoothSocketPrivate::localName() const
{
    const QBluetoothLocalDevice device;
    return device.name();
}

QBluetoothAddress QBluetoothSocketPrivate::localAddress() const
{
    const QBluetoothLocalDevice device;
    return device.address();
}

quint16 QBluetoothSocketPrivate::localPort() const
{
    return 0;
}

QString QBluetoothSocketPrivate::peerName() const
{
    QT_BT_MAC_AUTORELEASEPOOL;

    NSString *nsName = nil;
    if (socketType == QBluetoothServiceInfo::RfcommProtocol) {
        if (rfcommChannel)
            nsName = [rfcommChannel.getAs<ObjCRFCOMMChannel>() peerName];
    } else if (socketType == QBluetoothServiceInfo::L2capProtocol) {
        if (l2capChannel)
            nsName = [l2capChannel.getAs<ObjCL2CAPChannel>() peerName];
    }

    if (nsName)
        return QString::fromNSString(nsName);

    return QString();
}

QBluetoothAddress QBluetoothSocketPrivate::peerAddress() const
{
    BluetoothDeviceAddress addr = {};
    if (socketType == QBluetoothServiceInfo::RfcommProtocol) {
        if (rfcommChannel)
            addr = [rfcommChannel.getAs<ObjCRFCOMMChannel>() peerAddress];
    } else if (socketType == QBluetoothServiceInfo::L2capProtocol) {
        if (l2capChannel)
            addr = [l2capChannel.getAs<ObjCL2CAPChannel>() peerAddress];
    }

    return OSXBluetooth::qt_address(&addr);
}

quint16 QBluetoothSocketPrivate::peerPort() const
{
    if (socketType == QBluetoothServiceInfo::RfcommProtocol) {
        if (rfcommChannel)
            return [rfcommChannel.getAs<ObjCRFCOMMChannel>() getChannelID];
    } else if (socketType == QBluetoothServiceInfo::L2capProtocol) {
        if (l2capChannel)
            return [l2capChannel.getAs<ObjCL2CAPChannel>() getPSM];
    }

    return 0;
}

void QBluetoothSocketPrivate::abort()
{
    // Can never be called while we're in connectToService:
    Q_ASSERT_X(!isConnecting, Q_FUNC_INFO, "internal inconsistency - "
               "still in connectToService()");

    if (socketType == QBluetoothServiceInfo::RfcommProtocol)
        rfcommChannel.reset();
    else if (socketType == QBluetoothServiceInfo::L2capProtocol)
        l2capChannel.reset();

    Q_ASSERT(q_ptr);

    q_ptr->setSocketState(QBluetoothSocket::UnconnectedState);
    emit q_ptr->readChannelFinished();
    emit q_ptr->disconnected();

}

void QBluetoothSocketPrivate::close()
{
    // Can never be called while we're in connectToService:
    Q_ASSERT_X(!isConnecting, Q_FUNC_INFO, "internal inconsistency - "
               "still in connectToService()");

    if (!txBuffer.size())
        abort();
}


qint64 QBluetoothSocketPrivate::writeData(const char *data, qint64 maxSize)
{
    Q_ASSERT_X(data, Q_FUNC_INFO, "invalid data (null)");
    Q_ASSERT_X(maxSize > 0, Q_FUNC_INFO, "invalid data size");

    if (state != QBluetoothSocket::ConnectedState) {
        errorString = QCoreApplication::translate(SOCKET, SOC_NOWRITE);
        q_ptr->setSocketError(QBluetoothSocket::OperationError);
        return -1;
    }

    // We do not have a real socket API under the hood,
    // IOBluetoothL2CAPChannel is buffered (writeAsync).

    if (!txBuffer.size())
        QMetaObject::invokeMethod(this, [this](){_q_writeNotify();}, Qt::QueuedConnection);

    char *dst = txBuffer.reserve(int(maxSize));
    std::copy(data, data + maxSize, dst);

    return maxSize;
}

qint64 QBluetoothSocketPrivate::readData(char *data, qint64 maxSize)
{
    if (!data)
        return 0;

    if (state != QBluetoothSocket::ConnectedState) {
        errorString = QCoreApplication::translate(SOCKET, SOC_NOREAD);
        q_ptr->setSocketError(QBluetoothSocket::OperationError);
        return -1;
    }

    if (!buffer.isEmpty())
        return buffer.read(data, int(maxSize));

    return 0;
}

qint64 QBluetoothSocketPrivate::bytesAvailable() const
{
    return buffer.size();
}

bool QBluetoothSocketPrivate::canReadLine() const
{
    return buffer.canReadLine();
}

qint64 QBluetoothSocketPrivate::bytesToWrite() const
{
    return txBuffer.size();
}

bool QBluetoothSocketPrivate::setSocketDescriptor(int socketDescriptor, QBluetoothServiceInfo::Protocol socketType,
                                                  QBluetoothSocket::SocketState socketState, QIODevice::OpenMode openMode)
{
    Q_UNUSED(socketDescriptor)
    Q_UNUSED(socketType)
    Q_UNUSED(socketState)
    Q_UNUSED(openMode)

    qCWarning(QT_BT_OSX) << "setting a socket descriptor is not supported by IOBluetooth";
    // Noop on macOS.
    return true;
}

void QBluetoothSocketPrivate::connectToServiceHelper(const QBluetoothAddress &address, quint16 port,
                                                     QIODevice::OpenMode openMode)
{
    Q_UNUSED(address)
    Q_UNUSED(port)
    Q_UNUSED(openMode)
}

void QBluetoothSocketPrivate::connectToService(const QBluetoothServiceInfo &service, QIODevice::OpenMode openMode)
{
    Q_ASSERT(q_ptr);

    OSXBluetooth::qt_test_iobluetooth_runloop();

    if (state!= QBluetoothSocket::UnconnectedState && state != QBluetoothSocket::ServiceLookupState) {
        qCWarning(QT_BT_OSX)  << "called on a busy socket";
        errorString = QCoreApplication::translate(SOCKET, SOC_CONNECT_IN_PROGRESS);
        q_ptr->setSocketError(QBluetoothSocket::OperationError);
        return;
    }

    // Report this problem early, potentially avoid device discovery:
    if (service.socketProtocol() == QBluetoothServiceInfo::UnknownProtocol) {
        qCWarning(QT_BT_OSX) << Q_FUNC_INFO << "cannot connect with 'UnknownProtocol' type";
        errorString = QCoreApplication::translate(SOCKET, SOC_NETWORK_ERROR);
        q_ptr->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return;
    }

    socketType = service.socketProtocol();

    if (service.protocolServiceMultiplexer() > 0) {
        connectToService(service.device().address(),
                         quint16(service.protocolServiceMultiplexer()),
                         openMode);
    } else if (service.serverChannel() > 0) {
        connectToService(service.device().address(),
                         quint16(service.serverChannel()),
                         openMode);
    } else {
        // Try service discovery.
        if (service.serviceUuid().isNull()) {
            qCWarning(QT_BT_OSX) << "No port, no PSM, and no "
                                    "UUID provided, unable to connect";
            return;
        }

        q_ptr->doDeviceDiscovery(service, openMode);
    }
}

void QBluetoothSocketPrivate::connectToService(const QBluetoothAddress &address, const QBluetoothUuid &uuid,
                                               QIODevice::OpenMode openMode)
{
    Q_ASSERT(q_ptr);

    OSXBluetooth::qt_test_iobluetooth_runloop();

    // Report this problem early, avoid device discovery:
    if (socketType == QBluetoothServiceInfo::UnknownProtocol) {
        qCWarning(QT_BT_OSX) << Q_FUNC_INFO << "cannot connect with 'UnknownProtocol' type";
        errorString = QCoreApplication::translate(SOCKET, SOC_NETWORK_ERROR);
        q_ptr->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return;
    }

    if (state != QBluetoothSocket::UnconnectedState) {
        qCWarning(QT_BT_OSX) << "called on a busy socket";
        errorString = QCoreApplication::translate(SOCKET, SOC_CONNECT_IN_PROGRESS);
        q_ptr->setSocketError(QBluetoothSocket::OperationError);
        return;
    }

    QBluetoothDeviceInfo device(address, QString(), QBluetoothDeviceInfo::MiscellaneousDevice);
    QBluetoothServiceInfo service;
    service.setDevice(device);
    service.setServiceUuid(uuid);
    q_ptr->doDeviceDiscovery(service, openMode);
}

void QBluetoothSocketPrivate::connectToService(const QBluetoothAddress &address, quint16 port,
                                               QIODevice::OpenMode mode)
{
    Q_ASSERT(q_ptr);

    OSXBluetooth::qt_test_iobluetooth_runloop();

    if (socketType == QBluetoothServiceInfo::UnknownProtocol) {
        qCWarning(QT_BT_OSX) << Q_FUNC_INFO << "cannot connect with 'UnknownProtocol' type";
        errorString = QCoreApplication::translate(SOCKET, SOC_NETWORK_ERROR);
        q_ptr->setSocketError(QBluetoothSocket::UnsupportedProtocolError);
        return;
    }

    Q_ASSERT_X(state == QBluetoothSocket::ServiceLookupState || state == QBluetoothSocket::UnconnectedState,
               Q_FUNC_INFO, "invalid state");

    q_ptr->setOpenMode(mode);

    socketError = QBluetoothSocket::NoSocketError;
    errorString.clear();
    buffer.clear();
    txBuffer.clear();

    IOReturn status = kIOReturnError;
    // Setting socket state on q_ptr will emit a signal,
    // we'd like to avoid any signal until this function completes.
    const QBluetoothSocket::SocketState oldState = state;
    // To prevent other connectToService calls (from QBluetoothSocket):
    // and also avoid signals in delegate callbacks.
    state = QBluetoothSocket::ConnectingState;
    // We're still inside this function:
    isConnecting = true;

    // We'll later (or now) have to set an open mode on QBluetoothSocket.
    openMode = mode;

    if (socketType == QBluetoothServiceInfo::RfcommProtocol) {
        rfcommChannel.reset([[ObjCRFCOMMChannel alloc] initWithDelegate:this], RetainPolicy::noInitialRetain);
        if (rfcommChannel)
            status = [rfcommChannel.getAs<ObjCRFCOMMChannel>() connectAsyncToDevice:address withChannelID:port];
        else
            status = kIOReturnNoMemory;
    } else if (socketType == QBluetoothServiceInfo::L2capProtocol) {
        l2capChannel.reset([[ObjCL2CAPChannel alloc] initWithDelegate:this], RetainPolicy::noInitialRetain);
        if (l2capChannel)
            status = [l2capChannel.getAs<ObjCL2CAPChannel>() connectAsyncToDevice:address withPSM:port];
        else
            status = kIOReturnNoMemory;
    }

    // We're probably still connecting, but at least are leaving this function:
    isConnecting = false;

    // QBluetoothSocket will change the state and also emit
    // a signal later if required.
    if (status == kIOReturnSuccess && socketError == QBluetoothSocket::NoSocketError) {
        if (state == QBluetoothSocket::ConnectedState) {
            // Callback 'channelOpenComplete' fired before
            // connectToService finished:
            state = oldState;
            // Connected, setOpenMode on a QBluetoothSocket.
            q_ptr->setOpenMode(openMode);
            q_ptr->setSocketState(QBluetoothSocket::ConnectedState);
            emit q_ptr->connected();
            if (buffer.size()) // We also have some data already ...
                emit q_ptr->readyRead();
        } else if (state == QBluetoothSocket::UnconnectedState) {
            // Even if we have some data, we can not read it if
            // state != ConnectedState.
            buffer.clear();
            state = oldState;
            q_ptr->setSocketError(QBluetoothSocket::UnknownSocketError);
        } else {
            // No error and we're connecting ...
            state = oldState;
            q_ptr->setSocketState(QBluetoothSocket::ConnectingState);
        }
    } else {
        state = oldState;
        if (status != kIOReturnSuccess)
            errorString = OSXBluetooth::qt_error_string(status);
        q_ptr->setSocketError(QBluetoothSocket::UnknownSocketError);
    }
}

void QBluetoothSocketPrivate::_q_writeNotify()
{
    Q_ASSERT_X(socketType == QBluetoothServiceInfo::L2capProtocol
               || socketType == QBluetoothServiceInfo::RfcommProtocol,
               Q_FUNC_INFO, "invalid socket type");
    Q_ASSERT_X(l2capChannel || rfcommChannel, Q_FUNC_INFO,
               "invalid socket (no open channel)");
    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    if (txBuffer.size()) {
        const bool isL2CAP = socketType == QBluetoothServiceInfo::L2capProtocol;
        writeChunk.resize(isL2CAP ? std::numeric_limits<UInt16>::max() :
                          [rfcommChannel.getAs<ObjCRFCOMMChannel>() getMTU]);

        const int size = txBuffer.read(writeChunk.data(), writeChunk.size());
        IOReturn status = kIOReturnError;
        if (!isL2CAP)
            status = [rfcommChannel.getAs<ObjCRFCOMMChannel>() writeAsync:writeChunk.data() length:UInt16(size)];
        else
            status = [l2capChannel.getAs<ObjCL2CAPChannel>() writeAsync:writeChunk.data() length:UInt16(size)];

        if (status != kIOReturnSuccess) {
            errorString = QCoreApplication::translate(SOCKET, SOC_NETWORK_ERROR);
            q_ptr->setSocketError(QBluetoothSocket::NetworkError);
            return;
        } else {
            emit q_ptr->bytesWritten(size);
        }
    }

    if (!txBuffer.size() && state == QBluetoothSocket::ClosingState)
        close();
}

bool QBluetoothSocketPrivate::setRFCOMChannel(void *generic)
{
    // A special case "constructor": on OS X we do not have a real listening socket,
    // instead a bluetooth server "listens" for channel open notifications and
    // creates (if asked by a user later) a "socket" object
    // for this connection. This function initializes
    // a "socket" from such an external channel (reported by a notification).
    auto channel = static_cast<IOBluetoothRFCOMMChannel *>(generic);
    // It must be a newborn socket!
    Q_ASSERT_X(socketError == QBluetoothSocket::NoSocketError
               && state == QBluetoothSocket::UnconnectedState && !rfcommChannel && !l2capChannel,
               Q_FUNC_INFO, "unexpected socket state");

    openMode = QIODevice::ReadWrite;
    rfcommChannel.reset([[ObjCRFCOMMChannel alloc] initWithDelegate:this channel:channel],
                        RetainPolicy::noInitialRetain);
    if (rfcommChannel) {// We do not handle errors, up to an external user.
        q_ptr->setOpenMode(QIODevice::ReadWrite);
        state = QBluetoothSocket::ConnectedState;
        socketType = QBluetoothServiceInfo::RfcommProtocol;
    }

    return rfcommChannel;
}

bool QBluetoothSocketPrivate::setL2CAPChannel(void *generic)
{
    // A special case "constructor": on OS X we do not have a real listening socket,
    // instead a bluetooth server "listens" for channel open notifications and
    // creates (if asked by a user later) a "socket" object
    // for this connection. This function initializes
    // a "socket" from such an external channel (reported by a notification).
    auto channel = static_cast<IOBluetoothL2CAPChannel *>(generic);

    // It must be a newborn socket!
    Q_ASSERT_X(socketError == QBluetoothSocket::NoSocketError
               && state == QBluetoothSocket::UnconnectedState && !l2capChannel && !rfcommChannel,
               Q_FUNC_INFO, "unexpected socket state");

    openMode = QIODevice::ReadWrite;
    l2capChannel.reset([[ObjCL2CAPChannel alloc] initWithDelegate:this channel:channel], RetainPolicy::noInitialRetain);
    if (l2capChannel) {// We do not handle errors, up to an external user.
        q_ptr->setOpenMode(QIODevice::ReadWrite);
        state = QBluetoothSocket::ConnectedState;
        socketType = QBluetoothServiceInfo::L2capProtocol;
    }

    return l2capChannel;
}

void QBluetoothSocketPrivate::setChannelError(IOReturn errorCode)
{
    Q_UNUSED(errorCode)

    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    if (isConnecting) {
        // The delegate's method was called while we are still in
        // connectToService ... will emit a moment later.
        socketError = QBluetoothSocket::UnknownSocketError;
    } else {
        q_ptr->setSocketError(QBluetoothSocket::UnknownSocketError);
    }
}

void QBluetoothSocketPrivate::channelOpenComplete()
{
    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    if (!isConnecting) {
        q_ptr->setSocketState(QBluetoothSocket::ConnectedState);
        q_ptr->setOpenMode(openMode);
        emit q_ptr->connected();
    } else {
        state = QBluetoothSocket::ConnectedState;
        // We are still in connectToService, it'll care
        // about signals!
    }
}

void QBluetoothSocketPrivate::channelClosed()
{
    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    // Channel was closed by IOBluetooth and we can not write any data
    // (thus close/abort probably will not work).

    if (!isConnecting) {
        q_ptr->setSocketState(QBluetoothSocket::UnconnectedState);
        q_ptr->setOpenMode(QIODevice::NotOpen);
        emit q_ptr->readChannelFinished();
        emit q_ptr->disconnected();
    } else {
        state = QBluetoothSocket::UnconnectedState;
        // We are still in connectToService and do not want
        // to emit any signals yet.
    }
}

void QBluetoothSocketPrivate::readChannelData(void *data, std::size_t size)
{
    Q_ASSERT_X(data, Q_FUNC_INFO, "invalid data (null)");
    Q_ASSERT_X(size, Q_FUNC_INFO, "invalid data size (0)");
    Q_ASSERT_X(q_ptr, Q_FUNC_INFO, "invalid q_ptr (null)");

    const char *src = static_cast<char *>(data);
    char *dst = buffer.reserve(int(size));
    std::copy(src, src + size, dst);

    if (!isConnecting) {
        // If we're still in connectToService, do not emit.
        emit q_ptr->readyRead();
    }   // else connectToService must check and emit readyRead!
}

void QBluetoothSocketPrivate::writeComplete()
{
    _q_writeNotify();
}

QT_END_NAMESPACE
