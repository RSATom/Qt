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
