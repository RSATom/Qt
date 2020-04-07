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

#include <QtTest/QtTest>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "private/bmlayer_p.h"
#include "private/bmtrimpath_p.h"

class tst_BMTrimPath: public QObject
{
    Q_OBJECT

public:
    tst_BMTrimPath();
    ~tst_BMTrimPath();

private:

    //    void testParseStaticRect();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStaticInitialStart();
    void testStaticInitialEnd();
    void testStaticInitialOffset();
    void testStaticInitialSimultaneous();
    void testStaticUpdatedStart();
    void testStaticUpdatedEnd();
    void testStaticUpdatedOffset();
    void testStaticUpdatedSimultaneous();

    void testAnimatedInitialStart();
    void testAnimatedInitialEnd();
    void testAnimatedInitialOffset();
    void testAnimatedInitialSimultaneous();
    void testAnimatedUpdatedStart();
    void testAnimatedUpdatedEnd();
    void testAnimatedUpdatedOffset();
    void testAnimatedUpdatedSimultaneous();

    void testName();
    void testType();
    void testHidden();
    void testActive();

private:
    void loadTestData(const QByteArray &filename);
    void updateProperty(int frame);

    BMTrimPath *m_trimpath = nullptr;
};

tst_BMTrimPath::tst_BMTrimPath()
{

}

tst_BMTrimPath::~tst_BMTrimPath()
{

}

void tst_BMTrimPath::initTestCase()
{
}

void tst_BMTrimPath::cleanupTestCase()
{
    if (m_trimpath)
        delete m_trimpath;
}

void tst_BMTrimPath::testStaticInitialStart()
{
    loadTestData("trimpath_static_20to80.json");

    QVERIFY(qFuzzyCompare(m_trimpath->start(), 20.0));
}

void tst_BMTrimPath::testStaticInitialEnd()
{
    loadTestData("trimpath_static_20to80.json");

    QVERIFY(qFuzzyCompare(m_trimpath->end(), 80.0));
}

void tst_BMTrimPath::testStaticInitialOffset()
{
    loadTestData("trimpath_static_20to80.json");

    QVERIFY(qFuzzyCompare(m_trimpath->offset(), 0.0));
}

void tst_BMTrimPath::testStaticInitialSimultaneous()
{
    loadTestData("trimpath_static_20to80.json");

    QVERIFY(m_trimpath->simultaneous() == true);
}

void tst_BMTrimPath::testStaticUpdatedStart()
{
    loadTestData("trimpath_static_20to80.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_trimpath->start(), 20.0));
}

void tst_BMTrimPath::testStaticUpdatedEnd()
{
    loadTestData("trimpath_static_20to80.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_trimpath->end(), 80.0));
}

void tst_BMTrimPath::testStaticUpdatedOffset()
{
    loadTestData("trimpath_static_20to80.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_trimpath->offset(), 0.0));
}

void tst_BMTrimPath::testStaticUpdatedSimultaneous()
{
    loadTestData("trimpath_static_20to80.json");
    updateProperty(180);

    QVERIFY(m_trimpath->simultaneous() == true);
}

void tst_BMTrimPath::testAnimatedInitialStart()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_trimpath->start(), 20.0));
}

void tst_BMTrimPath::testAnimatedInitialEnd()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_trimpath->end(), 80.0));
}

void tst_BMTrimPath::testAnimatedInitialOffset()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_trimpath->offset(), 0.0));
}

void tst_BMTrimPath::testAnimatedInitialSimultaneous()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(0);

    QVERIFY(m_trimpath->simultaneous() == false);
}

void tst_BMTrimPath::testAnimatedUpdatedStart()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_trimpath->start(), 0.0));
}

void tst_BMTrimPath::testAnimatedUpdatedEnd()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_trimpath->end(), 60.0));
}

void tst_BMTrimPath::testAnimatedUpdatedOffset()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_trimpath->offset(), (360 * 3 + 30.0)));
}

void tst_BMTrimPath::testAnimatedUpdatedSimultaneous()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    updateProperty(180);

    QVERIFY(m_trimpath->simultaneous() == false);
}

void tst_BMTrimPath::testName()
{
    loadTestData("trimpath_hidden.json");

    QVERIFY(m_trimpath->name() == QString("Trim Paths 1"));
}

void tst_BMTrimPath::testType()
{
    loadTestData("trimpath_hidden.json");

    QVERIFY(m_trimpath->type() == BM_SHAPE_TRIM_IX);
}

void tst_BMTrimPath::testHidden()
{
    loadTestData("trimpath_hidden.json");

    QVERIFY(m_trimpath->hidden() == true);
}

void tst_BMTrimPath::testActive()
{
    loadTestData("trimpath_animated_2080_0_to_0060_3x30.json");
    QVERIFY(m_trimpath->active(100) == true);

    loadTestData("trimpath_hidden.json");
    QVERIFY(m_trimpath->active(100) == false);
}

void tst_BMTrimPath::loadTestData(const QByteArray &filename)
{
    if (m_trimpath) {
        delete m_trimpath;
        m_trimpath = nullptr;
    }

    QFile sourceFile(QFINDTESTDATA(filename.constData()));
    if (!sourceFile.exists())
        QFAIL("File does not exist");
    if (!sourceFile.open(QIODevice::ReadOnly))
        QFAIL("Cannot read test file");

    QByteArray json = sourceFile.readAll();

    sourceFile.close();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject rootObj = doc.object();
    if (rootObj.empty())
        QFAIL("Cannot parse test file");

    QJsonArray layers = rootObj.value(QLatin1String("layers")).toArray();
    QJsonObject layerObj = layers[0].toObject();
    int type = layerObj.value(QLatin1String("ty")).toInt();
    if (type != 4)
        QFAIL("It's not shape layer");

    QJsonArray shapes = layerObj.value(QLatin1String("shapes")).toArray();
    QJsonArray::const_iterator shapesIt = shapes.constBegin();
    BMShape* shape = nullptr;
    while (shapesIt != shapes.end()) {
        QJsonObject childObj = (*shapesIt).toObject();
        shape = BMShape::construct(childObj);
        QVERIFY(shape != nullptr);
        if (shape->type() == BM_SHAPE_TRIM_IX)
            break;
        shapesIt++;
    }

    m_trimpath = static_cast<BMTrimPath*>(shape);

    QVERIFY(m_trimpath != nullptr);
}

void tst_BMTrimPath::updateProperty(int frame)
{
    m_trimpath->updateProperties(frame);
}

QTEST_MAIN(tst_BMTrimPath)
#include "tst_bmtrimpath.moc"
