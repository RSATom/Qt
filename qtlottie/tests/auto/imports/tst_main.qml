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

import QtQuick 2.9
import QtQuick.Window 2.2
import QtTest 1.0
import Qt.labs.lottieqt 1.0

Item {
    id: animContainer
    width: childrenRect.width
    height: childrenRect.height

    LottieAnimation {
        id: bmAnim

        x: 0
        y: 0
        quality: LottieAnimation.HighQuality
        source: "rec_pos_col_opa.json"

        onFinished: {
            bmAnim.start();
        }


    }

    TestCase{
        id: testID
        name: "testAnimation"

        function test_1_loadingStatus(){
            compare(bmAnim.status, 2);      /* status: Null(0), Loading(1), Ready(2), Error(3)
                                            if the source has been loaded successfully, the status
                                            must be "Ready"
                                            */
        }

        function test_2_initialFrameRate(){
            compare(bmAnim.frameRate, 60);  /* frame rate of the source used in this test is 60 */
        }

        function test_3_changeFrameRate(){
            bmAnim.frameRate = 30;
            compare(bmAnim.frameRate, 30);
        }

        function test_4_initialQuality(){
            compare(bmAnim.quality, 2);     /* quality: LowQuality(0), MediumQuality(1), HighQuality(2)
                                            lottieanimation's initial quality is set to HighQuality
                                            */
        }

        function test_5_changeQuality(){
            bmAnim.quality = 1;             /* change quality to MediumQuality */
            compare(bmAnim.quality, 1);
        }


    }
}
