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

#include "bmshapetransform_p.h"

#include <QJsonObject>
#include <QtMath>

#include "bmconstants_p.h"
#include "bmbasictransform_p.h"

BMShapeTransform::BMShapeTransform(const BMShapeTransform &other)
    : BMBasicTransform(other)
{
    m_skew = other.m_skew;
    m_skewAxis = other.m_skewAxis;
    m_shearX = other.m_shearX;
    m_shearY = other.m_shearY;
    m_shearAngle = other.m_shearAngle;
}

BMShapeTransform::BMShapeTransform(const QJsonObject &definition, BMBase *parent)
{
    setParent(parent);
    construct(definition);
}

BMBase *BMShapeTransform::clone() const
{
    return new BMShapeTransform(*this);
}

void BMShapeTransform::construct(const QJsonObject &definition)
{
    BMBasicTransform::construct(definition);

    qCDebug(lcLottieQtBodymovinParser) << "BMShapeTransform::construct():" << BMShape::name();

    QJsonObject skew = definition.value(QLatin1String("sk")).toObject();
    skew = resolveExpression(skew);
    m_skew.construct(skew);

    QJsonObject skewAxis = definition.value(QLatin1String("sa")).toObject();
    skewAxis = resolveExpression(skewAxis);
    m_skewAxis.construct(skewAxis);
}

void BMShapeTransform::updateProperties(int frame)
{
    BMBasicTransform::updateProperties(frame);

    m_skew.update(frame);
    m_skewAxis.update(frame);

    double rads = qDegreesToRadians(m_skewAxis.value());
    m_shearX = qCos(rads);
    m_shearY = qSin(rads);
    double tan = qDegreesToRadians(-m_skew.value());
    m_shearAngle = qTan(tan);
}

void BMShapeTransform::render(LottieRenderer &renderer) const
{
    renderer.render(*this);
}

qreal BMShapeTransform::skew() const
{
    return m_skew.value();
}

qreal BMShapeTransform::skewAxis() const
{
    return m_skewAxis.value();
}

qreal BMShapeTransform::shearX() const
{
    return m_shearX;
}

qreal BMShapeTransform::shearY() const
{
    return m_shearY;
}

qreal BMShapeTransform::shearAngle() const
{
    return m_shearAngle;
}
