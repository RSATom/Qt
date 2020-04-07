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
    width: 640
    height: 480

    Timeline {
        id: keyframeMutator
        endFrame: 1000
        enabled: true

        animations: [
            TimelineAnimation {
                running: true
                duration: 1000
                loops: -1
                from: keyframeMutator.startFrame
                to: keyframeMutator.endFrame
            }
        ]

        KeyframeGroup {
            target: circle1
            property: "width"
            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 500
                value: 250
            }

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 50
            }
        }

        KeyframeGroup {
            target: circle1
            property: "height"
            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 500
                value: 250
            }

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 50
            }
        }

        KeyframeGroup {
            target: circle1
            property: "opacity"

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 500
                value: 1
            }

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 0
            }
        }

        KeyframeGroup {
            target: circle
            property: "opacity"

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 500
                value: 1
            }
        }

        KeyframeGroup {
            target: circle
            property: "width"

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 500
                value: 180
            }

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 80
            }
        }

        KeyframeGroup {
            target: circle
            property: "height"

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 500
                value: 180
            }

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 80
            }
        }

        KeyframeGroup {
            target: circle2
            property: "opacity"

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 0
            }
        }

        KeyframeGroup {
            target: circle2
            property: "width"

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 10
            }
        }

        KeyframeGroup {
            target: circle2
            property: "height"

            Keyframe {
                easing.bezierCurve: [0.65,0.05,0.35,1.00,1,1]
                frame: 1000
                value: 10
            }
        }
    }


    Circle {
        id: circle
        x: 283
        y: 202
        width: 75
        height: 76
        color: "#00ffffff"
        opacity: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        borderWidth: 4
        borderColor: "#808080"
    }

    Circle {
        id: circle1
        x: 280
        y: 206
        width: 75
        height: 76
        color: "#00ffffff"
        opacity: 0
        borderWidth: 4
        borderColor: "#808080"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    Circle {
        id: circle2
        x: 272
        y: 206
        width: 75
        height: 76
        color: "#00ffffff"
        borderWidth: 4
        borderColor: "#808080"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
