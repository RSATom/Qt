/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtWebView module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qwebenginewebview_p.h"
#include <private/qwebview_p.h>
#include <private/qwebviewloadrequest_p.h>

#include <QtWebView/private/qquickwebview_p.h>

#include <QtCore/qmap.h>
#include <QtGui/qguiapplication.h>
#include <QtCore/qjsondocument.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/qurl.h>

#include <QtQml/qqml.h>

#include <QtQuick/qquickwindow.h>
#include <QtQuick/qquickview.h>
#include <QtQuick/qquickitem.h>

#include <QtWebEngine/private/qquickwebengineview_p.h>
#include <QtWebEngine/private/qquickwebengineloadrequest_p.h>

QT_BEGIN_NAMESPACE

static QByteArray qmlSource()
{
    return QByteArrayLiteral("import QtWebEngine 1.1\n"
                             "    WebEngineView {\n"
                             "}\n");
}

QWebEngineWebViewPrivate::QWebEngineWebViewPrivate(QObject *p)
    : QAbstractWebView(p), m_profile(nullptr)
{
    m_webEngineView.m_parent = this;
}

QWebEngineWebViewPrivate::~QWebEngineWebViewPrivate()
{
}

QString QWebEngineWebViewPrivate::httpUserAgent() const
{
    return m_httpUserAgent;
}

void QWebEngineWebViewPrivate::setHttpUserAgent(const QString &userAgent)
{
    m_httpUserAgent = userAgent;
    if (m_profile) {
        m_profile->setHttpUserAgent(userAgent);
        Q_EMIT httpUserAgentChanged(userAgent);
    }
}

QUrl QWebEngineWebViewPrivate::url() const
{
    return m_webEngineView->url();
}

void QWebEngineWebViewPrivate::setUrl(const QUrl &url)
{
    m_webEngineView->setUrl(url);
}

void QWebEngineWebViewPrivate::loadHtml(const QString &html, const QUrl &baseUrl)
{
    m_webEngineView->loadHtml(html, baseUrl);
}

bool QWebEngineWebViewPrivate::canGoBack() const
{
    return m_webEngineView->canGoBack();
}

void QWebEngineWebViewPrivate::goBack()
{
    m_webEngineView->goBack();
}

bool QWebEngineWebViewPrivate::canGoForward() const
{
    return m_webEngineView->canGoForward();
}

void QWebEngineWebViewPrivate::goForward()
{
    m_webEngineView->goForward();
}

void QWebEngineWebViewPrivate::reload()
{
    m_webEngineView->reload();
}

QString QWebEngineWebViewPrivate::title() const
{
    return m_webEngineView->title();
}

void QWebEngineWebViewPrivate::setGeometry(const QRect &geometry)
{
    m_webEngineView->setSize(geometry.size());
}

void QWebEngineWebViewPrivate::setVisibility(QWindow::Visibility visibility)
{
    setVisible(visibility != QWindow::Hidden ? true : false);
}

void QWebEngineWebViewPrivate::runJavaScriptPrivate(const QString &script,
                                                    int callbackId)
{
    m_webEngineView->runJavaScript(script, QQuickWebView::takeCallback(callbackId));
}

void QWebEngineWebViewPrivate::setVisible(bool visible)
{
    m_webEngineView->setVisible(visible);
}

void QWebEngineWebViewPrivate::setFocus(bool focus)
{
    if (focus)
        m_webEngineView->forceActiveFocus();
}

int QWebEngineWebViewPrivate::loadProgress() const
{
    return m_webEngineView->loadProgress();
}

bool QWebEngineWebViewPrivate::isLoading() const
{
    return m_webEngineView->isLoading();
}

void QWebEngineWebViewPrivate::setParentView(QObject *parentView)
{
    Q_UNUSED(parentView);
}

QObject *QWebEngineWebViewPrivate::parentView() const
{
    return m_webEngineView->window();
}

void QWebEngineWebViewPrivate::stop()
{
    m_webEngineView->stop();
}

void QWebEngineWebViewPrivate::q_urlChanged()
{
    Q_EMIT urlChanged(m_webEngineView->url());
}

void QWebEngineWebViewPrivate::q_loadProgressChanged()
{
    Q_EMIT loadProgressChanged(m_webEngineView->loadProgress());
}

void QWebEngineWebViewPrivate::q_titleChanged()
{
    Q_EMIT titleChanged(m_webEngineView->title());
}

void QWebEngineWebViewPrivate::q_loadingChanged(QQuickWebEngineLoadRequest *loadRequest)
{
    QWebViewLoadRequestPrivate lr(loadRequest->url(),
                                  static_cast<QWebView::LoadStatus>(loadRequest->status()), // These "should" match...
                                  loadRequest->errorString());

    Q_EMIT loadingChanged(lr);
}

void QWebEngineWebViewPrivate::q_profileChanged()
{
    auto profile = m_webEngineView->profile();
    if (profile == m_profile)
        return;

    m_profile = profile;
    auto userAgent = m_profile->httpUserAgent();
    if (m_httpUserAgent == userAgent)
        return;
    m_httpUserAgent = userAgent;
    QObject::connect(m_profile, &QQuickWebEngineProfile::httpUserAgentChanged, this, &QWebEngineWebViewPrivate::q_httpUserAgentChanged);
    Q_EMIT httpUserAgentChanged(userAgent);
}

void QWebEngineWebViewPrivate::q_httpUserAgentChanged()
{
    QString httpUserAgent = m_profile->httpUserAgent();
    if (m_httpUserAgent == httpUserAgent)
        return;
     m_httpUserAgent = httpUserAgent;
     Q_EMIT httpUserAgentChanged(m_httpUserAgent);
}

void QWebEngineWebViewPrivate::QQuickWebEngineViewPtr::init() const
{
    Q_ASSERT(!m_webEngineView);
    QObject *p = qobject_cast<QObject *>(m_parent);
    QQuickItem *parentItem = nullptr;
    while (p) {
        p = p->parent();
        parentItem = qobject_cast<QQuickWebView *>(p);
        if (parentItem)
            break;
    }

    if (!parentItem)
        return;

    QQmlEngine *engine = qmlEngine(parentItem);
    if (!engine)
        return;

    QQmlComponent *component = new QQmlComponent(engine);
    component->setData(qmlSource(), QUrl::fromLocalFile(QLatin1String("")));
    QQuickWebEngineView *webEngineView = qobject_cast<QQuickWebEngineView *>(component->create());
    Q_ASSERT(webEngineView);
    QQuickWebEngineProfile *profile = webEngineView->profile();
    m_parent->m_profile = profile;
    // When the httpUserAgent is set as a property then it will be set before
    // init() is called
    if (m_parent->m_httpUserAgent.isEmpty())
        m_parent->m_httpUserAgent = profile->httpUserAgent();
    else
        profile->setHttpUserAgent(m_parent->m_httpUserAgent);
    QObject::connect(webEngineView, &QQuickWebEngineView::urlChanged, m_parent, &QWebEngineWebViewPrivate::q_urlChanged);
    QObject::connect(webEngineView, &QQuickWebEngineView::loadProgressChanged, m_parent, &QWebEngineWebViewPrivate::q_loadProgressChanged);
    QObject::connect(webEngineView, &QQuickWebEngineView::loadingChanged, m_parent, &QWebEngineWebViewPrivate::q_loadingChanged);
    QObject::connect(webEngineView, &QQuickWebEngineView::titleChanged, m_parent, &QWebEngineWebViewPrivate::q_titleChanged);
    QObject::connect(webEngineView, &QQuickWebEngineView::profileChanged,m_parent, &QWebEngineWebViewPrivate::q_profileChanged);
    QObject::connect(profile, &QQuickWebEngineProfile::httpUserAgentChanged, m_parent, &QWebEngineWebViewPrivate::q_httpUserAgentChanged);
    webEngineView->setParentItem(parentItem);
    m_webEngineView.reset(webEngineView);
}

QT_END_NAMESPACE
