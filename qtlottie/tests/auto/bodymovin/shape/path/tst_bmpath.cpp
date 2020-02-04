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
#include "private/bmfreeformshape_p.h"

class tst_BMPath: public QObject
{
    Q_OBJECT

public:
    tst_BMPath();
    ~tst_BMPath();

private:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStaticCurveInitialStartPoint();
    void testStaticCurveInitialSecondPoint();
    void testStaticCurveInitialThirdPoint();
    void testStaticCurveInitialFourthPoint();
    void testStaticCurveInitialFifthPoint();
    void testStaticCurveInitialSixthPoint();
    void testStaticCurveUpdatedStartPoint();
    void testStaticCurveUpdatedSecondPoint();
    void testStaticCurveUpdatedThirdPoint();
    void testStaticCurveUpdatedFourthPoint();
    void testStaticCurveUpdatedFifthPoint();
    void testStaticCurveUpdatedSixthPoint();

    void testStaticTriangleInitialStartPoint();
    void testStaticTriangleInitialSecondPoint();
    void testStaticTriangleInitialThirdPoint();
    void testStaticTriangleInitialClosed();
    void testStaticTriangleUpdatedStartPoint();
    void testStaticTriangleUpdatedSecondPoint();
    void testStaticTriangleUpdatedThirdPoint();
    void testStaticTriangleUpdatedClosed();

    void testStaticRotoInitialStartPoint();
    void testStaticRotoInitialSecondPoint();
    void testStaticRotoInitialThirdPoint();
    void testStaticRotoInitialFourthPoint();
    void testStaticRotoInitialFifthPoint();
    void testStaticRotoInitialSixthPoint();
    void testStaticRotoInitialSeventhPoint();
    void testStaticRotoInitialEighthPoint();
    void testStaticRotoInitialNinthPoint();
    void testStaticRotoInitialTenthPoint();
    void testStaticRotoInitialEleventhPoint();
    void testStaticRotoInitialTwelvthPoint();
    void testStaticRotoInitialClosed();
    void testStaticRotoUpdatedStartPoint();
    void testStaticRotoUpdatedSecondPoint();
    void testStaticRotoUpdatedThirdPoint();
    void testStaticRotoUpdatedFourthPoint();
    void testStaticRotoUpdatedFifthPoint();
    void testStaticRotoUpdatedSixthPoint();
    void testStaticRotoUpdatedSeventhPoint();
    void testStaticRotoUpdatedEighthPoint();
    void testStaticRotoUpdatedNinthPoint();
    void testStaticRotoUpdatedTenthPoint();
    void testStaticRotoUpdatedEleventhPoint();
    void testStaticRotoUpdatedTwelvthPoint();
    void testStaticRotoUpdatedClosed();

    void testAnimatedCurveInitialStartPoint();
    void testAnimatedCurveInitialSecondPoint();
    void testAnimatedCurveInitialThirdPoint();
    void testAnimatedCurveInitialFourthPoint();
    void testAnimatedCurveInitialFifthPoint();
    void testAnimatedCurveInitialSixthPoint();
    void testAnimatedCurveUpdatedStartPoint();
    void testAnimatedCurveUpdatedSecondPoint();
    void testAnimatedCurveUpdatedThirdPoint();
    void testAnimatedCurveUpdatedFourthPoint();
    void testAnimatedCurveUpdatedFifthPoint();
    void testAnimatedCurveUpdatedSixthPoint();

    void testAnimatedTriangleInitialStartPoint();
    void testAnimatedTriangleInitialSecondPoint();
    void testAnimatedTriangleInitialThirdPoint();
    void testAnimatedTriangleInitialClosed();
    void testAnimatedTriangleUpdatedStartPoint();
    void testAnimatedTriangleUpdatedSecondPoint();
    void testAnimatedTriangleUpdatedThirdPoint();
    void testAnimatedTriangleUpdatedClosed();

    void testAnimatedRotoInitialStartPoint();
    void testAnimatedRotoInitialSecondPoint();
    void testAnimatedRotoInitialThirdPoint();
    void testAnimatedRotoInitialFourthPoint();
    void testAnimatedRotoInitialFifthPoint();
    void testAnimatedRotoInitialSixthPoint();
    void testAnimatedRotoInitialSeventhPoint();
    void testAnimatedRotoInitialEighthPoint();
    void testAnimatedRotoInitialNinthPoint();
    void testAnimatedRotoInitialTenthPoint();
    void testAnimatedRotoInitialEleventhPoint();
    void testAnimatedRotoInitialTwelvthPoint();
    void testAnimatedRotoInitialClosed();
    void testAnimatedRotoUpdatedStartPoint();
    void testAnimatedRotoUpdatedSecondPoint();
    void testAnimatedRotoUpdatedThirdPoint();
    void testAnimatedRotoUpdatedFourthPoint();
    void testAnimatedRotoUpdatedFifthPoint();
    void testAnimatedRotoUpdatedSixthPoint();
    void testAnimatedRotoUpdatedSeventhPoint();
    void testAnimatedRotoUpdatedEighthPoint();
    void testAnimatedRotoUpdatedNinthPoint();
    void testAnimatedRotoUpdatedTenthPoint();
    void testAnimatedRotoUpdatedEleventhPoint();
    void testAnimatedRotoUpdatedTwelvthPoint();
    void testAnimatedRotoUpdatedClosed();

    void testName();
    void testType();
    void testActive();
    void testHidden();
    void testDirection();

private:
    void loadTestData(const QByteArray &filename);
    void updateProperty(int frame);

    BMFreeFormShape *m_path = nullptr;
};

tst_BMPath::tst_BMPath()
{

}

tst_BMPath::~tst_BMPath()
{

}

void tst_BMPath::initTestCase()
{
}

void tst_BMPath::cleanupTestCase()
{
    if (m_path)
        delete m_path;
}

void tst_BMPath::testStaticCurveInitialStartPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 15);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticCurveInitialSecondPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 28);
    QVERIFY(qRound(el.y) == 85);
}

void tst_BMPath::testStaticCurveInitialThirdPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticCurveInitialFourthPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 51);
    QVERIFY(qRound(el.y) == 38);
}

void tst_BMPath::testStaticCurveInitialFifthPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 90);
    QVERIFY(qRound(el.y) == 25);
}

void tst_BMPath::testStaticCurveInitialSixthPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 85);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticCurveUpdatedStartPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 15);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticCurveUpdatedSecondPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 28);
    QVERIFY(qRound(el.y) == 85);
}

void tst_BMPath::testStaticCurveUpdatedThirdPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticCurveUpdatedFourthPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 51);
    QVERIFY(qRound(el.y) == 38);
}

void tst_BMPath::testStaticCurveUpdatedFifthPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 90);
    QVERIFY(qRound(el.y) == 25);
}

void tst_BMPath::testStaticCurveUpdatedSixthPoint()
{
    loadTestData("freeform_curve_static.json");

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 85);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticTriangleInitialStartPoint()
{
    loadTestData("freeform_triangle_static.json");

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 50);
    QVERIFY(qRound(el.y) == 30);
}

void tst_BMPath::testStaticTriangleInitialSecondPoint()
{
    loadTestData("freeform_triangle_static.json");

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticTriangleInitialThirdPoint()
{
    loadTestData("freeform_triangle_static.json");

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticTriangleInitialClosed()
{
    loadTestData("freeform_triangle_static.json");

    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testStaticTriangleUpdatedStartPoint()
{
    loadTestData("freeform_triangle_static.json");

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 50);
    QVERIFY(qRound(el.y) == 30);
}

void tst_BMPath::testStaticTriangleUpdatedSecondPoint()
{
    loadTestData("freeform_triangle_static.json");

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticTriangleUpdatedThirdPoint()
{
    loadTestData("freeform_triangle_static.json");
    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testStaticTriangleUpdatedClosed()
{
    loadTestData("freeform_triangle_static.json");
    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testStaticRotoInitialStartPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 40);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testStaticRotoInitialSecondPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(1);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testStaticRotoInitialThirdPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 55);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testStaticRotoInitialFourthPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testStaticRotoInitialFifthPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 55);
}

void tst_BMPath::testStaticRotoInitialSixthPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 45);
}

void tst_BMPath::testStaticRotoInitialSeventhPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 40);
}

void tst_BMPath::testStaticRotoInitialEighthPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(7);
    QVERIFY(qRound(el.x) == 55);
    QVERIFY(qRound(el.y) == 35);
}

void tst_BMPath::testStaticRotoInitialNinthPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(8);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 35);
}

void tst_BMPath::testStaticRotoInitialTenthPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(9);
    QVERIFY(qRound(el.x) == 40);
    QVERIFY(qRound(el.y) == 40);
}

void tst_BMPath::testStaticRotoInitialEleventhPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(10);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 45);
}

void tst_BMPath::testStaticRotoInitialTwelvthPoint()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el = m_path->path().elementAt(11);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 55);
}

void tst_BMPath::testStaticRotoInitialClosed()
{
    loadTestData("freeform_roto_static.json");

    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testStaticRotoUpdatedStartPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 40);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testStaticRotoUpdatedSecondPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(1);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testStaticRotoUpdatedThirdPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 55);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testStaticRotoUpdatedFourthPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testStaticRotoUpdatedFifthPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 55);
}

void tst_BMPath::testStaticRotoUpdatedSixthPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 45);
}

void tst_BMPath::testStaticRotoUpdatedSeventhPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 40);
}

void tst_BMPath::testStaticRotoUpdatedEighthPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(7);
    QVERIFY(qRound(el.x) == 55);
    QVERIFY(qRound(el.y) == 35);
}

void tst_BMPath::testStaticRotoUpdatedNinthPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(8);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 35);
}

void tst_BMPath::testStaticRotoUpdatedTenthPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(9);
    QVERIFY(qRound(el.x) == 40);
    QVERIFY(qRound(el.y) == 40);
}

void tst_BMPath::testStaticRotoUpdatedEleventhPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(10);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 45);
}

void tst_BMPath::testStaticRotoUpdatedTwelvthPoint()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(11);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 55);
}

void tst_BMPath::testStaticRotoUpdatedClosed()
{
    loadTestData("freeform_roto_static.json");
    updateProperty(179);

    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testAnimatedCurveInitialStartPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 15);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedCurveInitialSecondPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 28);
    QVERIFY(qRound(el.y) == 85);
}

void tst_BMPath::testAnimatedCurveInitialThirdPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedCurveInitialFourthPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 51);
    QVERIFY(qRound(el.y) == 38);
}

void tst_BMPath::testAnimatedCurveInitialFifthPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 90);
    QVERIFY(qRound(el.y) == 25);
}

void tst_BMPath::testAnimatedCurveInitialSixthPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 85);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedCurveUpdatedStartPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 15);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedCurveUpdatedSecondPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 28);
    QVERIFY(qRound(el.y) == 5);
}

void tst_BMPath::testAnimatedCurveUpdatedThirdPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedCurveUpdatedFourthPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 90);
}

void tst_BMPath::testAnimatedCurveUpdatedFifthPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 12);
}

void tst_BMPath::testAnimatedCurveUpdatedSixthPoint()
{
    loadTestData("freeform_curve_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 85);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedTriangleInitialStartPoint()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 50);
    QVERIFY(qRound(el.y) == 30);
}

void tst_BMPath::testAnimatedTriangleInitialSecondPoint()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedTriangleInitialThirdPoint()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedTriangleInitialClosed()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(0);

    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testAnimatedTriangleUpdatedStartPoint()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 50);
    QVERIFY(qRound(el.y) == 79);
}

void tst_BMPath::testAnimatedTriangleUpdatedSecondPoint()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testAnimatedTriangleUpdatedThirdPoint()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testAnimatedTriangleUpdatedClosed()
{
    loadTestData("freeform_triangle_animated.json");
    updateProperty(179);

    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testAnimatedRotoInitialStartPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 40);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testAnimatedRotoInitialSecondPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(1);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testAnimatedRotoInitialThirdPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 55);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testAnimatedRotoInitialFourthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testAnimatedRotoInitialFifthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 55);
}

void tst_BMPath::testAnimatedRotoInitialSixthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 45);
}

void tst_BMPath::testAnimatedRotoInitialSeventhPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 40);
}

void tst_BMPath::testAnimatedRotoInitialEighthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(7);
    QVERIFY(qRound(el.x) == 55);
    QVERIFY(qRound(el.y) == 35);
}

void tst_BMPath::testAnimatedRotoInitialNinthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(8);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 35);
}

void tst_BMPath::testAnimatedRotoInitialTenthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(9);
    QVERIFY(qRound(el.x) == 40);
    QVERIFY(qRound(el.y) == 40);
}

void tst_BMPath::testAnimatedRotoInitialEleventhPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(10);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 45);
}

void tst_BMPath::testAnimatedRotoInitialTwelvthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el = m_path->path().elementAt(11);
    QVERIFY(qRound(el.x) == 35);
    QVERIFY(qRound(el.y) == 55);
}

void tst_BMPath::testAnimatedRotoInitialClosed()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(0);

    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testAnimatedRotoUpdatedStartPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(0);
    QVERIFY(qRound(el.x) == 40);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testAnimatedRotoUpdatedSecondPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(1);
    QVERIFY(qRound(el.x) == 45);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testAnimatedRotoUpdatedThirdPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(2);
    QVERIFY(qRound(el.x) == 55);
    QVERIFY(qRound(el.y) == 65);
}

void tst_BMPath::testAnimatedRotoUpdatedFourthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(3);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 60);
}

void tst_BMPath::testAnimatedRotoUpdatedFifthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(4);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 55);
}

void tst_BMPath::testAnimatedRotoUpdatedSixthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(5);
    QVERIFY(qRound(el.x) == 65);
    QVERIFY(qRound(el.y) == 45);
}

void tst_BMPath::testAnimatedRotoUpdatedSeventhPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(6);
    QVERIFY(qRound(el.x) == 60);
    QVERIFY(qRound(el.y) == 40);
}

void tst_BMPath::testAnimatedRotoUpdatedEighthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(7);
    QVERIFY(qRound(el.x) == 57);
    QVERIFY(qRound(el.y) == 37);
}

void tst_BMPath::testAnimatedRotoUpdatedNinthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(8);
    QVERIFY(qRound(el.x) == 53);
    QVERIFY(qRound(el.y) == 47);
}

void tst_BMPath::testAnimatedRotoUpdatedTenthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(9);
    QVERIFY(qRound(el.x) == 50);
    QVERIFY(qRound(el.y) == 50);
}

void tst_BMPath::testAnimatedRotoUpdatedEleventhPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(10);
    QVERIFY(qRound(el.x) == 47);
    QVERIFY(qRound(el.y) == 53);
}

void tst_BMPath::testAnimatedRotoUpdatedTwelvthPoint()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el = m_path->path().elementAt(11);
    QVERIFY(qRound(el.x) == 37);
    QVERIFY(qRound(el.y) == 57);
}

void tst_BMPath::testAnimatedRotoUpdatedClosed()
{
    loadTestData("freeform_roto_animated.json");
    updateProperty(179);

    QPainterPath::Element el1 = m_path->path().elementAt(0);
    QPainterPath::Element el2 = m_path->path().elementAt(m_path->path().elementCount()-1);
    QVERIFY(qFuzzyCompare(el1.x, el2.x));
    QVERIFY(qFuzzyCompare(el1.y, el2.y));
}

void tst_BMPath::testName()
{
    loadTestData("freeform_curve_static.json");
    QVERIFY(m_path->name() == QString("Path 1"));
}

void tst_BMPath::testType()
{
    loadTestData("freeform_curve_static.json");
    QVERIFY(m_path->type() == BM_SHAPE_SHAPE_IX);
}

void tst_BMPath::testActive()
{
    loadTestData("freeform_curve_static.json");
    QVERIFY(m_path->active(100) == true);

    loadTestData("freeform_hidden.json");
    QVERIFY(m_path->active(100) == false);
}

void tst_BMPath::testHidden()
{
    loadTestData("freeform_hidden.json");
    QVERIFY(m_path->hidden() == true);
}

void tst_BMPath::testDirection()
{
    loadTestData("freeform_hidden.json");
    QVERIFY(m_path->direction() == 0);
    loadTestData("freeform_direction.json");
    QVERIFY(m_path->direction() == 0);
}

void tst_BMPath::loadTestData(const QByteArray &filename)
{
    if (m_path) {
        delete m_path;
        m_path = nullptr;
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
        if (shape->type() == BM_SHAPE_SHAPE_IX)
            break;
        shapesIt++;
    }

    m_path = static_cast<BMFreeFormShape*>(shape);

    QVERIFY(m_path != nullptr);
}

void tst_BMPath::updateProperty(int frame)
{
    m_path->updateProperties(frame);
}

QTEST_MAIN(tst_BMPath)
#include "tst_bmpath.moc"
