/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Timeline Add-on.
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
