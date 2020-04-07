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

#include "bmgfill_p.h"

#include <QLinearGradient>
#include <QRadialGradient>
#include <QtMath>
#include <QColor>

QT_BEGIN_NAMESPACE

BMGFill::BMGFill(const BMGFill &other)
    : BMShape(other)
{
    m_opacity = other.m_opacity;
    m_startPoint = other.m_startPoint;
    m_endPoint = other.m_endPoint;
    m_highlightLength = other.m_highlightLength;
    m_highlightAngle = other.m_highlightAngle;
    m_colors = other.m_colors;
    if (other.gradientType() == QGradient::LinearGradient)
        m_gradient = new QLinearGradient;
    else if (other.gradientType() == QGradient::RadialGradient)
        m_gradient = new QRadialGradient;
    else {
        Q_UNREACHABLE();
    }
}

BMGFill::~BMGFill()
{
    if (m_gradient)
        delete m_gradient;
}

BMBase *BMGFill::clone() const
{
    return new BMGFill(*this);
}

BMGFill::BMGFill(const QJsonObject &definition, BMBase *parent)
{
    setParent(parent);

    BMBase::parse(definition);
    if (m_hidden)
        return;

    qCDebug(lcLottieQtBodymovinParser) << "BMGFill::construct():" << m_name;

    int type = definition.value(QLatin1String("t")).toVariant().toInt();
    switch (type) {
    case 1:
        m_gradient = new QLinearGradient;
        break;
    case 2:
        m_gradient = new QRadialGradient;
        break;
    default:
        qCWarning(lcLottieQtBodymovinParser) << "Unknown gradient fill type";
    }

    QJsonObject color = definition.value(QLatin1String("g")).toObject();
    QJsonArray colorArr = color.value(QLatin1String("k")).toObject().value(QLatin1String("k")).toArray();
    int elementCount = color.value(QLatin1String("p")).toInt();
    for (int i = 0; i < (elementCount) * 4; i += 4) {
        // p denotes the color stop percentage
        QVector4D colorVec;
        colorVec[0] = colorArr[i + 1].toVariant().toFloat();
        colorVec[1] = colorArr[i + 2].toVariant().toFloat();
        colorVec[2] = colorArr[i + 3].toVariant().toFloat();
        // Set gradient stop position into w of the vector
        colorVec[3] = colorArr[i + 0].toVariant().toFloat();
        BMProperty4D<QVector4D> colorPos;
        colorPos.setValue(colorVec);
        m_colors.push_back(colorPos);
    }

    QJsonObject opacity = definition.value(QLatin1String("o")).toObject();
    opacity = resolveExpression(opacity);
    m_opacity.construct(opacity);

    QJsonObject startPoint = definition.value(QLatin1String("s")).toObject();
    startPoint = resolveExpression(startPoint);
    m_startPoint.construct(startPoint);

    QJsonObject endPoint = definition.value(QLatin1String("e")).toObject();
    endPoint = resolveExpression(endPoint);
    m_endPoint.construct(endPoint);

    QJsonObject highlight = definition.value(QLatin1String("h")).toObject();
    m_highlightLength.construct(highlight);

    QJsonObject angle = definition.value(QLatin1String("a")).toObject();
    angle = resolveExpression(angle);
    m_highlightAngle.construct(angle);

    m_highlightAngle.setValue(0.0);
}

void BMGFill::updateProperties(int frame)
{
    QGradient::Type type = gradientType();
    if (type != QGradient::LinearGradient &&
        type != QGradient::RadialGradient)
        return;

    m_startPoint.update(frame);
    m_endPoint.update(frame);
    m_highlightLength.update(frame);
    m_highlightAngle.update(frame);
    m_opacity.update(frame);
    QList<BMProperty4D<QVector4D>>::iterator colorIt = m_colors.begin();
    while (colorIt != m_colors.end()) {
        (*colorIt).update(frame);
        ++colorIt;
    }

    setGradient();
}

void BMGFill::render(LottieRenderer &renderer) const
{
    renderer.render(*this);
}

QGradient *BMGFill::value() const
{
    return m_gradient;
}

QGradient::Type BMGFill::gradientType() const
{
    if (m_gradient)
        return m_gradient->type();
    else
        return QGradient::NoGradient;
}

QPointF BMGFill::startPoint() const
{
    return m_startPoint.value();
}

QPointF BMGFill::endPoint() const
{
    return m_endPoint.value();
}

qreal BMGFill::highlightLength() const
{
    return m_highlightLength.value();
}

qreal BMGFill::highlightAngle() const
{
    return m_highlightAngle.value();
}

qreal BMGFill::opacity() const
{
    return m_opacity.value();
}

void BMGFill::setGradient()
{
    QList<BMProperty4D<QVector4D>>::iterator colorIt = m_colors.begin();
    while (colorIt != m_colors.end()) {
        QVector4D colorPos = (*colorIt).value();
        QColor color;
        color.setRedF(static_cast<qreal>(colorPos[0]));
        color.setGreenF(static_cast<qreal>(colorPos[1]));
        color.setBlueF(static_cast<qreal>(colorPos[2]));
        color.setAlphaF(m_opacity.value() / 100.0);
        m_gradient->setColorAt(static_cast<qreal>(colorPos[3]),
                               color);
        ++colorIt;
    }

    switch (gradientType()) {
    case QGradient::LinearGradient:
    {
        QLinearGradient *g = static_cast<QLinearGradient*>(m_gradient);
        g->setStart(m_startPoint.value());
        g->setFinalStop(m_endPoint.value());
        break;
    }
    case QGradient::RadialGradient:
    {
        QRadialGradient *g = static_cast<QRadialGradient*>(m_gradient);
        qreal dx = qAbs(m_endPoint.value().x() + m_startPoint.value().x());
        qreal dy = qAbs(m_endPoint.value().y() + m_startPoint.value().y());
        qreal radius = qSqrt(dx * dx +  dy * dy);
        qreal angle = qAsin(dy / radius);
        g->setCenter(m_startPoint.value());
        g->setCenterRadius(radius);
        qreal focusRadius = 2;
        qreal x = (g->radius() - 2 * focusRadius) * qCos(angle + qDegreesToRadians(m_highlightAngle.value()));
        qreal y = (g->radius() - 2 * focusRadius) * qSin(angle + qDegreesToRadians(m_highlightAngle.value()));
        g->setFocalPoint(g->center() + QPointF(x, y));
        g->setFocalRadius(focusRadius);
        break;
    }
    default:
        break;
    }
}

QT_END_NAMESPACE
