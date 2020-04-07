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

#include "bmlayer_p.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLoggingCategory>

#include "bmshapelayer_p.h"
#include "bmfilleffect_p.h"
#include "bmbasictransform_p.h"

QT_BEGIN_NAMESPACE

BMLayer::BMLayer(const BMLayer &other)
    : BMBase(other)
{
    m_layerIndex = other.m_layerIndex;
    m_startFrame = other.m_startFrame;
    m_endFrame = other.m_endFrame;
    m_startTime = other.m_startTime;
    m_blendMode = other.m_blendMode;
    m_3dLayer = other.m_3dLayer;
    m_stretch = other.m_stretch;
    m_parentLayer = other.m_parentLayer;
    m_td = other.m_td;
    m_clipMode = other.m_clipMode;
    if (other.m_effects) {
        m_effects = new BMBase;
        for (BMBase *effect : other.m_effects->children())
            m_effects->addChild(effect->clone());
    }
    //m_transformAtFirstFrame = other.m_transformAtFirstFrame;
}

BMLayer::~BMLayer()
{
    if (m_effects)
        delete m_effects;
}

BMBase *BMLayer::clone() const
{
    return new BMLayer(*this);
}

BMLayer *BMLayer::construct(QJsonObject definition)
{
    qCDebug(lcLottieQtBodymovinParser) << "BMLayer::construct()";

    BMLayer *layer = nullptr;
    int type = definition.value(QLatin1String("ty")).toInt();
    switch (type) {
    case 4:
        qCDebug(lcLottieQtBodymovinParser) << "Parse shape layer";
        layer = new BMShapeLayer(definition);
        break;
    default:
        qCWarning(lcLottieQtBodymovinParser) << "Unsupported layer type:" << type;
    }
    return layer;
}

bool BMLayer::active(int frame) const
{
    return (!m_hidden && (frame >= m_startFrame && frame <= m_endFrame));
}

void BMLayer::parse(const QJsonObject &definition)
{
    BMBase::parse(definition);
    if (m_hidden)
        return;

    qCDebug(lcLottieQtBodymovinParser) << "BMLayer::parse():" << m_name;

    m_layerIndex = definition.value(QLatin1String("ind")).toVariant().toInt();
    m_startFrame = definition.value(QLatin1String("ip")).toVariant().toInt();
    m_endFrame = definition.value(QLatin1String("op")).toVariant().toInt();
    m_startTime = definition.value(QLatin1String("st")).toVariant().toReal();
    m_blendMode = definition.value(QLatin1String("bm")).toVariant().toInt();
    m_autoOrient = definition.value(QLatin1String("ao")).toBool();
    m_3dLayer = definition.value(QLatin1String("ddd")).toBool();
    m_stretch = definition.value(QLatin1String("sr")).toVariant().toReal();
    m_parentLayer = definition.value(QLatin1String("parent")).toVariant().toInt();
    m_td = definition.value(QLatin1String("td")).toInt();
    int clipMode = definition.value(QLatin1String("tt")).toInt(-1);
    if (clipMode > -1 && clipMode < 5)
        m_clipMode = static_cast<MatteClipMode>(clipMode);

    QJsonArray effects = definition.value(QLatin1String("ef")).toArray();
    parseEffects(effects);

    if (m_td > 1)
        qCWarning(lcLottieQtBodymovinParser)
                << "BM Layer: Only alpha mask layer supported:" << m_clipMode;
    if (m_blendMode > 0)
        qCWarning(lcLottieQtBodymovinParser)
                << "BM Layer: Unsupported blend mode" << m_blendMode;
    if (m_stretch > 1)
        qCWarning(lcLottieQtBodymovinParser)
                << "BM Layer: stretch not supported" << m_stretch;
    if (m_autoOrient)
        qCWarning(lcLottieQtBodymovinParser)
                << "BM Layer: auto-orient not supported";
    if (m_3dLayer)
        qCWarning(lcLottieQtBodymovinParser)
                << "BM Layer: is a 3D layer, but not handled";
}

void BMLayer::updateProperties(int frame)
{
    if (m_parentLayer)
        resolveLinkedLayer();

    // Update first effects, as they are not children of the layer
    if (m_effects) {
        for (BMBase* effect : m_effects->children())
            effect->updateProperties(frame);
    }

    BMBase::updateProperties(frame);
}

void BMLayer::render(LottieRenderer &renderer) const
{
    // Render first effects, as they affect the children
    renderEffects(renderer);

    BMBase::render(renderer);
}

BMBase *BMLayer::findChild(const QString &childName)
{
    BMBase *child = nullptr;

    if (m_effects)
        child = m_effects->findChild(childName);

    if (child)
        return child;
    else
        return BMBase::findChild(childName);
}

BMLayer *BMLayer::resolveLinkedLayer()
{
    if (m_linkedLayer)
        return m_linkedLayer;

    resolveTopRoot();

    Q_ASSERT(topRoot());

    for (BMBase *child : topRoot()->children()) {
        BMLayer *layer = static_cast<BMLayer*>(child);
        if (layer->layerId() == m_parentLayer) {
            m_linkedLayer = layer;
            break;
        }
    }
    return m_linkedLayer;
}

BMLayer *BMLayer::linkedLayer() const
{
    return m_linkedLayer;
}

bool BMLayer::isClippedLayer() const
{
    return m_clipMode != NoClip;
}

bool BMLayer::isMaskLayer() const
{
    return m_td > 0;
}

BMLayer::MatteClipMode BMLayer::clipMode() const
{
    return m_clipMode;
}

int BMLayer::layerId() const
{
    return m_layerIndex;
}

BMBasicTransform *BMLayer::transform() const
{
    return m_layerTransform;
}

void BMLayer::renderEffects(LottieRenderer &renderer) const
{
    if (!m_effects)
        return;

    for (BMBase* effect : m_effects->children()) {
        if (effect->hidden())
            continue;
        effect->render(renderer);
    }
}

void BMLayer::parseEffects(const QJsonArray &definition, BMBase *effectRoot)
{
    QJsonArray::const_iterator it = definition.constEnd();
    while (it != definition.constBegin()) {
        // Create effects container if at least one effect found
        if (!m_effects) {
            m_effects = new BMBase;
            effectRoot = m_effects;
        }
        it--;
        QJsonObject effect = (*it).toObject();
        int type = effect.value(QLatin1String("ty")).toInt();
        switch (type) {
        case 0:
        {
            BMBase *slider = new BMBase;
            slider->parse(effect);
            effectRoot->addChild(slider);
            break;
        }
        case 5:
        {
            if (effect.value(QLatin1String("en")).toInt()) {
                BMBase *group = new BMBase;
                group->parse(effect);
                effectRoot->addChild(group);
                parseEffects(effect.value(QLatin1String("ef")).toArray(), group);
            }
            break;
        }
        case 21:
        {
            BMFillEffect *fill = new BMFillEffect;
            fill->construct(effect);
            effectRoot->addChild(fill);
            break;
        }
        default:
            qCWarning(lcLottieQtBodymovinParser)
                << "BMLayer: Unsupported effect" << type;
        }
    }
}

QT_END_NAMESPACE
