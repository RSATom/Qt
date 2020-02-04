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
#include "private/bmshapelayer_p.h"

class tst_BMShapeLayer: public QObject
{
    Q_OBJECT

public:
    tst_BMShapeLayer();
    ~tst_BMShapeLayer();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testName();
    void testType();
    void testWidth();
    void testHeight();
    void testActive();
    void testIsClippedLayer();
    void testIsMaskLayer();
    void testClipMode();

private:
    void loadTestData(const QByteArray &filename);

    BMShapeLayer *m_layer;
    BMShapeLayer *m_clippedlayer;
    qreal m_width;
    qreal m_height;
};

tst_BMShapeLayer::tst_BMShapeLayer()
{
    m_layer = nullptr;
    m_clippedlayer = nullptr;
}

tst_BMShapeLayer::~tst_BMShapeLayer()
{

}

void tst_BMShapeLayer::initTestCase()
{

}

void tst_BMShapeLayer::cleanupTestCase()
{

}

void tst_BMShapeLayer::testName()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_layer->name() == QString("Shape Layer 1"));
}

void tst_BMShapeLayer::testType()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_layer->type() == BM_LAYER_SHAPE_IX);
}

void tst_BMShapeLayer::testActive()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_layer->active(0) == false);
    QVERIFY(m_layer->active(100) == true);
    QVERIFY(m_layer->active(150) == false);
}

void tst_BMShapeLayer::testWidth()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_width == 100.0);
}

void tst_BMShapeLayer::testHeight()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_height == 100.0);
}

void tst_BMShapeLayer::testIsClippedLayer()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_layer->isClippedLayer() == false);
    loadTestData("shape_mask_alphaclip.json");
    QVERIFY(m_clippedlayer->isClippedLayer() == true);
}

void tst_BMShapeLayer::testIsMaskLayer()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_layer->isMaskLayer() == false);
    loadTestData("shape_mask_alphaclip.json");
    QVERIFY(m_layer->isMaskLayer() == true);
}

void tst_BMShapeLayer::testClipMode()
{
    loadTestData("shape_active_60to120.json");
    QVERIFY(m_layer->clipMode() == BMLayer::MatteClipMode::NoClip);
    loadTestData("shape_mask_alphaclip.json");
    QVERIFY(m_clippedlayer->clipMode() == BMLayer::MatteClipMode::Alpha);
    loadTestData("shape_mask_alphainvclip.json");
    QVERIFY(m_clippedlayer->clipMode() == BMLayer::MatteClipMode::InvertedAlpha);
    loadTestData("shape_mask_lumaclip.json");
    QVERIFY(m_clippedlayer->clipMode() == BMLayer::MatteClipMode::Luminence);
    loadTestData("shape_mask_lumainvclip.json");
    QVERIFY(m_clippedlayer->clipMode() == BMLayer::MatteClipMode::InvertedLuminence);
}

void tst_BMShapeLayer::loadTestData(const QByteArray &filename)
{
    if (m_layer) {
        delete m_layer;
        m_layer = nullptr;
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

    m_width = rootObj.value(QLatin1String("w")).toVariant().toReal();
    m_height = rootObj.value(QLatin1String("h")).toVariant().toReal();

    QJsonArray layers = rootObj.value(QLatin1String("layers")).toArray();
    QJsonObject layerObj = layers[0].toObject();
    int type = layerObj.value(QLatin1String("ty")).toInt();
    if (type != 4)
        QFAIL("It's not shape layer");
    m_layer = new BMShapeLayer(layerObj);
    QVERIFY(m_layer != nullptr);

    if (layers.size() > 1) {
        layerObj = layers[1].toObject();
        type = layerObj.value(QLatin1String("ty")).toInt();
        if (type != 4)
            QFAIL("it's not shape layer");
        m_clippedlayer = new BMShapeLayer(layerObj);
        QVERIFY(m_clippedlayer != nullptr);
    }
}

QTEST_MAIN(tst_BMShapeLayer)
#include "tst_bmshapelayer.moc"
