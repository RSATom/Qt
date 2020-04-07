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

#include "bmtrimpath_p.h"

#include <QtGlobal>
#include <private/qpainterpath_p.h>
#include <private/qbezier_p.h>

#include "bmconstants_p.h"
#include "trimpath_p.h"

BMTrimPath::BMTrimPath()
{
    m_appliedTrim = this;
}

BMTrimPath::BMTrimPath(const QJsonObject &definition, BMBase *parent)
{
    m_appliedTrim = this;

    setParent(parent);
    construct(definition);
}

BMTrimPath::BMTrimPath(const BMTrimPath &other)
    : BMShape(other)
{
    m_start = other.m_start;
    m_end = other.m_end;
    m_offset = other.m_offset;
    m_simultaneous = other.m_simultaneous;
}

BMBase *BMTrimPath::clone() const
{
    return new BMTrimPath(*this);
}

void BMTrimPath::construct(const QJsonObject &definition)
{
    BMBase::parse(definition);
    if (m_hidden)
        return;

    qCDebug(lcLottieQtBodymovinParser) << "BMTrimPath::construct():" << m_name;

    QJsonObject start = definition.value(QLatin1String("s")).toObject();
    start = resolveExpression(start);
    m_start.construct(start);

    QJsonObject end = definition.value(QLatin1String("e")).toObject();
    end = resolveExpression(end);
    m_end.construct(end);

    QJsonObject offset = definition.value(QLatin1String("o")).toObject();
    offset = resolveExpression(offset);
    m_offset.construct(offset);

    int simultaneous = true;
    if (definition.contains(QLatin1String("m"))) {
        simultaneous = definition.value(QLatin1String("m")).toInt();
    }
    m_simultaneous = (simultaneous == 1);

    if (strcmp(qgetenv("QLOTTIE_FORCE_TRIM_MODE"), "simultaneous") == 0) {
        qCDebug(lcLottieQtBodymovinRender) << "Forcing trim mode to Simultaneous";
        m_simultaneous = true;
    } else if (strcmp(qgetenv("QLOTTIE_FORCE_TRIM_MODE"), "individual") == 0) {
        qCDebug(lcLottieQtBodymovinRender) << "Forcing trim mode to Individual";
        m_simultaneous = false;
    }
}

void BMTrimPath::updateProperties(int frame)
{
    m_start.update(frame);
    m_end.update(frame);
    m_offset.update(frame);

    qCDebug(lcLottieQtBodymovinUpdate) << name() << frame << m_start.value()
                                       << m_end.value() << m_offset.value();

    BMShape::updateProperties(frame);
}

void BMTrimPath::render(LottieRenderer &renderer) const
{
    if (m_appliedTrim) {
        if (m_appliedTrim->simultaneous())
            renderer.setTrimmingState(LottieRenderer::Simultaneous);
        else
            renderer.setTrimmingState(LottieRenderer::Individual);
    } else
        renderer.setTrimmingState(LottieRenderer::Off);

    renderer.render(*this);
}

bool BMTrimPath::acceptsTrim() const
{
    return true;
}

void BMTrimPath::applyTrim(const BMTrimPath &other)
{
     qCDebug(lcLottieQtBodymovinUpdate) << "Join trim paths:"
                                        << other.name() << "into:" << name();

    m_name = m_name + QStringLiteral(" & ") + other.name();
    qreal newStart = other.start() + (m_start.value() / 100.0) *
            (other.end() - other.start());
    qreal newEnd = other.start() + (m_end.value() / 100.0) *
            (other.end() - other.start());

    m_start.setValue(newStart);
    m_end.setValue(newEnd);
    m_offset.setValue(m_offset.value() + other.offset());
}

qreal BMTrimPath::start() const
{
    return m_start.value();
}

qreal BMTrimPath::end() const
{
    return m_end.value();
}

qreal BMTrimPath::offset() const
{
    return m_offset.value();
}

bool BMTrimPath::simultaneous() const
{
    return m_simultaneous;
}

QPainterPath BMTrimPath::trim(const QPainterPath &path) const
{
    TrimPath trimmer;
    trimmer.setPath(path);
    qreal offset = m_offset.value() / 360.0;
    qreal start = m_start.value() / 100.0;
    qreal end = m_end.value() / 100.0;
    QPainterPath trimmedPath;
    if (!qFuzzyIsNull(start - end))
        trimmedPath = trimmer.trimmed(start, end, offset);
    return trimmedPath;
}
