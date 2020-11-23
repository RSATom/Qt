/****************************************************************************
**
** Copyright (C) 2017-2015 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectsettingsstore.h>
#include <QtRemoteObjects/qremoteobjectpendingcall.h>
#include <QTimer>
#include <QQmlExtensionPlugin>
#include <QJSValue>
#include <QtQml/private/qjsvalue_p.h>
#include <QtQml/qqmlengine.h>
#include <qqmlinfo.h>
#include <qqml.h>

QT_BEGIN_NAMESPACE

struct QtQmlRemoteObjectsResponse {
    QJSValue promise;
    QTimer *timer;
};


// documentation updates for this class can be made in remoteobjects-qml.qdoc
class QtQmlRemoteObjects : public QObject
{
    Q_OBJECT
public:
    ~QtQmlRemoteObjects() {
        auto i = m_callbacks.begin();
        while (i != m_callbacks.end()) {
            delete i.key();
            delete i.value().timer;
            i = m_callbacks.erase(i);
        }
    }

    Q_INVOKABLE QJSValue watch(const QRemoteObjectPendingCall &reply, int timeout = 30000) {
        if (m_accessiblePromise.isUndefined())
            m_accessiblePromise = qmlEngine(this)->evaluate("(function() { var obj = {}; obj.promise = new Promise(function(resolve, reject) { obj.resolve = resolve; obj.reject = reject; }); return obj; })");

        QRemoteObjectPendingCallWatcher *watcher = new QRemoteObjectPendingCallWatcher(reply);

        QJSValue promise = m_accessiblePromise.call();
        QtQmlRemoteObjectsResponse response;
        response.promise = promise;
        response.timer = new QTimer();
        response.timer->setSingleShot(true);
        m_callbacks.insert(watcher, response);

        // handle timeout
        connect(response.timer, &QTimer::timeout, [this, watcher]() {
            auto i = m_callbacks.find(watcher);
            if (i == m_callbacks.end()) {
                qmlWarning(this) << "could not find callback for watcher.";
                return;
            }

            QJSValue v(QLatin1String("timeout"));
            i.value().promise.property("reject").call(QJSValueList() << v);

            delete i.key();
            delete i.value().timer;
            m_callbacks.erase(i);
        });

        // handle success
        connect(watcher, &QRemoteObjectPendingCallWatcher::finished, [this](QRemoteObjectPendingCallWatcher *self) {
            auto i = m_callbacks.find(self);
            if (i == m_callbacks.end()) {
                qmlWarning(this) << "could not find callback for watcher.";
                return;
            }

            QJSValue v;
            QJSValuePrivate::setVariant(&v, self->returnValue());
            i.value().promise.property("resolve").call(QJSValueList() << v);

            delete i.key();
            delete i.value().timer;
            m_callbacks.erase(i);
        });

        response.timer->start(timeout);
        return promise.property("promise");
    }

private:
    QHash<QRemoteObjectPendingCallWatcher*,QtQmlRemoteObjectsResponse> m_callbacks;
    QJSValue m_accessiblePromise;
};

class QtRemoteObjectsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override
    {
        qmlRegisterModule(uri, 5, QT_VERSION_MINOR);

        qmlRegisterUncreatableType<QRemoteObjectAbstractPersistedStore>(uri, 5, 12, "PersistedStore", "Cannot create PersistedStore");

        qmlRegisterType<QRemoteObjectNode>(uri, 5, 12, "Node");
        qmlRegisterType<QRemoteObjectSettingsStore>(uri, 5, 12, "SettingsStore");
        qmlRegisterSingletonType<QtQmlRemoteObjects>(uri, 5, 14, "QtRemoteObjects", [](QQmlEngine *, QJSEngine*){return new QtQmlRemoteObjects();});
        qmlRegisterType<QRemoteObjectHost>(uri, 5, 15, "Host");
        qmlProtectModule(uri, 5);
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
