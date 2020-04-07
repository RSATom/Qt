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
#include "private/bmgroup_p.h"
#include "private/bmshapetransform_p.h"

class tst_BMShapeTransform: public QObject
{
    Q_OBJECT

public:
    tst_BMShapeTransform();
    ~tst_BMShapeTransform();

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
    void testStaticInitialOpacity();
    void testStaticInitialSkew();
    void testStaticInitialSkewAxis();
    void testStaticUpdatedAnchorX();
    void testStaticUpdatedAnchorY();
    void testStaticUpdatedPositionX();
    void testStaticUpdatedPositionY();
    void testStaticUpdatedScaleX();
    void testStaticUpdatedScaleY();
    void testStaticUpdatedRotation();
    void testStaticUpdatedOpacity();
    void testStaticUpdatedSkew();
    void testStaticUpdatedSkewAxis();


    void testAnimatedInitialAnchorX();
    void testAnimatedInitialAnchorY();
    void testAnimatedInitialPositionX();
    void testAnimatedInitialPositionY();
    void testAnimatedInitialScaleX();
    void testAnimatedInitialScaleY();
    void testAnimatedInitialRotation();
    void testAnimatedInitialOpacity();
    void testAnimatedInitialSkew();
    void testAnimatedInitialSkewAxis();
    void testAnimatedUpdatedAnchorX();
    void testAnimatedUpdatedAnchorY();
    void testAnimatedUpdatedPositionX();
    void testAnimatedUpdatedPositionY();
    void testAnimatedUpdatedScaleX();
    void testAnimatedUpdatedScaleY();
    void testAnimatedUpdatedRotation();
    void testAnimatedUpdatedOpacity();
    void testAnimatedUpdatedSkew();
    void testAnimatedUpdatedSkewAxis();

    void testName();
    void testType();
    void testActive();

private:
    void loadTestData(const QByteArray &filename);
    void updateProperty(int frame);

    BMShapeTransform *m_transform = nullptr;
};

tst_BMShapeTransform::tst_BMShapeTransform()
{

}

tst_BMShapeTransform::~tst_BMShapeTransform()
{

}

void tst_BMShapeTransform::initTestCase()
{
}

void tst_BMShapeTransform::cleanupTestCase()
{
    if (m_transform)
        delete m_transform;
}

void tst_BMShapeTransform::testStaticInitialAnchorX()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 50.0));
}

void tst_BMShapeTransform::testStaticInitialAnchorY()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 50.0));
}

void tst_BMShapeTransform::testStaticInitialPositionX()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 50.0));
}

void tst_BMShapeTransform::testStaticInitialPositionY()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 50.0));
}

void tst_BMShapeTransform::testStaticInitialScaleX()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 0.5));
}

void tst_BMShapeTransform::testStaticInitialScaleY()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 0.5));
}

void tst_BMShapeTransform::testStaticInitialRotation()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->rotation(), 0.0));
}

void tst_BMShapeTransform::testStaticInitialOpacity()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->opacity(), 1.0));
}

void tst_BMShapeTransform::testStaticInitialSkew()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->skew(), 0.0));
}

void tst_BMShapeTransform::testStaticInitialSkewAxis()
{
    loadTestData("shapetransform_static.json");

    QVERIFY(qFuzzyCompare(m_transform->skewAxis(), 0.0));
}

void tst_BMShapeTransform::testStaticUpdatedAnchorX()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 50.0));
}

void tst_BMShapeTransform::testStaticUpdatedAnchorY()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 50.0));
}

void tst_BMShapeTransform::testStaticUpdatedPositionX()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 50.0));
}

void tst_BMShapeTransform::testStaticUpdatedPositionY()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 50.0));
}

void tst_BMShapeTransform::testStaticUpdatedScaleX()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 0.5));
}

void tst_BMShapeTransform::testStaticUpdatedScaleY()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 0.5));
}

void tst_BMShapeTransform::testStaticUpdatedRotation()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->rotation(), 0.0));
}

void tst_BMShapeTransform::testStaticUpdatedOpacity()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->opacity(), 1.0));
}

void tst_BMShapeTransform::testStaticUpdatedSkew()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->skew(), 0.0));
}

void tst_BMShapeTransform::testStaticUpdatedSkewAxis()
{
    loadTestData("shapetransform_static.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->skewAxis(), 0.0));
}

void tst_BMShapeTransform::testAnimatedInitialAnchorX()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 50.0));
}

void tst_BMShapeTransform::testAnimatedInitialAnchorY()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 50.0));
}

void tst_BMShapeTransform::testAnimatedInitialPositionX()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 50.0));
}

void tst_BMShapeTransform::testAnimatedInitialPositionY()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 50.0));
}

void tst_BMShapeTransform::testAnimatedInitialScaleX()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 0.5));
}

void tst_BMShapeTransform::testAnimatedInitialScaleY()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 0.5));
}

void tst_BMShapeTransform::testAnimatedInitialRotation()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->rotation(), 0.0));
}

void tst_BMShapeTransform::testAnimatedInitialOpacity()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->opacity(), 1.0));
}

void tst_BMShapeTransform::testAnimatedInitialSkew()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->skew(), 0.0));
}

void tst_BMShapeTransform::testAnimatedInitialSkewAxis()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(0);

    QVERIFY(qFuzzyCompare(m_transform->skewAxis(), 0.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedAnchorX()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().x(), 25.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedAnchorY()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->anchorPoint().y(), 25.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedPositionX()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().x(), 75.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedPositionY()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->position().y(), 75.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedScaleX()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().x(), 1.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedScaleY()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->scale().y(), 1.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedRotation()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->rotation(), (3 * 360 + 30.0)));
}

void tst_BMShapeTransform::testAnimatedUpdatedOpacity()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->opacity(), 0.25));
}

void tst_BMShapeTransform::testAnimatedUpdatedSkew()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->skew(), 4.0));
}

void tst_BMShapeTransform::testAnimatedUpdatedSkewAxis()
{
    loadTestData("shapetransform_animated.json");
    updateProperty(180);

    QVERIFY(qFuzzyCompare(m_transform->skewAxis(), (2 * 360 + 25.0)));
}

void tst_BMShapeTransform::testName()
{
    loadTestData("shapetransform_static.json");
    QVERIFY(m_transform->name() == QString("Transform"));
}

void tst_BMShapeTransform::testType()
{
    loadTestData("shapetransform_static.json");
    QVERIFY(m_transform->type() == BM_SHAPE_TRANS_IX);
}

void tst_BMShapeTransform::testActive()
{
    loadTestData("shapetransform_static.json");
    QVERIFY(m_transform->active(100) == true);
}

void tst_BMShapeTransform::loadTestData(const QByteArray &filename)
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
    BMGroup* group = nullptr;
    while (shapesIt != shapes.end()) {
        QJsonObject childObj = (*shapesIt).toObject();
        BMShape *shape = BMShape::construct(childObj);
        QVERIFY(shape != nullptr);
        if (shape->type() == BM_SHAPE_GROUP_IX) {
            group = static_cast<BMGroup *>(shape);
            break;
        }
        shapesIt++;
    }
    QVERIFY(group != nullptr);

    m_transform = static_cast<BMShapeTransform*>(group->findChild("Transform"));

    QVERIFY(m_transform != nullptr);
}

void tst_BMShapeTransform::updateProperty(int frame)
{
    m_transform->updateProperties(frame);
}

QTEST_MAIN(tst_BMShapeTransform)
#include "tst_bmshapetransform.moc"
