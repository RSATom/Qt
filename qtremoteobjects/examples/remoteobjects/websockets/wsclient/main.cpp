/****************************************************************************
**
** Copyright (C) 2019 Ford Motor Company
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtRemoteObjects module of the Qt Toolkit.
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

#include <QTreeView>
#include <QApplication>
#include <QRemoteObjectNode>
#include <QAbstractItemModelReplica>
#include <QWebSocket>

#ifndef QT_NO_SSL
# include <QFile>
# include <QSslConfiguration>
# include <QSslKey>
#endif
#include "websocketiodevice.h"

int main(int argc, char **argv)
{

    QLoggingCategory::setFilterRules("qt.remoteobjects.debug=false\n"
                                     "qt.remoteobjects.warning=false\n"
                                     "qt.remoteobjects.models.debug=false\n"
                                     "qt.remoteobjects.models.debug=false");

    QApplication app(argc, argv);



    QScopedPointer<QWebSocket> webSocket{new QWebSocket};
    WebSocketIoDevice socket(webSocket.data());
#ifndef QT_NO_SSL
    // Always use secure connections when available
    QSslConfiguration sslConf;
    QFile certFile(QStringLiteral(":/sslcert/client.crt"));
    if (!certFile.open(QIODevice::ReadOnly))
        qFatal("Can't open client.crt file");
    sslConf.setLocalCertificate(QSslCertificate{certFile.readAll()});

    QFile keyFile(QStringLiteral(":/sslcert/client.key"));
    if (!keyFile.open(QIODevice::ReadOnly))
        qFatal("Can't open client.key file");
    sslConf.setPrivateKey(QSslKey{keyFile.readAll(), QSsl::Rsa});

    sslConf.setPeerVerifyMode(QSslSocket::VerifyPeer);
    webSocket->setSslConfiguration(sslConf);
#endif
    QRemoteObjectNode node;
    node.addClientSideConnection(&socket);
    node.setHeartbeatInterval(1000);
    webSocket->open(QStringLiteral("ws://localhost:8088"));

    QTreeView view;
    view.setWindowTitle(QStringLiteral("RemoteView"));
    view.resize(640,480);
    QScopedPointer<QAbstractItemModelReplica> model(node.acquireModel(QStringLiteral("RemoteModel")));
    view.setModel(model.data());
    view.show();

    return app.exec();
}
