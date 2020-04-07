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

#include "qbluetoothserviceinfo.h"
#include "qbluetoothserviceinfo_p.h"
#include "qbluetoothserver_p.h"
#include "qbluetoothserver.h"

#include <bluetoothapis.h>

QT_BEGIN_NAMESPACE

QBluetoothServiceInfoPrivate::QBluetoothServiceInfoPrivate()
    : registered(false)
{
}

QBluetoothServiceInfoPrivate::~QBluetoothServiceInfoPrivate()
{
}

bool QBluetoothServiceInfoPrivate::isRegistered() const
{
    return registered;
}

bool QBluetoothServiceInfoPrivate::registerService(const QBluetoothAddress &localAdapter)
{
    if (registered)
        return false;

    GUID serviceUuid = attributes.value(QBluetoothServiceInfo::ServiceId).value<QBluetoothUuid>();
    const QString name = attributes.value(QBluetoothServiceInfo::ServiceName).toString();
    const QString description = attributes.value(QBluetoothServiceInfo::ServiceDescription).toString();

    ::memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.addressFamily = AF_BTH;
    sockaddr.port = serverChannel();
    sockaddr.btAddr = localAdapter.toUInt64();

    ::memset(&addrinfo, 0, sizeof(addrinfo));
    addrinfo.LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
    addrinfo.LocalAddr.lpSockaddr = (LPSOCKADDR)&sockaddr;
    addrinfo.RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
    addrinfo.RemoteAddr.lpSockaddr = (LPSOCKADDR)&sockaddr;
    addrinfo.iSocketType = SOCK_STREAM;
    addrinfo.iProtocol = BTHPROTO_RFCOMM;

    serviceName.resize(name.size() + 1);
    name.toWCharArray(serviceName.data());
    serviceName[name.size()] = WCHAR(0);
    serviceDescription.resize(description.size() + 1);
    description.toWCharArray(serviceDescription.data());
    serviceDescription[description.size()] = WCHAR(0);

    ::memset(&regInfo, 0, sizeof(regInfo));
    regInfo.dwSize = sizeof(WSAQUERYSET);
    regInfo.dwNameSpace = NS_BTH;
    regInfo.dwNumberOfCsAddrs = 1;
    regInfo.lpcsaBuffer = &addrinfo;
    regInfo.lpszServiceInstanceName = serviceName.data();
    regInfo.lpszComment = serviceDescription.data();
    regInfo.lpServiceClassId = &serviceUuid;

    if (::WSASetService(&regInfo, RNRSERVICE_REGISTER, 0) == SOCKET_ERROR)
        return false;

    registered = true;
    return true;
}

bool QBluetoothServiceInfoPrivate::unregisterService()
{
    if (!registered)
        return false;

    if (::WSASetService(&regInfo, RNRSERVICE_DELETE, 0) == SOCKET_ERROR)
        return false;

    registered = false;
    return true;
}

QT_END_NAMESPACE
