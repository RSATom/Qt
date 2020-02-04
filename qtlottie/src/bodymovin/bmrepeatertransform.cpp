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

#include "bmrepeatertransform_p.h"

QT_BEGIN_NAMESPACE

BMRepeaterTransform::BMRepeaterTransform(const BMRepeaterTransform &other)
    : BMBasicTransform(other)
{
    m_startOpacity = other.m_startOpacity;
    m_endOpacity = other.m_endOpacity;
    m_opacities = other.m_opacities;
}

BMRepeaterTransform::BMRepeaterTransform(const QJsonObject &definition, BMBase *parent)
{
    setParent(parent);
    construct(definition);
}

BMBase *BMRepeaterTransform::clone() const
{
    return new BMRepeaterTransform(*this);
}

void BMRepeaterTransform::construct(const QJsonObject &definition)
{
    qCDebug(lcLottieQtBodymovinParser) << "BMRepeaterTransform::construct():" << name();

    BMBasicTransform::construct(definition);
    if (m_hidden)
        return;

    QJsonObject startOpacity = definition.value(QLatin1String("so")).toObject();
    startOpacity = resolveExpression(startOpacity);
    m_startOpacity.construct(startOpacity);

    QJsonObject endOpacity = definition.value(QLatin1String("eo")).toObject();
    endOpacity = resolveExpression(endOpacity);
    m_endOpacity.construct(endOpacity);
}

void BMRepeaterTransform::updateProperties(int frame)
{
    BMBasicTransform::updateProperties(frame);

    m_startOpacity.update(frame);
    m_endOpacity.update(frame);

    m_opacities.clear();
    for (int i = 0; i < m_copies; i++) {
        qreal opacity = m_startOpacity.value() +
                (m_endOpacity.value() - m_startOpacity.value()) * i / m_copies;
        m_opacities.push_back(opacity);
    }
}

void BMRepeaterTransform::render(LottieRenderer &renderer) const
{
    renderer.render(*this);
}

void BMRepeaterTransform::setInstanceCount(int copies)
{
    m_copies = copies;
}

qreal BMRepeaterTransform::opacityAtInstance(int instance) const
{
    return m_opacities.at(instance) / 100.0;
}

qreal BMRepeaterTransform::startOpacity() const
{
    return m_startOpacity.value();
}

qreal BMRepeaterTransform::endOpacity() const
{
    return m_endOpacity.value();
}

QT_END_NAMESPACE
