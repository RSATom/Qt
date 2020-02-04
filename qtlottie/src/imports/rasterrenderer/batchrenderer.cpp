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

#include "batchrenderer.h"

#include <QImage>
#include <QPainter>
#include <QHash>
#include <QMutexLocker>
#include <QLoggingCategory>
#include <QThread>

#include <QJsonDocument>
#include <QJsonArray>

#include <QtBodymovin/private/bmconstants_p.h>
#include <QtBodymovin/private/bmbase_p.h>
#include <QtBodymovin/private/bmlayer_p.h>

#include "lottieanimation.h"
#include "lottierasterrenderer.h"

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcLottieQtBodymovinRenderThread, "qt.lottieqt.bodymovin.render.thread");

BatchRenderer *BatchRenderer::m_rendererInstance = nullptr;

BatchRenderer::~BatchRenderer()
{
    qDeleteAll(m_animData);
    qDeleteAll(m_frameCache);
}

BatchRenderer *BatchRenderer::instance()
{
    if (!m_rendererInstance)
        m_rendererInstance = new BatchRenderer;

    return m_rendererInstance;
}

void BatchRenderer::deleteInstance()
{
    delete m_rendererInstance;
    m_rendererInstance = nullptr;
}

void BatchRenderer::registerAnimator(LottieAnimation *animator)
{
    QMutexLocker mlocker(&m_mutex);

    qCDebug(lcLottieQtBodymovinRenderThread) << "Register Animator:"
                                       << static_cast<void*>(animator);

    Entry *entry = new Entry;
    entry->animator = animator;
    entry->startFrame = animator->startFrame();
    entry->endFrame = animator->endFrame();
    entry->currentFrame = animator->startFrame();
    if (animator->direction() == LottieAnimation::Reverse)
        entry->animDir = -1;
    // animDir == 1 by default
    entry->bmTreeBlueprint = new BMBase;
    parse(entry->bmTreeBlueprint, animator->jsonSource());
    m_animData.insert(animator, entry);
    m_waitCondition.wakeAll();
}

void BatchRenderer::deregisterAnimator(LottieAnimation *animator)
{
    QMutexLocker mlocker(&m_mutex);

    qCDebug(lcLottieQtBodymovinRenderThread) << "Deregister Animator:"
                                       << static_cast<void*>(animator);

    Entry *entry = m_animData.value(animator, nullptr);
    if (entry) {
        qDeleteAll(entry->frameCache);
        delete entry->bmTreeBlueprint;
        delete entry;
        m_animData.remove(animator);
    }
}

bool BatchRenderer::gotoFrame(LottieAnimation *animator, int frame)
{
    Entry *entry = m_animData.value(animator, nullptr);
    if (entry) {
        QMutexLocker mlocker(&m_mutex);
        qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:"
                                           << static_cast<void*>(animator)
                                           << "Goto frame:" << frame;
        entry->currentFrame = frame;
        pruneFrameCache(entry);
        m_waitCondition.wakeAll();
        return true;
    }
    return false;
}

void BatchRenderer::pruneFrameCache(Entry* e)
{
    QHash<int, BMBase*>::iterator it = e->frameCache.begin();

    while (it != e->frameCache.end()) {
        if (it.key() == e->currentFrame) {
            ++it;
        } else {
            delete it.value();
            it = e->frameCache.erase(it);
        }
    }
}

BMBase *BatchRenderer::getFrame(LottieAnimation *animator, int frameNumber)
{
    QMutexLocker mlocker(&m_mutex);

    Entry *entry = m_animData.value(animator, nullptr);
    if (entry)
        return entry->frameCache.value(frameNumber, nullptr);
    else
        return nullptr;
}

void BatchRenderer::setCacheSize(int size)
{
    if (size < 1 || isRunning())
        return;

    qCDebug(lcLottieQtBodymovinRenderThread) << "Setting frame cache size to" << size;
    m_cacheSize = size;
}

void BatchRenderer::prerender(Entry *animEntry)
{
    LottieAnimation *animator = animEntry->animator;

    if (animEntry->frameCache.count() == m_cacheSize) {
        qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:" << static_cast<void*>(animEntry->animator)
                                                        << "Cache full, cannot render more";
        return;
    }

    while (animEntry->frameCache.count() < m_cacheSize) {
        // It may be that the animator has deregistered itself while
        // te mutex was locked. In that case we cannot render here anymore
        if (!animEntry->bmTreeBlueprint)
            break;

        if (!animEntry->frameCache.contains(animEntry->currentFrame)) {
            BMBase *bmTree = new BMBase(*animEntry->bmTreeBlueprint);

            for (BMBase *elem : bmTree->children()) {
                if (elem->active(animEntry->currentFrame))
                    elem->updateProperties( animEntry->currentFrame);
            }

            animEntry->frameCache.insert( animEntry->currentFrame, bmTree);
        }

        qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:"
                                           << static_cast<void*>(animEntry->animator)
                                           << "Frame drawn to cache. FN:"
                                           << animEntry->currentFrame;
        emit frameReady(animator,  animEntry->currentFrame);

        animEntry->currentFrame += animEntry->animDir;

        if (animEntry->currentFrame > animEntry->endFrame) {
            animEntry->currentFrame = animEntry->startFrame;
        } else if (animEntry->currentFrame < animEntry->startFrame) {
            animEntry->currentFrame = animEntry->endFrame;
        }
    }
}

void BatchRenderer::prerender()
{
    QMutexLocker mlocker(&m_mutex);
    bool wait = true;

    foreach (Entry *e, m_animData) {
        if (e->frameCache.size() < m_cacheSize) {
            wait = false;
            break;
        }
    }

    if (wait)
        m_waitCondition.wait(&m_mutex);

    QHash<LottieAnimation*, Entry*>::iterator it = m_animData.begin();
    while (it != m_animData.end()) {
        Entry *e = *it;
        if (e && e->frameCache.size() < m_cacheSize)
            prerender(e);
        ++it;
    }
}

void BatchRenderer::frameRendered(LottieAnimation *animator, int frameNumber)
{
    Entry *entry = m_animData.value(animator, nullptr);
    if (entry) {
        qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:" << static_cast<void*>(animator)
                                           << "Remove frame from cache" << frameNumber;

        QMutexLocker mlocker(&m_mutex);
        BMBase *root = entry->frameCache.value(frameNumber, nullptr);
        delete root;
        entry->frameCache.remove(frameNumber);
        m_waitCondition.wakeAll();
    }
}

void BatchRenderer::run()
{
    qCDebug(lcLottieQtBodymovinRenderThread) << "rendering thread" << QThread::currentThread();

    while (-1) {
        if (QThread::currentThread()->isInterruptionRequested())
            return;

        prerender();
    }
}

int BatchRenderer::parse(BMBase* rootElement, QByteArray jsonSource)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonSource);
    QJsonObject rootObj = doc.object();

    if (rootObj.empty())
        return -1;

    QJsonArray jsonLayers = rootObj.value(QLatin1String("layers")).toArray();
    QJsonArray::const_iterator jsonLayerIt = jsonLayers.constEnd();
    while (jsonLayerIt != jsonLayers.constBegin()) {
        jsonLayerIt--;
        QJsonObject jsonLayer = (*jsonLayerIt).toObject();
        BMLayer *layer = BMLayer::construct(jsonLayer);
        if (layer) {
            layer->setParent(rootElement);
            rootElement->addChild(layer);
        }
    }

    // Mask layers must be rendered before the layers they affect to
    // although they appear before in layer hierarchy. For this reason
    // move a mask after the affected layers, so it will be rendered first
    QList<BMBase *> &layers = rootElement->children();
    int moveTo = -1;
    for (int i = 0; i < layers.count(); i++) {
        BMLayer *layer = static_cast<BMLayer*>(layers.at(i));
        if (layer->isClippedLayer())
            moveTo = i;
        if (layer->isMaskLayer()) {
            qCDebug(lcLottieQtBodymovinParser()) << "Move mask layer"
                                                 <<  layers.at(i)->name()
                                                 << "before" << layers.at(moveTo)->name();
            layers.move(i, moveTo);
        }
    }

    return 0;
}

QT_END_NAMESPACE
