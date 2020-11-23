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
    id: root
    width: 640
    height: 480

    Timeline {
        id: timeline
        startFrame: 0
        endFrame: 1000
        enabled: true

        KeyframeGroup {
            target: rectangle2
            property: "visible"

            Keyframe {
                frame: 0
                value: false
            }

            Keyframe {
                frame: 729
                value: true
            }

            Keyframe {
                frame: 954
                value: false
            }
        }

        KeyframeGroup {
            target: rectangle3
            property: "visible"

            Keyframe {
                frame: 0
                value: false
            }

            Keyframe {
                frame: 793
                value: true
            }

            Keyframe {
                frame: 868
                value: false
            }
        }

        KeyframeGroup {
            target: rectangle1
            property: "visible"

            Keyframe {
                frame: 0
                value: false
            }

            Keyframe {
                frame: 470
                value: true
            }

            Keyframe {
                frame: 757
                value: false
            }
        }

        KeyframeGroup {
            target: rectangle
            property: "visible"

            Keyframe {
                frame: 0
                value: false
            }

            Keyframe {
                frame: 199
                value: true
            }

            Keyframe {
                frame: 546
                value: false
            }
        }
    }

    NumberAnimation {
        id: numberAnimation
        target: timeline
        property: "currentFrame"
        running: true
        loops: -1
        to: timeline.endFrame
        from: timeline.startFrame
        duration: 1000
    }

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 200
        height: 200
        color: "#f12929"
        visible: false
    }

    Rectangle {
        id: rectangle1
        x: 440
        y: 0
        width: 200
        height: 200
        color: "#2851bb"
        visible: false
    }

    Rectangle {
        id: rectangle2
        x: 0
        y: 280
        width: 200
        height: 200
        color: "#2fd21b"
        visible: false
    }

    Rectangle {
        id: rectangle3
        x: 440
        y: 280
        width: 200
        height: 200
        color: "#9119dd"
        visible: false
    }
}

/*##^## Designer {
    D{i:1;currentFrame__AT__NodeInstance:0}D{i:3;timeline_expanded:true}D{i:4;timeline_expanded:true}
D{i:5;timeline_expanded:true}D{i:6;timeline_expanded:true}
}
 ##^##*/
