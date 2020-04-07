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

BatchRenderer::BatchRenderer()
    : QThread()
{
    const QByteArray cacheStr = qgetenv("QLOTTIE_RENDER_CACHE_SIZE");
    int cacheSize = cacheStr.toInt();
    if (cacheSize > 0) {
        qCDebug(lcLottieQtBodymovinRenderThread) << "Setting frame cache size to" << cacheSize;
        m_cacheSize = cacheSize;
    }
}

BatchRenderer::~BatchRenderer()
{
    QMutexLocker mlocker(&m_mutex);

    qDeleteAll(m_animData);
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

    Entry *&entry = m_animData[animator];
    Q_ASSERT(entry == nullptr);
    entry = new Entry;
    entry->animator = animator;
    entry->startFrame = animator->startFrame();
    entry->endFrame = animator->endFrame();
    entry->currentFrame = animator->startFrame();
    entry->animDir = animator->direction();
    entry->bmTreeBlueprint = new BMBase;
    parse(entry->bmTreeBlueprint, animator->jsonSource());
    m_waitCondition.wakeAll();
}

void BatchRenderer::deregisterAnimator(LottieAnimation *animator)
{
    QMutexLocker mlocker(&m_mutex);

    qCDebug(lcLottieQtBodymovinRenderThread) << "Deregister Animator:"
                                       << static_cast<void*>(animator);

    Entry *entry = m_animData.take(animator);
    if (entry) {
        qDeleteAll(entry->frameCache);
        delete entry->bmTreeBlueprint;
        delete entry;
    }
}

bool BatchRenderer::gotoFrame(LottieAnimation *animator, int frame)
{
    QMutexLocker mlocker(&m_mutex);

    Entry *entry = m_animData.value(animator, nullptr);
    if (entry) {
        qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:"
                                           << static_cast<void*>(animator)
                                           << "Goto frame:" << frame;
        entry->currentFrame = frame;
        entry->animDir = animator->direction();
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
        int frame = it.key();
        if ((frame - e->currentFrame) * e->animDir >= 0) { // same frame or same direction
            ++it;
        } else {
            qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:" << static_cast<void*>(e->animator)
                                                     << "Remove frame from cache" << frame;
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

void BatchRenderer::prerender(Entry *animEntry)
{
    while (animEntry->frameCache.count() < m_cacheSize) {
        BMBase *&bmTree = animEntry->frameCache[animEntry->currentFrame];
        if (bmTree == nullptr) {
            bmTree = new BMBase(*animEntry->bmTreeBlueprint);

            for (BMBase *elem : bmTree->children()) {
                if (elem->active(animEntry->currentFrame))
                    elem->updateProperties( animEntry->currentFrame);
            }
        }

        qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:"
                                           << static_cast<void*>(animEntry->animator)
                                           << "Frame drawn to cache. FN:"
                                           << animEntry->currentFrame;
        emit frameReady(animEntry->animator,  animEntry->currentFrame);

        animEntry->currentFrame += animEntry->animDir;

        if (animEntry->currentFrame > animEntry->endFrame) {
            animEntry->currentFrame = animEntry->startFrame;
        } else if (animEntry->currentFrame < animEntry->startFrame) {
            animEntry->currentFrame = animEntry->endFrame;
        }
    }
}

void BatchRenderer::frameRendered(LottieAnimation *animator, int frameNumber)
{
    QMutexLocker mlocker(&m_mutex);

    Entry *entry = m_animData.value(animator, nullptr);
    if (entry) {
        qCDebug(lcLottieQtBodymovinRenderThread) << "Animator:" << static_cast<void*>(animator)
                                           << "Remove frame from cache" << frameNumber;

        BMBase *root = entry->frameCache.take(frameNumber);
        if (root != nullptr) {
            delete root;
            m_waitCondition.wakeAll();
        }
    }
}

void BatchRenderer::run()
{
    qCDebug(lcLottieQtBodymovinRenderThread) << "rendering thread" << QThread::currentThread();

    while (!isInterruptionRequested()) {
        QMutexLocker mlocker(&m_mutex);

        for (Entry *e : qAsConst(m_animData))
            prerender(e);

        m_waitCondition.wait(&m_mutex);
    }
}

int BatchRenderer::parse(BMBase *rootElement, const QByteArray &jsonSource) const
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
            // Mask layers must be rendered before the layers they affect to
            // although they appear before in layer hierarchy. For this reason
            // move a mask after the affected layers, so it will be rendered first
            if (layer->isMaskLayer())
                rootElement->prependChild(layer);
            else
                rootElement->appendChild(layer);
        }
    }

    return 0;
}

QT_END_NAMESPACE
