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
#include "private/bmrepeater_p.h"
#include "private/bmrepeatertransform_p.h"

class tst_BMRepeaterTransform: public QObject
{
    Q_OBJECT

public:
    tst_BMRepeaterTransform();
    ~tst_BMRepeaterTransform();

private:

    //    void testParseStaticRect();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStaticInitialAnchorX();
    void testStaticInitialAnchorY();
    void testStaticInitialPositionX();
    void testStaticInitialPositionY();
    void testStaticInitialScaleX();
    void testStaticInitialScaleY();
    void testStaticInitialRotation();
    void testStaticInitialStartOpacity();
    void testStaticInitialEndOpacity();
    void testStaticUpdatedAnchorX();
    void testStaticUpdatedAnchorY();
    void testStaticUpdatedPositionX();
    void testStaticUpdatedPositionY();
    void testStaticUpdatedScaleX();
    void testStaticUpdatedScaleY();
    void testStaticUpdatedRotation();
    void testStaticUpdatedStartOpacity();
    void testStaticUpdatedEndOpacity();

    void testAnimatedInitialAnchorX();
    void testAnimatedInitialAnchorY();
    void testAnimatedInitialPositionX();
    void testAnimatedInitialPositionY();
    void testAnimatedInitialScaleX();
    void testAnimatedInitialScaleY();
    void testAnimatedInitialRotation();
    void testAnimatedInitialStartOpacity();
    void testAnimatedInitialEndOpacity();
    void testAnimatedUpdatedAnchorX();
    void testAnimatedUpdatedAnchorY();
    void testAnimatedUpdatedPositionX();
    void testAnimatedUpdatedPositionY();
    void testAnimatedUpdatedScaleX();
    void testAnimatedUpdatedScaleY();
    void testAnimatedUpdatedRotation();
    void testAnimatedUpdatedStartOpacity();
    void testAnimatedUpdatedEndOpacity();

    void testActive();

private:
    void loadTestData(const QByteArray &filename);
    void updateProperty(int frame);

    BMRepeaterTransform *m_transform = nullptr;
};

tst_BMRepeaterTransform::tst_BMRepeaterTransform()
{

}

tst_BMRepeaterTransform::~tst_BMRepeaterTransform()
{

}

void tst_BMRepeaterTransform::initTestCase()
{
}

void tst_BMRepeaterTransform::cleanupTestCase()
{
    if (m_transform)
        delete m_transform;
}

void tst_BMRepeaterTransform::testStaticInitialAnchorX()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 50.0));
}

void tst_BMRepeaterTransform::testStaticInitialAnchorY()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 50.0));
}

void tst_BMRepeaterTransform::testStaticInitialPositionX()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 50.0));
}

void tst_BMRepeaterTransform::testStaticInitialPositionY()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 50.0));
}

void tst_BMRepeaterTransform::testStaticInitialScaleX()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 0.5));
}

void tst_BMRepeaterTransform::testStaticInitialScaleY()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 0.5));
}

void tst_BMRepeaterTransform::testStaticInitialRotation()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->rotation(), 0.0));
}

void tst_BMRepeaterTransform::testStaticInitialStartOpacity()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->startOpacity(), 100.0));
}

void tst_BMRepeaterTransform::testStaticInitialEndOpacity()
{
    loadTestData("repeater_transform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->endOpacity(), 100.0));
}

void tst_BMRepeaterTransform::testStaticUpdatedAnchorX()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 50.0));
}

void tst_BMRepeaterTransform::testStaticUpdatedAnchorY()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 50.0));
}

void tst_BMRepeaterTransform::testStaticUpdatedPositionX()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 50.0));
}

void tst_BMRepeaterTransform::testStaticUpdatedPositionY()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 50.0));
}

void tst_BMRepeaterTransform::testStaticUpdatedScaleX()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 0.5));
}

void tst_BMRepeaterTransform::testStaticUpdatedScaleY()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 0.5));
}

void tst_BMRepeaterTransform::testStaticUpdatedRotation()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->rotation(), 0.0));
}

void tst_BMRepeaterTransform::testStaticUpdatedStartOpacity()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->startOpacity(), 100.0));
}

void tst_BMRepeaterTransform::testStaticUpdatedEndOpacity()
{
    loadTestData("repeater_transform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->endOpacity(), 100.0));
}

void tst_BMRepeaterTransform::testAnimatedInitialAnchorX()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 50.0));
}

void tst_BMRepeaterTransform::testAnimatedInitialAnchorY()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 50.0));
}

void tst_BMRepeaterTransform::testAnimatedInitialPositionX()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 50.0));
}

void tst_BMRepeaterTransform::testAnimatedInitialPositionY()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 50.0));
}

void tst_BMRepeaterTransform::testAnimatedInitialScaleX()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 0.5));
}

void tst_BMRepeaterTransform::testAnimatedInitialScaleY()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 0.5));
}

void tst_BMRepeaterTransform::testAnimatedInitialRotation()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->rotation(), 0.0));
}

void tst_BMRepeaterTransform::testAnimatedInitialStartOpacity()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->startOpacity(), 0.0));
}

void tst_BMRepeaterTransform::testAnimatedInitialEndOpacity()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->endOpacity(), 100.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedAnchorX()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 25.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedAnchorY()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 25.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedPositionX()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 75.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedPositionY()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 75.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedScaleX()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 1.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedScaleY()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 1.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedRotation()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->rotation(), (3 * 360 + 30.0)));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedStartOpacity()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->startOpacity(), 25.0));
}

void tst_BMRepeaterTransform::testAnimatedUpdatedEndOpacity()
{
    loadTestData("repeater_transform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->endOpacity(), 75.0));
}

void tst_BMRepeaterTransform::testActive()
{
    loadTestData("repeater_transform_static.json");
    QVERIFY(m_transform->active(100) == true);
}

void tst_BMRepeaterTransform::loadTestData(const QByteArray &filename)
{
    if (m_transform) {
        delete m_transform;
        m_transform = nullptr;
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
        if (shape->type() == BM_SHAPE_REPEATER_IX)
            break;
        shapesIt++;
    }

    BMRepeater *repeater = static_cast<BMRepeater*>(shape);
    m_transform = static_cast<BMRepeaterTransform*>(repeater->transform().clone());

    QVERIFY(m_transform != nullptr);
}

void tst_BMRepeaterTransform::updateProperty(int frame)
{
    m_transform->updateProperties(frame);
}

QTEST_MAIN(tst_BMRepeaterTransform)
#include "tst_bmrepeatertransform.moc"
