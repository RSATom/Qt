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

#include "lottierasterrenderer.h"

#include <QPainter>
#include <QRectF>
#include <QBrush>
#include <QTransform>
#include <QGradient>
#include <QPointer>
#include <QVectorIterator>

#include <QtBodymovin/private/bmshape_p.h>
#include <QtBodymovin/private/bmfill_p.h>
#include <QtBodymovin/private/bmgfill_p.h>
#include <QtBodymovin/private/bmbasictransform_p.h>
#include <QtBodymovin/private/bmshapetransform_p.h>
#include <QtBodymovin/private/bmrect_p.h>
#include <QtBodymovin/private/bmellipse_p.h>
#include <QtBodymovin/private/bmround_p.h>
#include <QtBodymovin/private/bmfreeformshape_p.h>
#include <QtBodymovin/private/bmtrimpath_p.h>
#include <QtBodymovin/private/bmfilleffect_p.h>
#include <QtBodymovin/private/bmrepeater_p.h>

QT_BEGIN_NAMESPACE

LottieRasterRenderer::LottieRasterRenderer(QPainter *painter)
    : m_painter(painter)
{
    m_painter->setPen(QPen(Qt::NoPen));
}

void LottieRasterRenderer::saveState()
{
    qCDebug(lcLottieQtBodymovinRender) << "Save painter state";
    m_painter->save();
    saveTrimmingState();
    m_pathStack.push_back(m_unitedPath);
    m_fillEffectStack.push_back(m_fillEffect);
    m_unitedPath = QPainterPath();
}

void LottieRasterRenderer::restoreState()
{
    qCDebug(lcLottieQtBodymovinRender) << "Restore painter state";
    m_painter->restore();
    restoreTrimmingState();
    m_unitedPath = m_pathStack.pop();
    m_fillEffect = m_fillEffectStack.pop();
}

void LottieRasterRenderer::render(const BMLayer &layer)
{
    qCDebug(lcLottieQtBodymovinRender) << "Layer:" << layer.name()
                                       << "clip layer" << layer.isClippedLayer();

    if (layer.isMaskLayer())
        m_buildingClipRegion = true;
    else if (!m_clipPath.isEmpty()) {
        if (layer.clipMode() == BMLayer::Alpha)
            m_painter->setClipPath(m_clipPath);
        else if (layer.clipMode() == BMLayer::InvertedAlpha) {
            QPainterPath screen;
            screen.addRect(0, 0, m_painter->device()->width(),
                           m_painter->device()->height());
            m_painter->setClipPath(screen - m_clipPath);
        }
        else {
            // Clipping is not applied to paths that have
            // not setting clipping parameters
            m_painter->setClipPath(QPainterPath());
        }
        m_buildingClipRegion = false;
        m_clipPath = QPainterPath();
    }
}

void LottieRasterRenderer::render(const BMRect &rect)
{
    m_painter->save();

    for (int i = 0; i < m_repeatCount; i++) {
        qCDebug(lcLottieQtBodymovinRender) << rect.name()
                                           << rect.position() << rect.size();
        applyRepeaterTransform(i);
        if (trimmingState() == LottieRenderer::Individual) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(rect.path());
            tp.addPath(m_unitedPath);
            m_unitedPath = tp;
        } else if (m_buildingClipRegion) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(rect.path());
            tp.addPath(m_clipPath);
            m_clipPath = tp;
        } else
            m_painter->drawPath(rect.path());
    }

    m_painter->restore();
}

void LottieRasterRenderer::render(const BMEllipse &ellipse)
{
    m_painter->save();

    for (int i = 0; i < m_repeatCount; i++) {
        qCDebug(lcLottieQtBodymovinRender) << "Ellipse:" << ellipse.name()
                                           << ellipse.position()
                                           << ellipse.size();

        applyRepeaterTransform(i);
        if (trimmingState() == LottieRenderer::Individual) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(ellipse.path());
            tp.addPath(m_unitedPath);
            m_unitedPath = tp;
        } else if (m_buildingClipRegion) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(ellipse.path());
            tp.addPath(m_clipPath);
            m_clipPath = tp;
        } else
            m_painter->drawPath(ellipse.path());
    }

    m_painter->restore();
}

void LottieRasterRenderer::render(const BMRound &round)
{
    m_painter->save();

    for (int i = 0; i < m_repeatCount; i++) {
        qCDebug(lcLottieQtBodymovinRender) << "Round:" << round.name()
                                           << round.position() << round.radius();

        if (trimmingState() == LottieRenderer::Individual) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(round.path());
            tp.addPath(m_unitedPath);
            m_unitedPath = tp;
        } else if (m_buildingClipRegion) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(round.path());
            tp.addPath(m_clipPath);
            m_clipPath = tp;
        } else
            m_painter->drawPath(round.path());
    }

    m_painter->restore();
}

void LottieRasterRenderer::render(const BMFill &fill)
{
    qCDebug(lcLottieQtBodymovinRender) << "Fill:" <<fill.name()
                                       << fill.color();

    if (m_fillEffect)
        return;

    m_painter->setBrush(fill.color());
}

void LottieRasterRenderer::render(const BMGFill &gradient)
{
    qCDebug(lcLottieQtBodymovinRender) << "Gradient:" << gradient.name()
                                       << gradient.value();

    if (m_fillEffect)
        return;

    if (gradient.value())
        m_painter->setBrush(*gradient.value());
    else
        qCWarning(lcLottieQtBodymovinRender) << "Gradient:"
                                             << gradient.name()
                                             << "Cannot draw gradient fill";
}

void LottieRasterRenderer::render(const BMStroke &stroke)
{
    qCDebug(lcLottieQtBodymovinRender) << "Stroke:" << stroke.name()
                                       << stroke.pen() << stroke.pen().miterLimit();

    if (m_fillEffect)
        return;

    m_painter->setPen(stroke.pen());
}

void applyBMTransform(QTransform *xf, const BMBasicTransform &bmxf, bool isBMShapeTransform = false)
{
    QPointF pos = bmxf.position();
    qreal rot = bmxf.rotation();
    QPointF sca = bmxf.scale();
    QPointF anc = bmxf.anchorPoint();

    xf->translate(pos.x(), pos.y());

    if (!qFuzzyIsNull(rot))
        xf->rotate(rot);

    if (isBMShapeTransform) {
        const BMShapeTransform &shxf = static_cast<const BMShapeTransform &>(bmxf);
        if (!qFuzzyIsNull(shxf.skew())) {
            QTransform t(shxf.shearX(), shxf.shearY(), 0, -shxf.shearY(), shxf.shearX(), 0, 0, 0, 1);
            t *= QTransform(1, 0, 0, shxf.shearAngle(), 1, 0, 0, 0, 1);
            t *= QTransform(shxf.shearX(), -shxf.shearY(), 0, shxf.shearY(), shxf.shearX(), 0, 0, 0, 1);
            *xf = t * (*xf);
        }
    }

    xf->scale(sca.x(), sca.y());
    xf->translate(-anc.x(), -anc.y());
}

void LottieRasterRenderer::render(const BMBasicTransform &transform)
{
    QTransform t = m_painter->transform();
    applyBMTransform(&t, transform);
    m_painter->setTransform(t);
    m_painter->setOpacity(m_painter->opacity() * transform.opacity());

    qCDebug(lcLottieQtBodymovinRender) << transform.name()
                                       << m_painter->transform()
                                       << "opacity:"  << m_painter->opacity();
}

void LottieRasterRenderer::render(const BMShapeTransform &transform)
{
    qCDebug(lcLottieQtBodymovinRender) << "Shape transform:"  << transform.name()
                                       << "of" << transform.parent()->name();

    QTransform t = m_painter->transform();
    applyBMTransform(&t, transform, true);
    m_painter->setTransform(t);
    m_painter->setOpacity(m_painter->opacity() * transform.opacity());

    qCDebug(lcLottieQtBodymovinRender) << transform.name()
                                   << m_painter->transform()
                                   << m_painter->opacity();
}

void LottieRasterRenderer::render(const BMFreeFormShape &shape)
{
    m_painter->save();

    for (int i = 0; i < m_repeatCount; i ++) {
        qCDebug(lcLottieQtBodymovinRender) << "Render shape:"
                                           << shape.name() << "of"
                                           << shape.parent()->name();
        applyRepeaterTransform(i);
        if (trimmingState() == LottieRenderer::Individual) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(shape.path());
            tp.addPath(m_unitedPath);
            m_unitedPath = tp;
        } else if (m_buildingClipRegion) {
            QTransform t = m_painter->transform();
            QPainterPath tp = t.map(shape.path());
            tp.addPath(m_clipPath);
            m_clipPath = tp;
        } else
            m_painter->drawPath(shape.path());
    }

    m_painter->restore();
}

void LottieRasterRenderer::render(const BMTrimPath &trimPath)
{
    // TODO: Remove "Individual" trimming to the prerendering thread
    // Now it is done in the GUI thread

    m_painter->save();

    for (int i = 0; i < m_repeatCount; i ++) {
        qCDebug(lcLottieQtBodymovinRender) << "Render shape:"
                                           << trimPath.name() << "of"
                                           << trimPath.parent()->name();
        applyRepeaterTransform(i);
        if (!trimPath.simultaneous() && !qFuzzyCompare(0.0, m_unitedPath.length())) {
            qCDebug(lcLottieQtBodymovinRender) << "Render trim path in the GUI thread";
            QPainterPath tr = trimPath.trim(m_unitedPath);
            // Do not use the applied transform, as the transform
            // is already included in m_unitedPath
            m_painter->setTransform(QTransform());
            m_painter->drawPath(tr);
        }
    }

    m_painter->restore();
}

void LottieRasterRenderer::render(const BMFillEffect &effect)
{
    qCDebug(lcLottieQtBodymovinRender) << "Fill:" <<effect.name()
                                       << effect.color();

    m_fillEffect = &effect;
    m_painter->setBrush(m_fillEffect->color());
    m_painter->setOpacity(m_painter->opacity() * m_fillEffect->opacity());
}

void LottieRasterRenderer::render(const BMRepeater &repeater)
{
    qCDebug(lcLottieQtBodymovinRender) << "Repeater:" <<repeater.name()
                                       << "count:" << repeater.copies();

    if (m_repeaterTransform) {
        qCWarning(lcLottieQtBodymovinRender) << "Only one Repeater can be active at a time!";
        return;
    }

    m_repeatCount = repeater.copies();
    m_repeatOffset = repeater.offset();

    // Can store pointer to transform, although the transform
    // is managed by another thread. The object will be available
    // until the frame has been rendered
    m_repeaterTransform = &repeater.transform();

    m_painter->translate(m_repeatOffset * m_repeaterTransform->position().x(),
                         m_repeatOffset * m_repeaterTransform->position().y());
}

void LottieRasterRenderer::applyRepeaterTransform(int instance)
{
    if (!m_repeaterTransform || instance == 0)
        return;

    QTransform t = m_painter->transform();

    QPointF anchors = -m_repeaterTransform->anchorPoint();
    QPointF position = m_repeaterTransform->position();
    QPointF anchoredCenter = anchors + position;

    t.translate(anchoredCenter.x(), anchoredCenter.y());
    t.rotate(m_repeaterTransform->rotation());
    t.scale(m_repeaterTransform->scale().x(),
            m_repeaterTransform->scale().y());
    m_painter->setTransform(t);

    qreal o =m_repeaterTransform->opacityAtInstance(instance);

    m_painter->setOpacity(m_painter->opacity() * o);
}

QT_END_NAMESPACE
