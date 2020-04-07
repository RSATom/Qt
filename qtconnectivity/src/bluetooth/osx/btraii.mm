/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "btraii_p.h"

#include <qdebug.h>

#include <Foundation/Foundation.h>

#include <utility>

QT_BEGIN_NAMESPACE

namespace DarwinBluetooth {

StrongReference::StrongReference(void *object, RetainPolicy policy)
    : objCInstance(object)
{
    if (policy == RetainPolicy::doInitialRetain)
        objCInstance = [getAs<NSObject>() retain];
}

StrongReference::StrongReference(const StrongReference &other)
{
    objCInstance = [other.getAs<NSObject>() retain];
}

StrongReference::StrongReference(StrongReference &&other)
{
    std::swap(objCInstance, other.objCInstance);
}

StrongReference::~StrongReference()
{
    [getAs<NSObject>() release];
}

StrongReference &StrongReference::operator = (const StrongReference &other) noexcept
{
    if (this != &other) {
        [getAs<NSObject>() release];
        objCInstance = [other.getAs<NSObject>() retain];
    }

    return *this;
}

StrongReference &StrongReference::operator = (StrongReference &&other) noexcept
{
    swap(other);
    return *this;
}

void StrongReference::reset()
{
    [getAs<NSObject>() release];
    objCInstance = nullptr;
}

void StrongReference::reset(void *obj, RetainPolicy policy)
{
    [getAs<NSObject>() release];
    objCInstance = obj;

    if (policy == RetainPolicy::doInitialRetain) {
        auto newInstance = static_cast<NSObject *>(obj);
        Q_ASSERT(newInstance);
        objCInstance = [newInstance retain];
    }
}

} // namespace DarwinBluetooth

QT_END_NAMESPACE
