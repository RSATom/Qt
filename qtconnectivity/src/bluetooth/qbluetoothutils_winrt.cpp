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

#include "qbluetoothutils_winrt_p.h"
#include <QtBluetooth/private/qtbluetoothglobal_p.h>

#include <QtCore/qfunctions_winrt.h>

#include <robuffer.h>
#include <wrl.h>
#include <windows.foundation.metadata.h>
#include <windows.storage.streams.h>

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using namespace ABI::Windows::Foundation::Metadata;
using namespace ABI::Windows::Storage::Streams;

QT_BEGIN_NAMESPACE

bool supportsNewLEApi()
{
    static bool initialized = false;
    static boolean apiPresent = false;
    if (initialized)
        return apiPresent;

    initialized = true;
#if !QT_CONFIG(winrt_btle_no_pairing)
    return apiPresent;
#endif

    ComPtr<IApiInformationStatics> apiInformationStatics;
    HRESULT hr = RoGetActivationFactory(HString::MakeReference(RuntimeClass_Windows_Foundation_Metadata_ApiInformation).Get(),
                                IID_PPV_ARGS(&apiInformationStatics));
    if (FAILED(hr))
        return apiPresent;

    const HStringReference valueRef(L"Windows.Foundation.UniversalApiContract");
    hr = apiInformationStatics->IsApiContractPresentByMajor(
                valueRef.Get(), 4, &apiPresent);
    apiPresent = SUCCEEDED(hr) && apiPresent;
    return apiPresent;
}

QByteArray byteArrayFromBuffer(const ComPtr<NativeBuffer> &buffer, bool isWCharString)
{
    if (!buffer) {
        qErrnoWarning("nullptr passed to byteArrayFromBuffer");
        return QByteArray();
    }
    ComPtr<Windows::Storage::Streams::IBufferByteAccess> byteAccess;
    HRESULT hr = buffer.As(&byteAccess);
    RETURN_IF_FAILED("Could not cast buffer", return QByteArray())
    char *data;
    hr = byteAccess->Buffer(reinterpret_cast<byte **>(&data));
    RETURN_IF_FAILED("Could not obtain buffer data", return QByteArray())
    UINT32 size;
    hr = buffer->get_Length(&size);
    RETURN_IF_FAILED("Could not obtain buffer size", return QByteArray())
    if (isWCharString) {
        QString valueString = QString::fromUtf16(reinterpret_cast<ushort *>(data)).left(size / 2);
        return valueString.toUtf8();
    }
    return QByteArray(data, qint32(size));
}

QT_END_NAMESPACE
