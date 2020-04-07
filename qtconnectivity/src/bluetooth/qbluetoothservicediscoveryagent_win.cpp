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

#include "qbluetoothservicediscoveryagent.h"
#include "qbluetoothservicediscoveryagent_p.h"

#include <QtCore/QByteArray>
#include <QtCore/QLibrary>
#include <QtCore/QLoggingCategory>
#include <QtCore/QThread>
#include <QtCore/QUrl>

#include <initguid.h>
#include <winsock2.h>
#include <qt_windows.h>

#if defined(Q_CC_MINGW)
// Workaround for MinGW headers declaring BluetoothSdpGetElementData incorrectly.
# define BluetoothSdpGetElementData _BluetoothSdpGetElementData_notok
# include <bluetoothapis.h>
# undef BluetoothSdpGetElementData
  extern "C" DWORD WINAPI BluetoothSdpGetElementData(LPBYTE, ULONG, PSDP_ELEMENT_DATA);
#else
# include <bluetoothapis.h>
#endif

#include <ws2bth.h>
#include <iostream>

QT_BEGIN_NAMESPACE

struct FindServiceResult {
    QBluetoothServiceInfo info;
    Qt::HANDLE hSearch = INVALID_HANDLE_VALUE;
    int systemError = NO_ERROR;
};

Q_DECLARE_LOGGING_CATEGORY(QT_BT_WINDOWS)

static QList<QVariant> spdContainerToVariantList(LPBYTE containerStream, ULONG containerLength);

static QVariant spdElementToVariant(const SDP_ELEMENT_DATA &element)
{
    QVariant variant;

    switch (element.type) {
    case SDP_TYPE_UINT: {
        switch (element.specificType) {
        case SDP_ST_UINT128:
            //Not supported!!!
            break;
        case SDP_ST_UINT64:
            variant =  QVariant::fromValue<quint64>(element.data.uint64);
            break;
        case SDP_ST_UINT32:
            variant =  QVariant::fromValue<quint32>(element.data.uint32);
            break;
        case SDP_ST_UINT16:
            variant =  QVariant::fromValue<quint16>(element.data.uint16);
            break;
        case SDP_ST_UINT8:
            variant =  QVariant::fromValue<quint8>(element.data.uint8);
            break;
        default:
            break;
        }
        break;
    }
    case SDP_TYPE_INT: {
        switch (element.specificType) {
        case SDP_ST_INT128: {
            //Not supported!!!
            break;
        }
        case SDP_ST_INT64:
            variant = QVariant::fromValue<qint64>(element.data.int64);
            break;
        case SDP_ST_INT32:
            variant = QVariant::fromValue<qint32>(element.data.int32);
            break;
        case SDP_ST_INT16:
            variant = QVariant::fromValue<qint16>(element.data.int16);
            break;
        case SDP_ST_INT8:
            variant = QVariant::fromValue<qint8>(element.data.int8);
            break;
        default:
            break;
        }
        break;
    }
    case SDP_TYPE_UUID: {
        switch (element.specificType) {
        case SDP_ST_UUID128:
            variant = QVariant::fromValue(QBluetoothUuid(element.data.uuid128));
            break;
        case SDP_ST_UUID32:
            variant = QVariant::fromValue(QBluetoothUuid(quint32(element.data.uuid32)));
            break;
        case SDP_ST_UUID16:
            variant = QVariant::fromValue(QBluetoothUuid(quint16(element.data.uuid16)));
            break;
        default:
            break;
        }
        break;
    }
    case SDP_TYPE_STRING: {
        const QByteArray stringBuffer(reinterpret_cast<const char*>(element.data.string.value), element.data.string.length);
        variant = QVariant::fromValue<QString>(QString::fromLocal8Bit(stringBuffer));
        break;
    }
    case SDP_TYPE_URL: {
        const QString urlString = QString::fromLocal8Bit(reinterpret_cast<const char*>(element.data.url.value),
                                                   int(element.data.url.length));
        const QUrl url(urlString);
        if (url.isValid())
            variant = QVariant::fromValue<QUrl>(url);
        break;
    }
    case SDP_TYPE_SEQUENCE: {
        const QList<QVariant> sequenceList = spdContainerToVariantList(element.data.sequence.value,
                                                                 element.data.sequence.length);
        const QBluetoothServiceInfo::Sequence sequence(sequenceList);
        variant = QVariant::fromValue(sequence);
        break;
    }
    case SDP_TYPE_ALTERNATIVE: {
        const QList<QVariant> alternativeList = spdContainerToVariantList(element.data.alternative.value,
                                                                    element.data.alternative.length);
        const QBluetoothServiceInfo::Alternative alternative(alternativeList);
        variant = QVariant::fromValue(alternative);
        break;
    }
    case SDP_TYPE_BOOLEAN:
        variant = QVariant::fromValue<bool>(bool(element.data.booleanVal));
        break;
    case SDP_TYPE_NIL:
        break;
    default:
        break;
    }

    return variant;
}

static QList<QVariant> spdContainerToVariantList(LPBYTE containerStream, ULONG containerLength)
{
    HBLUETOOTH_CONTAINER_ELEMENT iter = nullptr;
    SDP_ELEMENT_DATA element = {};

    QList<QVariant> sequence;

    for (;;) {
        const DWORD result = ::BluetoothSdpGetContainerElementData(containerStream,
                                                           containerLength,
                                                           &iter,
                                                           &element);

        if (result == ERROR_SUCCESS) {
            const QVariant variant = spdElementToVariant(element);
            sequence.append(variant);
        } else if (result == ERROR_NO_MORE_ITEMS) {
            break;
        } else if (result == ERROR_INVALID_PARAMETER) {
            break;
        }
    }

    return sequence;
}

#if defined(Q_CC_MINGW)
# define SDP_CALLBACK
#else
# define SDP_CALLBACK QT_WIN_CALLBACK
#endif
static BOOL SDP_CALLBACK bluetoothSdpCallback(ULONG attributeId, LPBYTE valueStream, ULONG streamSize, LPVOID param)
{
    QBluetoothServiceInfo *result = static_cast<QBluetoothServiceInfo*>(param);

    SDP_ELEMENT_DATA element = {};

    if (::BluetoothSdpGetElementData(valueStream, streamSize, &element) == ERROR_SUCCESS) {
        switch (element.type) {
        case SDP_TYPE_UINT:
        case SDP_TYPE_INT:
        case SDP_TYPE_UUID:
        case SDP_TYPE_STRING:
        case SDP_TYPE_URL:
        case SDP_TYPE_BOOLEAN:
        case SDP_TYPE_SEQUENCE:
        case SDP_TYPE_ALTERNATIVE: {
            const QVariant variant = spdElementToVariant(element);

            result->setAttribute(attributeId, variant);
            break;
        }
        case SDP_TYPE_NIL:
            break;
        default:
            break;
        }
    }
    return true;
}

enum {
    WSAControlFlags = LUP_FLUSHCACHE
          | LUP_RETURN_NAME
          | LUP_RETURN_TYPE
          | LUP_RETURN_ADDR
          | LUP_RETURN_BLOB
          | LUP_RETURN_COMMENT
};

static FindServiceResult findNextService(HANDLE hSearch)
{
    FindServiceResult result;
    result.hSearch = hSearch;

    QByteArray resultBuffer(2048, 0);
    WSAQUERYSET *resultQuery = reinterpret_cast<WSAQUERYSET*>(resultBuffer.data());
    DWORD resultBufferSize = DWORD(resultBuffer.size());
    const int resultCode = ::WSALookupServiceNext(hSearch,
                                                WSAControlFlags,
                                                &resultBufferSize,
                                                resultQuery);

    if (resultCode == SOCKET_ERROR) {
        result.systemError = ::WSAGetLastError();
        if (result.systemError == WSA_E_NO_MORE)
            ::WSALookupServiceEnd(hSearch);
        return result;
     }

    if (resultQuery->lpBlob
            && ::BluetoothSdpEnumAttributes(resultQuery->lpBlob->pBlobData,
                                          resultQuery->lpBlob->cbSize,
                                          bluetoothSdpCallback,
                                          &result.info)) {
        return result;
    } else {
        result.systemError = GetLastError();
    }
    return result;
}

static FindServiceResult findFirstService(const QBluetoothAddress &address)
{
    WSAData wsadata = {};
    FindServiceResult result;

    // IPv6 requires Winsock v2.0 or better.
    if (::WSAStartup(MAKEWORD(2, 0), &wsadata) != 0) {
        result.systemError = ::WSAGetLastError();
        return result;
    }

    const QString addressAsString = QStringLiteral("(%1)").arg(address.toString());
    QVector<WCHAR> addressAsWChar(addressAsString.size() + 1);
    addressAsString.toWCharArray(addressAsWChar.data());

    GUID protocol = L2CAP_PROTOCOL_UUID; //Search for L2CAP and RFCOMM services

    WSAQUERYSET serviceQuery = {};
    serviceQuery.dwSize = sizeof(WSAQUERYSET);
    serviceQuery.lpServiceClassId = &protocol;
    serviceQuery.dwNameSpace = NS_BTH;
    serviceQuery.dwNumberOfCsAddrs = 0;
    serviceQuery.lpszContext = addressAsWChar.data();

    HANDLE hSearch = nullptr;
    const int resultCode = ::WSALookupServiceBegin(&serviceQuery,
                                                 WSAControlFlags,
                                                 &hSearch);
    if (resultCode == SOCKET_ERROR) {
        result.systemError = ::WSAGetLastError();
        ::WSALookupServiceEnd(hSearch);
        return result;
    }
    return findNextService(hSearch);
}

QBluetoothServiceDiscoveryAgentPrivate::QBluetoothServiceDiscoveryAgentPrivate(
        QBluetoothServiceDiscoveryAgent *qp, const QBluetoothAddress &deviceAdapter)
    :  error(QBluetoothServiceDiscoveryAgent::NoError),
      state(Inactive),
      deviceDiscoveryAgent(0),
      mode(QBluetoothServiceDiscoveryAgent::MinimalDiscovery),
      singleDevice(false),
      pendingStop(false),
      pendingFinish(false),
      q_ptr(qp)
{
    Q_UNUSED(deviceAdapter);

    threadFind = new QThread;
    threadWorkerFind = new ThreadWorkerFind;
    threadWorkerFind->moveToThread(threadFind);
    connect(threadWorkerFind, &ThreadWorkerFind::findFinished, this, &QBluetoothServiceDiscoveryAgentPrivate::_q_nextSdpScan);
    connect(threadFind, &QThread::finished, threadWorkerFind, &ThreadWorkerFind::deleteLater);
    connect(threadFind, &QThread::finished, threadFind, &QThread::deleteLater);
    threadFind->start();
}

QBluetoothServiceDiscoveryAgentPrivate::~QBluetoothServiceDiscoveryAgentPrivate()
{
    if (pendingFinish)
        stop();
    if (threadFind)
        threadFind->quit();
}

void QBluetoothServiceDiscoveryAgentPrivate::start(const QBluetoothAddress &address)
{
    if (!pendingFinish) {
        pendingFinish = true;
        pendingStop = false;

        const auto threadWorker = threadWorkerFind;
        QMetaObject::invokeMethod(threadWorkerFind, [threadWorker, address]()
        {
            const FindServiceResult result = findFirstService(address);
            emit threadWorker->findFinished(QVariant::fromValue(result));
        }, Qt::QueuedConnection);
    }
}

void QBluetoothServiceDiscoveryAgentPrivate::stop()
{
    pendingStop = true;
}

bool QBluetoothServiceDiscoveryAgentPrivate::serviceMatches(const QBluetoothServiceInfo &info)
{
    if (uuidFilter.isEmpty())
        return true;

    if (uuidFilter.contains(info.serviceUuid()))
        return true;

    const QList<QBluetoothUuid> serviceClassUuids = info.serviceClassUuids();
    for (const QBluetoothUuid &uuid : serviceClassUuids)
        if (uuidFilter.contains(uuid))
            return true;

    return false;
}

void QBluetoothServiceDiscoveryAgentPrivate::_q_nextSdpScan(const QVariant &input)
{
    Q_Q(QBluetoothServiceDiscoveryAgent);
    auto result = input.value<FindServiceResult>();

    if (pendingStop) {
        ::WSALookupServiceEnd(result.hSearch);
        pendingStop = false;
        pendingFinish = false;
        emit q->canceled();
    } else {
        if (result.systemError == WSA_E_NO_MORE) {
            result.systemError = NO_ERROR;
        } else if (result.systemError != NO_ERROR) {
            if (result.hSearch != INVALID_HANDLE_VALUE)
                ::WSALookupServiceEnd(result.hSearch);
            error = (result.systemError == ERROR_INVALID_HANDLE) ?
                        QBluetoothServiceDiscoveryAgent::InvalidBluetoothAdapterError
                      : QBluetoothServiceDiscoveryAgent::InputOutputError;
            errorString = qt_error_string(result.systemError);
            qCWarning(QT_BT_WINDOWS) << errorString;
            emit q->error(this->error);
        } else {

            if (serviceMatches(result.info)) {
                result.info.setDevice(discoveredDevices.at(0));
                if (result.info.isValid()) {
                    if (!isDuplicatedService(result.info)) {
                        discoveredServices.append(result.info);
                        emit q->serviceDiscovered(result.info);
                    }
                }
            }

            const auto threadWorker = threadWorkerFind;
            const auto hSearch = result.hSearch;
            QMetaObject::invokeMethod(threadWorkerFind, [threadWorker, hSearch]()
            {
                FindServiceResult result = findNextService(hSearch);
                emit threadWorker->findFinished(QVariant::fromValue(result));
            }, Qt::QueuedConnection);
            return;
        }
        pendingFinish = false;
        _q_serviceDiscoveryFinished();
    }
}

QT_END_NAMESPACE

Q_DECLARE_METATYPE(FindServiceResult)
