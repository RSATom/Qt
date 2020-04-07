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

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "private/bmbase_p.h"
#include "private/bmlayer_p.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        printf("Filename missing\n");
        return -1;
    }

    QFile sourceFile(argv[1]);

    if (!sourceFile.open(QIODevice::ReadOnly)) {
        fprintf(stderr, "Cannot open file\n");
        return -1;
    }

    QByteArray json = sourceFile.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject rootObj = doc.object();

    if (rootObj.empty()) {
        fprintf(stderr, "Json document not found\n");
        return -1;
    }

    if (rootObj.value(QLatin1String("assets")).toArray().count())
        qWarning() << "assets not supported";

    if (rootObj.value(QLatin1String("chars")).toArray().count())
        qWarning() << "chars not supported";

    if (rootObj.value(QLatin1String("markers")).toArray().count())
        qWarning() << "markers not supported";

    QJsonArray jsonLayers = rootObj.value(QLatin1String("layers")).toArray();
    QJsonArray::const_iterator jsonLayerIt = jsonLayers.constBegin();
    while (jsonLayerIt != jsonLayers.constEnd()) {
        QJsonObject jsonLayer = (*jsonLayerIt).toObject();
        BMLayer *layer = BMLayer::construct(jsonLayer);
        if (layer)
            delete layer;
        jsonLayerIt++;
    }
}
