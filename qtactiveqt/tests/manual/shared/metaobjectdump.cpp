/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "metaobjectdump.h"

#include <QMetaMethod>
#include <QMetaObject>
#include <QTextStream>

QT_USE_NAMESPACE

using rightAlignNumber = QPair<int, int>; // Use as str << rightAlignNumber(value, width)

QTextStream &operator<<(QTextStream &str, const rightAlignNumber &r)
{
    auto oldWidth = str.fieldWidth();
    str.setFieldWidth(r.second);
    auto oldAlignment = str.fieldAlignment();
    str.setFieldAlignment(QTextStream::AlignRight);
    str << r.first;
    str.setFieldAlignment(oldAlignment);
    str.setFieldWidth(oldWidth);
    return str;
}

QTextStream &operator<<(QTextStream &str, const QMetaEnum &me)
{
    const int keyCount = me.keyCount();
    str << me.name() << ' ' << keyCount << " keys";
    if (me.isFlag())
        str << " [flag]";
    if (me.isScoped())
        str << " [scoped]";
    const int maxLogCount = std::min(6, keyCount);
    str << " {";
    for (int k = 0; k < maxLogCount; ++k) {
        if (k)
            str << ", ";
        str << me.key(k) << " = " << me.value(k);
    }
    if (maxLogCount < keyCount)
        str << ",...";
    str << '}';
    return str;
}

QTextStream &operator<<(QTextStream &str, const QMetaClassInfo &mc)
{
    str << '"' << mc.name() << "\": \"" << mc.value() << '"';
    return str;
}

QTextStream &operator<<(QTextStream &str, const QMetaProperty &mp)
{
    str << mp.typeName() << ' ' << mp.name();
    if (mp.isWritable())
        str << " [writable]";
    if (mp.isResettable())
        str << " [resettable]";
    if (mp.isDesignable())
        str << " [designable]";
    if (mp.isStored())
        str << " [stored]";
    if (mp.isUser())
        str << " [user]";
    if (mp.isConstant())
        str << " [constant]";
     if (mp.isFinal())
         str << " [final]";
     if (mp.isRequired())
         str << " [required]";
     if (mp.isFlagType())
         str << " [flag]";
     if (mp.isEnumType())
         str << " [enum " << mp.enumerator().name() << ']';
     if (mp.hasNotifySignal())
         str << " [notify " << mp.notifySignal().name() << ']';
    return str;
}

QTextStream &operator<<(QTextStream &str, const QMetaMethod &m)
{
    switch (m.access()) {
    case QMetaMethod::Private:
        str << "private ";
        break;
    case QMetaMethod::Protected:
        str << "protected ";
        break;
    case QMetaMethod::Public:
        break;
    }
    str << m.typeName() << ' ' << m.methodSignature();
    switch (m.methodType()) {
    case QMetaMethod::Method:
        break;
    case QMetaMethod::Signal:
        str << " [signal]";
        break;
    case QMetaMethod::Slot:
        str << " [slot]";
        break;
    case QMetaMethod::Constructor:
        str << " [ct]";
        break;
    }
    if (auto attributes = m.attributes()) {
        str << " attributes: " << Qt::hex << Qt::showbase << attributes
            << Qt::dec << Qt::noshowbase;
    }
    if (const int count = m.parameterCount()) {
        str << " Parameters: ";
        const auto parameterNames = m.parameterNames();
        const auto parameterTypes = m.parameterTypes();
        for (int p = 0; p < count; ++p) {
            if (p)
                str << ", ";
            str << parameterTypes.at(p) << ' ' << parameterNames.at(p);
        }
    }
    return str;
}

static void formatMetaObject(QTextStream &str, const QMetaObject *mo, const  QByteArray &indent)
{
    str << indent << "--- " << mo->className() << " ---\n";

    const int classInfoOffset = mo->classInfoOffset();
    const int classInfoCount = mo->classInfoCount();
    if (classInfoOffset < classInfoCount) {
        str << indent << "  Class Info of " << mo->className() << ": "
            << classInfoOffset << ".." << classInfoCount << '\n';
        for (int i = classInfoOffset; i < classInfoCount; ++i) {
            str << indent << "    " << rightAlignNumber(i, 3) << ' '
                << mo->classInfo(i) << '\n';
        }
    }

    const int enumOffset = mo->enumeratorOffset();
    const int enumCount = mo->enumeratorCount();
    if (enumOffset < enumCount) {
        str << indent << "  Enums of " << mo->className() << ": " << enumOffset
            << ".." << enumCount << '\n';
        for (int e = enumOffset; e < enumCount; ++e)
            str << indent << "    " << rightAlignNumber(e, 3) << ' ' << mo->enumerator(e) << '\n';
    }

    const int methodOffset = mo->methodOffset();
    const int methodCount = mo->methodCount();
    if (methodOffset < methodCount) {
        str << indent << "  Methods of " << mo->className() << ": " << methodOffset
            << ".." << methodCount << '\n';
        for (int m = methodOffset; m < methodCount; ++m)
            str << indent << "    " << rightAlignNumber(m, 3) << ' ' << mo->method(m) << '\n';
    }

    const int propertyOffset = mo->propertyOffset();
    const int propertyCount = mo-> propertyCount();
    if (propertyOffset < propertyCount) {
        str << indent << "  Properties of " << mo->className() << ": " << propertyOffset
            << ".." << propertyCount << '\n';
        for (int p = propertyOffset; p < propertyCount; ++p)
            str << indent << "    " << rightAlignNumber(p, 3) << ' ' << mo->property(p) << '\n';
    }
}

QTextStream &operator<<(QTextStream &str, const QMetaObject &o)
{
    QVector<const QMetaObject *> klasses;
    for (auto s = &o; s; s = s->superClass())
        klasses.prepend(s);

    QByteArray indent;
    for (auto k : klasses) {
        formatMetaObject(str, k, indent);
        indent += "  ";
    }
    return str;
}
