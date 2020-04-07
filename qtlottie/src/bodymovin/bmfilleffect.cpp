/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the lottie-qt module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
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
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "bmfilleffect_p.h"

#include <QJsonObject>
#include <QJsonValue>

#include "bmglobal.h"

QT_BEGIN_NAMESPACE

BMFillEffect::BMFillEffect(const BMFillEffect &other)
    : BMBase(other)
{
    m_color = other.m_color;
    m_opacity = other.m_opacity;
}

BMBase *BMFillEffect::clone() const
{
    return new BMFillEffect(*this);
}

void BMFillEffect::construct(const QJsonObject &definition)
{
    m_type = BM_EFFECT_FILL;

    if (!definition.value(QLatin1String("hd")).toBool(true))
        return;

    QJsonArray properties = definition.value(QLatin1String("ef")).toArray();

    // TODO: Check are property positions really fixed in the effect?

    m_color.construct(properties.at(2).toObject().value(QLatin1String("v")).toObject());
    m_opacity.construct(properties.at(6).toObject().value(QLatin1String("v")).toObject());

    if (!qFuzzyCompare(properties.at(0).toObject().value(QLatin1String("v")).toObject().value(QLatin1String("k")).toDouble(), 0.0))
        qCWarning(lcLottieQtBodymovinParser)<< "BMFillEffect: Property 'Fill mask' not supported";

    if (!qFuzzyCompare(properties.at(1).toObject().value(QLatin1String("v")).toObject().value(QLatin1String("k")).toDouble(), 0.0))
        qCWarning(lcLottieQtBodymovinParser) << "BMFillEffect: Property 'All masks' not supported";

    if (!qFuzzyCompare(properties.at(3).toObject().value(QLatin1String("v")).toObject().value(QLatin1String("k")).toDouble(), 0.0))
        qCWarning(lcLottieQtBodymovinParser) << "BMFillEffect: Property 'Invert' not supported";

    if (!qFuzzyCompare(properties.at(4).toObject().value(QLatin1String("v")).toObject().value(QLatin1String("k")).toDouble(), 0.0))
        qCWarning(lcLottieQtBodymovinParser) << "BMFillEffect: Property 'Horizontal feather' not supported";

    if (!qFuzzyCompare(properties.at(5).toObject().value(QLatin1String("v")).toObject().value(QLatin1String("k")).toDouble(), 0.0))
        qCWarning(lcLottieQtBodymovinParser) << "BMFillEffect: Property 'Vertical feather' not supported";

}

void BMFillEffect::updateProperties(int frame)
{
    m_color.update(frame);
    m_opacity.update(frame);
}

void BMFillEffect::render(LottieRenderer &renderer) const
{
    renderer.render(*this);
}

QColor BMFillEffect::color() const
{
    QVector4D cVec = m_color.value();
    QColor color;
    qreal r = static_cast<qreal>(cVec.x());
    qreal g = static_cast<qreal>(cVec.y());
    qreal b = static_cast<qreal>(cVec.z());
    qreal a = static_cast<qreal>(cVec.w());
    color.setRgbF(r, g, b, a);
    return color;
}

qreal BMFillEffect::opacity() const
{
    return m_opacity.value();
}

QT_END_NAMESPACE
