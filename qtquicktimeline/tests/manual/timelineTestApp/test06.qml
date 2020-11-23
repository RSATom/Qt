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
