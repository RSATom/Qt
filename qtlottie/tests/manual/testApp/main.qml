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
import Qt.labs.lottieqt 1.0

Window {
    visible: true
    width: animContainer.width
    height: animContainer.height
    title: qsTr("Animation test")

    property bool showFps: false

    color: "black"

    Item {
        id: animContainer
        width: childrenRect.width
        height: childrenRect.height

        Repeater {
            model: 1

            LottieAnimation {
                id: bmAnim

                x: 10 * index
                y: 10 * index
                loops: LottieAnimation.Infinite
                quality: LottieAnimation.MediumQuality
                source: "rect_rotate.json"
            }
        }
    }

    Text {
        id: text

        property real t
        property int frame: 0

        anchors.right: parent.right
        color: "red"
        visible: showFps
        text: "FPS: " + fpsTimer.fps

        Timer {
            id: fpsTimer
            property real fps: 0
            repeat: true
            interval: 1000
            running: showFps
            onTriggered: {
                parent.text = "FPS: " + fpsTimer.fps
                fps = text.frame
                text.frame = 0
            }
        }

        NumberAnimation on t {
            id: tAnim
            from: 0
            to: 1000
            running: showFps
            loops: Animation.Infinite
        }

        onTChanged: {
            update() // force continuous animation
            text.frame++;
        }
    }

    Rectangle {
        id: rot

        width: 10
        height: 100
        anchors.centerIn: parent
        color: "blue"
        enabled: showFps
        visible: enabled

        PropertyAnimation {
            target: rot
            property: "rotation"
            from: 0
            to: 360
            duration: 500
            running: true
            loops: Animation.Infinite
        }
    }

    PropertyAnimation {
        target: rot
        property: "rotation"
        from: 0
        to: 360
        duration: 500
        running: true
        loops: Animation.Infinite
    }
}
