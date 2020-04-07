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

#include "bmround_p.h"

#include <QJsonObject>

#include "bmtrimpath_p.h"

QT_BEGIN_NAMESPACE

BMRound::BMRound(const BMRound &other)
    : BMShape(other)
{
    m_position = other.m_position;
    m_radius = other.m_radius;
}

BMRound::BMRound(const QJsonObject &definition, BMBase *parent)
{
    setParent(parent);
    construct(definition);
}

BMBase *BMRound::clone() const
{
    return new BMRound(*this);
}

void BMRound::construct(const QJsonObject &definition)
{
    BMBase::parse(definition);
    if (m_hidden)
        return;

    qCDebug(lcLottieQtBodymovinParser) << "BMRound::construct():" << m_name;

    QJsonObject position = definition.value(QLatin1String("p")).toObject();
    position = resolveExpression(position);
    m_position.construct(position);

    QJsonObject radius = definition.value(QLatin1String("r")).toObject();
    radius = resolveExpression(radius);
    m_radius.construct(radius);
}

void BMRound::updateProperties(int frame)
{
    m_position.update(frame);
    m_radius.update(frame);

    // AE uses center of a shape as it's position,
    // in Qt a translation is needed
    QPointF center = QPointF(m_position.value().x() - m_radius.value() / 2,
                             m_position.value().y() - m_radius.value() / 2);

    m_path = QPainterPath();
    m_path.arcMoveTo(QRectF(center,
                            QSizeF(m_radius.value(), m_radius.value())), 90);
    m_path.arcTo(QRectF(center,
                        QSizeF(m_radius.value(), m_radius.value())), 90, -360);

    if (m_direction)
        m_path = m_path.toReversed();
}

void BMRound::render(LottieRenderer &renderer) const
{
    renderer.render(*this);
}

bool BMRound::acceptsTrim() const
{
    return true;
}

QPointF BMRound::position() const
{
    return m_position.value();
}

qreal BMRound::radius() const
{
    return m_radius.value();
}

QT_END_NAMESPACE
