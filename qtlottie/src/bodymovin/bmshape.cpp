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

#include "bmshape_p.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QLoggingCategory>

#include "bmgroup_p.h"
#include "bmfill_p.h"
#include "bmgfill_p.h"
#include "bmstroke_p.h"
#include "bmrect_p.h"
#include "bmellipse_p.h"
#include "bmround_p.h"
#include "bmtrimpath_p.h"
#include "bmshapetransform_p.h"
#include "bmfreeformshape_p.h"
#include "bmrepeater_p.h"
#include "bmconstants_p.h"

QT_BEGIN_NAMESPACE

const QMap<QLatin1String, int> BMShape::m_shapeMap =
        BMShape::setShapeMap();


BMShape::BMShape(const BMShape &other)
    : BMBase(other)
{
    m_direction = other.m_direction;
    m_path = other.m_path;
    m_appliedTrim = other.m_appliedTrim;
}

BMBase *BMShape::clone() const
{
    return new BMShape(*this);
}

QMap<QLatin1String, int> BMShape::setShapeMap()
{
    QMap<QLatin1String, int> shapeMap;
    shapeMap.insert(QLatin1String(BM_SHAPE_ELLIPSE_STR), BM_SHAPE_ELLIPSE_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_FILL_STR), BM_SHAPE_FILL_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_GFILL_STR), BM_SHAPE_GFILL_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_GSTROKE_STR), BM_SHAPE_GSTROKE_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_GROUP_STR), BM_SHAPE_GROUP_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_RECT_STR), BM_SHAPE_RECT_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_ROUND_STR), BM_SHAPE_ROUND_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_SHAPE_STR), BM_SHAPE_SHAPE_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_STAR_STR), BM_SHAPE_STAR_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_STROKE_STR), BM_SHAPE_STROKE_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_TRIM_STR), BM_SHAPE_TRIM_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_TRANSFORM_STR), BM_SHAPE_TRANS_IX);
    shapeMap.insert(QLatin1String(BM_SHAPE_REPEATER_STR), BM_SHAPE_REPEATER_IX);
    return shapeMap;
}

BMShape *BMShape::construct(QJsonObject definition, BMBase *parent, int constructAs)
{
    qCDebug(lcLottieQtBodymovinParser) << "BMShape::construct()";

    BMShape *shape = nullptr;
    QByteArray type = definition.value(QLatin1String("ty")).toVariant().toByteArray();

    int typeToBuild = m_shapeMap.value(QLatin1String(type.data()), -1);

    if (constructAs != BM_SHAPE_ANY_TYPE_IX)
        typeToBuild = constructAs;

    switch (typeToBuild) {
    case BM_SHAPE_GROUP_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse group";
        shape = new BMGroup(definition, parent);
        shape->setType(BM_SHAPE_GROUP_IX);
        break;
    }
    case BM_SHAPE_RECT_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse m_rect";
        shape = new BMRect(definition, parent);
        shape->setType(BM_SHAPE_RECT_IX);
        break;
    }
    case BM_SHAPE_FILL_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse fill";
        shape = new BMFill(definition, parent);
        shape->setType(BM_SHAPE_FILL_IX);
        break;
    }
    case BM_SHAPE_GFILL_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse group fill";
        shape = new BMGFill(definition, parent);
        shape->setType(BM_SHAPE_GFILL_IX);
        break;
    }
    case BM_SHAPE_STROKE_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse stroke";
        shape = new BMStroke(definition, parent);
        shape->setType(BM_SHAPE_STROKE_IX);
        break;
    }
    case BM_SHAPE_TRANS_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse shape transform";
        shape = new BMShapeTransform(definition, parent);
        shape->setType(BM_SHAPE_TRANS_IX);
        break;
    }
    case BM_SHAPE_ELLIPSE_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse ellipse";
        shape = new BMEllipse(definition);
        shape->setType(BM_SHAPE_ELLIPSE_IX);
        break;
    }
    case BM_SHAPE_ROUND_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse round";
        shape = new BMRound(definition, parent);
        shape->setType(BM_SHAPE_ELLIPSE_IX);
        break;
    }
    case BM_SHAPE_SHAPE_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse shape";
        shape = new BMFreeFormShape(definition, parent);
        shape->setType(BM_SHAPE_SHAPE_IX);
        break;
    }
    case BM_SHAPE_TRIM_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse trim path";
        shape = new BMTrimPath(definition, parent);
        shape->setType(BM_SHAPE_TRIM_IX);
        break;
    }
    case BM_SHAPE_REPEATER_IX:
    {
        qCDebug(lcLottieQtBodymovinParser) << "Parse trim path";
        shape = new BMRepeater(definition, parent);
        shape->setType(BM_SHAPE_REPEATER_IX);
        break;
    }
    default:
        qCWarning(lcLottieQtBodymovinParser) << "Unsupported shape type:"
                                             << type;
    }
    return shape;
}

bool BMShape::acceptsTrim() const
{
    return false;
}

void BMShape::applyTrim(const BMTrimPath &trimmer)
{
    if (trimmer.simultaneous())
        m_path = trimmer.trim(m_path);
}

int BMShape::direction() const
{
    return m_direction;
}

const QPainterPath &BMShape::path() const
{
    return m_path;
}

QT_END_NAMESPACE
