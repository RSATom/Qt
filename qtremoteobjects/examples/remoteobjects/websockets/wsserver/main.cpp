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
#include <QTimer>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QWebSocket>
#include <QWebSocketServer>

#ifndef QT_NO_SSL
# include <QFile>
# include <QSslConfiguration>
# include <QSslKey>
#endif

#include "websocketiodevice.h"

struct TimerHandler : public QObject
{
    Q_OBJECT
public:
    QStandardItemModel *model;
public Q_SLOTS:
    void changeData() {
        Q_ASSERT(model);
        Q_ASSERT(model->rowCount() > 50);
        Q_ASSERT(model->columnCount() > 1);
        for (int i = 10; i < 50; ++i)
            model->setData(model->index(i, 1), QColor(Qt::blue), Qt::BackgroundRole);
    }
    void insertData() {
        Q_ASSERT(model);
        Q_ASSERT(model->rowCount() > 50);
        Q_ASSERT(model->columnCount() > 1);
        model->insertRows(2, 9);
        for (int i = 2; i < 11; ++i) {
            model->setData(model->index(i, 1), QColor(Qt::green), Qt::BackgroundRole);
            model->setData(model->index(i, 1), QLatin1String("InsertedRow"), Qt::DisplayRole);
        }
    }
    void removeData() {
        model->removeRows(2, 4);
    }

    void changeFlags() {
        QStandardItem *item = model->item(0, 0);
        item->setEnabled(false);
        item = item->child(0, 0);
        item->setFlags(item->flags() & Qt::ItemIsSelectable);
    }

    void moveData() {
        model->moveRows(QModelIndex(), 2, 4, QModelIndex(), 10);
    }
};

QList<QStandardItem*> addChild(int numChildren, int nestingLevel)
{
    QList<QStandardItem*> result;
    if (nestingLevel == 0)
        return result;
    for (int i = 0; i < numChildren; ++i) {
        QStandardItem *child = new QStandardItem(QStringLiteral("Child num %1, nesting Level %2").arg(i+1).arg(nestingLevel));
        if (i == 0)
            child->appendRow(addChild(numChildren, nestingLevel -1));
        result.push_back(child);
    }
    return result;
}

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.remoteobjects.debug=false\n"
                                     "qt.remoteobjects.warning=false");
    QApplication app(argc, argv);

    const int modelSize = 100000;
    QStringList list;
    QStandardItemModel sourceModel;
    QStringList hHeaderList;
    hHeaderList << QStringLiteral("First Column with spacing") << QStringLiteral("Second Column with spacing");
    sourceModel.setHorizontalHeaderLabels(hHeaderList);
    list.reserve(modelSize);
    for (int i = 0; i < modelSize; ++i) {
        QStandardItem *firstItem = new QStandardItem(QStringLiteral("FancyTextNumber %1").arg(i));
        if (i == 0)
            firstItem->appendRow(addChild(2, 2));
        QStandardItem *secondItem = new QStandardItem(QStringLiteral("FancyRow2TextNumber %1").arg(i));
        if (i % 2 == 0)
            firstItem->setBackground(Qt::red);
        QList<QStandardItem*> row;
        row << firstItem << secondItem;
        sourceModel.invisibleRootItem()->appendRow(row);
        //sourceModel.appendRow(row);
        list << QStringLiteral("FancyTextNumber %1").arg(i);
    }

    // Needed by QMLModelViewClient
    QHash<int,QByteArray> roleNames = {
        {Qt::DisplayRole, "_text"},
        {Qt::BackgroundRole, "_color"}
    };
    sourceModel.setItemRoleNames(roleNames);

    QVector<int> roles;
    roles << Qt::DisplayRole << Qt::BackgroundRole;

    QWebSocketServer webSockServer{QStringLiteral("WS QtRO"), QWebSocketServer::NonSecureMode};
    webSockServer.listen(QHostAddress::Any, 8088);

    QRemoteObjectHost hostNode;
    hostNode.setHostUrl(webSockServer.serverAddress().toString(), QRemoteObjectHost::AllowExternalRegistration);

    hostNode.enableRemoting(&sourceModel, QStringLiteral("RemoteModel"), roles);

    QObject::connect(&webSockServer, &QWebSocketServer::newConnection, &hostNode, [&hostNode, &webSockServer]{
        while (auto conn = webSockServer.nextPendingConnection()) {
#ifndef QT_NO_SSL
            // Always use secure connections when available
            QSslConfiguration sslConf;
            QFile certFile(QStringLiteral(":/sslcert/server.crt"));
            if (!certFile.open(QIODevice::ReadOnly))
                qFatal("Can't open client.crt file");
            sslConf.setLocalCertificate(QSslCertificate{certFile.readAll()});

            QFile keyFile(QStringLiteral(":/sslcert/server.key"));
            if (!keyFile.open(QIODevice::ReadOnly))
                qFatal("Can't open client.key file");
            sslConf.setPrivateKey(QSslKey{keyFile.readAll(), QSsl::Rsa});

            sslConf.setPeerVerifyMode(QSslSocket::VerifyPeer);
            conn->setSslConfiguration(sslConf);
            QObject::connect(conn, &QWebSocket::sslErrors, conn, &QWebSocket::deleteLater);
#endif
            QObject::connect(conn, &QWebSocket::disconnected, conn, &QWebSocket::deleteLater);
            QObject::connect(conn,  QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), conn, &QWebSocket::deleteLater);
            auto ioDevice = new WebSocketIoDevice(conn);
            QObject::connect(conn, &QWebSocket::destroyed, ioDevice, &WebSocketIoDevice::deleteLater);
            hostNode.addHostSideConnection(ioDevice);
        }
    });

    QTreeView view;
    view.setWindowTitle(QStringLiteral("SourceView"));
    view.setModel(&sourceModel);
    view.show();
    TimerHandler handler;
    handler.model = &sourceModel;
    QTimer::singleShot(5000, &handler, SLOT(changeData()));
    QTimer::singleShot(10000, &handler, SLOT(insertData()));
    QTimer::singleShot(11000, &handler, SLOT(changeFlags()));
    QTimer::singleShot(12000, &handler, SLOT(removeData()));
    QTimer::singleShot(13000, &handler, SLOT(moveData()));

    return app.exec();
}

#include "main.moc"
