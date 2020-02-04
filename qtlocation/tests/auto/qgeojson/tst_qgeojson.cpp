/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>
#include <QtPositioning/QGeoCoordinate>
#include <QtCore/QJsonDocument>
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtLocation/private/qgeojson_p.h>

QT_USE_NAMESPACE

class tst_QGeoJson : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testGeojson();

private:
    QString testDataDir;
};

void tst_QGeoJson::testGeojson()
{
    QJsonDocument originalDocument;
    QJsonDocument processedDocument;
    QVariantList modelList;

    QString exampleGeometries[] = {
        QStringLiteral("01-point.json"),
        QStringLiteral("02-linestring.json"),
        QStringLiteral("03-multipoint.json"),
        QStringLiteral("04-polygon.json"),
        QStringLiteral("05-multilinestring.json"),
        QStringLiteral("06-multipolygon.json"),
        QStringLiteral("07-geometrycollection.json"),
        QStringLiteral("08-feature.json"),
        QStringLiteral("09-featurecollection.json"),
        QStringLiteral("10-countries.json"),
        QStringLiteral("11-full.json")
    };

    testDataDir = QFileInfo(QFINDTESTDATA("01-point.json")).absolutePath();
    if (testDataDir.isEmpty())
        testDataDir = QCoreApplication::applicationDirPath();
    for (int i = 0; i < 11; ++i) {
        modelList.clear();

        QFile file(testDataDir +  QStringLiteral("/") + exampleGeometries[i]);
        file.open(QFile::ReadOnly);

        QByteArray testJson = file.readAll();
        originalDocument = QJsonDocument::fromJson(testJson);

        modelList = QGeoJson::importGeoJson(originalDocument);
        processedDocument = QGeoJson::exportGeoJson(modelList);

        QVERIFY2(processedDocument == originalDocument, qPrintable("Test failed in file: " + exampleGeometries[i]));
    }
}

QTEST_MAIN(tst_QGeoJson)
#include "tst_qgeojson.moc"
