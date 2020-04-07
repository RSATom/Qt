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
    width: 640
    height: 480

    Timeline {
        id: keyframeMutator
        enabled: true
        endFrame: 1000

        KeyframeGroup {
            target: rectangle
            property: "x"
            Keyframe {
                frame: 0
                value: 447
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 500
                value: 220
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 1000
                value: -9
            }
        }

        KeyframeGroup {
            target: rectangle
            property: "y"
            Keyframe {
                frame: 0
                value: 140
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 500
                value: 140
            }

            Keyframe {
                frame: 1000
                value: 140
            }
        }

        KeyframeGroup {
            target: rectangle1
            property: "x"
            Keyframe {
                frame: 0
                value: 220
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 500
                value: -7
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 1000
                value: -236
            }
        }

        KeyframeGroup {
            target: rectangle1
            property: "y"
            Keyframe {
                frame: 0
                value: 140
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 500
                value: 140
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 1000
                value: 140
            }
        }

        KeyframeGroup {
            target: rectangle2
            property: "x"
            Keyframe {
                frame: 0
                value: 676
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 500
                value: 449
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 1000
                value: 220
            }
        }

        KeyframeGroup {
            target: rectangle2
            property: "y"
            Keyframe {
                frame: 0
                value: 140
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 500
                value: 140
            }

            Keyframe {
                easing.bezierCurve: [0.42,0.00,0.58,1.00,1,1]
                frame: 1000
                value: 140
            }
        }

        KeyframeGroup {
            target: rectangle1
            property: "scale"

            Keyframe {
                frame: 0
                value: 1
            }

            Keyframe {
                frame: 500
                value: 0.5
            }
        }

        KeyframeGroup {
            target: rectangle
            property: "scale"

            Keyframe {
                frame: 0
                value: 0.5
            }

            Keyframe {
                frame: 500
                value: 1
            }

            Keyframe {
                frame: 1000
                value: 0.5
            }
        }

        KeyframeGroup {
            target: rectangle2
            property: "scale"

            Keyframe {
                frame: 500
                value: 0.5
            }

            Keyframe {
                frame: 1000
                value: 1
            }
        }
    }

    Rectangle {
        id: rectangle
        x: 220
        y: 140
        width: 200
        height: 200
        color: "#868686"
    }

    Rectangle {
        id: rectangle1
        x: -7
        y: 140
        width: 200
        height: 200
        color: "#747474"
    }

    Rectangle {
        id: rectangle2
        x: 449
        y: 140
        width: 200
        height: 200
        color: "#767676"
    }

    PropertyAnimation {
        id: propertyAnimation
        target: keyframeMutator
        property: "currentFrame"
        running: true
        to: keyframeMutator.endFrame
        duration: 1000
        loops: -1
        from: keyframeMutator.startFrame
    }
}
