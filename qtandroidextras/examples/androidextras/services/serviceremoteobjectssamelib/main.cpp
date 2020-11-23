/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtAndroidExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "rep_qtandroidservice_replica.h"
#include "qtandroidservice_ro.h"

#include <QAndroidIntent>
#include <QtAndroid>
#include <QAndroidService>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

void startService()
{
    QAndroidIntent serviceIntent(QtAndroid::androidActivity().object(),
                                        "org/qtproject/example/qtandroidservice/QtAndroidService");
    QAndroidJniObject result = QtAndroid::androidActivity().callObjectMethod(
                "startService",
                "(Landroid/content/Intent;)Landroid/content/ComponentName;",
                serviceIntent.handle().object());
}

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QGuiApplication app(argc, argv);
        QQmlApplicationEngine engine;

        startService();

        QRemoteObjectNode repNode;
        repNode.connectToNode(QUrl(QStringLiteral("local:replica")));
        QSharedPointer<QtAndroidServiceReplica> rep(repNode.acquire<QtAndroidServiceReplica>());
        engine.rootContext()->setContextProperty("qtAndroidService", rep.data());
        bool res = rep->waitForSource();
        Q_ASSERT(res);
        rep->sendToService("Qt");

        engine.load(QUrl(QLatin1String("qrc:/main.qml")));

        return app.exec();

    } else if (argc > 1 && strcmp(argv[1], "-service") == 0) {
        qWarning() << "QtAndroidService starting from same .so";
        QAndroidService app(argc, argv);

        QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:replica")));
        QtAndroidService qtAndroidService;
        srcNode.enableRemoting(&qtAndroidService);

        return app.exec();

    } else {
        qWarning() << "Unrecognized command line argument";
        return -1;
    }
}
