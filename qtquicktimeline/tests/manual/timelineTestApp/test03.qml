/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd
** All rights reserved.
** For any questions to The Qt Company, please use contact form at http://www.qt.io/contact-us
**
** This file is part of the Qt Enterprise Qt Quick Timeline Add-on.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company.
**
** If you have questions regarding the use of this file, please use
** contact form at http://www.qt.io/contact-us
**
****************************************************************************/

import QtQuick 2.0
import QtQuick.Timeline 1.0

Item {
    id: item1

    Timeline {
        id: timeline
        enabled: true

        startFrame: 0
        endFrame: 200
        currentFrame: input.text
        KeyframeGroup {
            target: needle
            property: "rotation"
            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 100
                value: 90
            }
            Keyframe {
                frame: 200
                value: 180
            }
        }

        KeyframeGroup {
            target: needle
            property: "color"
            Keyframe {
                frame: 0
                value: "blue"
            }

            Keyframe {
                frame: 100
                value: "green"
            }
            Keyframe {
                frame: 200
                value: "red"
            }
        }

    }

    Rectangle {
        id: rectangle
        x: 220
        y: 140
        width: 300
        height: 300
        color: "#000000"
        radius: 150
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: needle
            x: 0
            y: 148
            width: 150
            height: 4
            color: "#c41616"
            transformOrigin: Item.Right
        }
    }

    TextInput {
        id: input
        x: 207
        y: 392
        width: 227
        height: 65
        text: "10"
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: 14
    }
}
