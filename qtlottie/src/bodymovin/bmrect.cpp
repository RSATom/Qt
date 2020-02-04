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

#include "bmrect_p.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QLoggingCategory>

#include <QDebug>

#include "bmtrimpath_p.h"

QT_BEGIN_NAMESPACE

BMRect::BMRect(const BMRect &other)
    : BMShape(other)
{
    m_position = other.m_position;
    m_size = other.m_size;
    m_roundness = other.m_roundness;
}

BMRect::BMRect(const QJsonObject &definition, BMBase *parent)
{
    setParent(parent);
    BMBase::parse(definition);
    if (m_hidden)
        return;

    qCDebug(lcLottieQtBodymovinParser) << "BMRect::BMRect():" << m_name;

    QJsonObject position = definition.value(QLatin1String("p")).toObject();
    position = resolveExpression(position);
    m_position.construct(position);

    QJsonObject size = definition.value(QLatin1String("s")).toObject();
    size = resolveExpression(size);
    m_size.construct(size);

    QJsonObject roundness = definition.value(QLatin1String("r")).toObject();
    roundness = resolveExpression(roundness);
    m_roundness.construct(roundness);

    m_direction = definition.value(QLatin1String("d")).toInt();
}


BMBase *BMRect::clone() const
{
    return new BMRect(*this);
}

bool BMRect::setProperty(BMLiteral::PropertyType propertyType, QVariant value)
{
    switch (propertyType) {
    case BMLiteral::RectPosition:
        qCDebug(lcLottieQtBodymovinParser) << "Set position" << value.toPointF();
        m_position.setValue(value.toPointF());
        break;
    default:
        return false;
    }
    return true;
}

void BMRect::updateProperties(int frame)
{
    m_size.update(frame);
    m_position.update(frame);
    m_roundness.update(frame);

    // AE uses center of a shape as it's position,
    // in Qt a translation is needed
    QPointF pos = QPointF(m_position.value().x() - m_size.value().width() / 2,
                             m_position.value().y() - m_size.value().height() / 2);

    m_path = QPainterPath();
    m_path.addRoundedRect(QRectF(pos, m_size.value()),
                               m_roundness.value(), m_roundness.value());

    if (m_direction)
        m_path = m_path.toReversed();
}

void BMRect::render(LottieRenderer &renderer) const
{
    renderer.render(*this);
}

bool BMRect::acceptsTrim() const
{
    return true;
}

QPointF BMRect::position() const
{
    return m_position.value();
}

QSizeF BMRect::size() const
{
    return m_size.value();
}

qreal BMRect::roundness() const
{
    return m_roundness.value();
}

QT_END_NAMESPACE
