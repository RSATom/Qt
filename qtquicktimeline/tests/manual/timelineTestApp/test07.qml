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

    Text {
        id: text1
        x: 70
        y: 93
        text: "Count 01"
        font.pixelSize: 12
    }

    Timeline {
        id: timeline
        enabled: true
        endFrame: 1000
        startFrame: 0

        KeyframeGroup {
            target: text1
            property: "text"

            Keyframe {
                frame: 0
                value: "Count 01"
            }

            Keyframe {
                frame: 100
                value: "Count 10"
            }

            Keyframe {
                frame: 200
                value: "Count 20"
            }

            Keyframe {
                frame: 300
                value: "Count 30"
            }

            Keyframe {
                frame: 400
                value: "Count 40"
            }

            Keyframe {
                frame: 500
                value: "Count 50"
            }

            Keyframe {
                frame: 600
                value: "Count 60"
            }

            Keyframe {
                frame: 700
                value: "Count 70"
            }

            Keyframe {
                frame: 800
                value: "Count 80"
            }

            Keyframe {
                frame: 900
                value: "Count 90"
            }

            Keyframe {
                frame: 1000
                value: "Count 100"
            }
        }
    }

    NumberAnimation {
        id: numberAnimation
        target: timeline
        property: "currentFrame"
        running: true
        to: timeline.endFrame
        duration: 1000
        from: timeline.startFrame
        loops: -1
    }
}

/*##^## Designer {
    D{i:1;timeline_expanded:true}D{i:2;currentFrame__AT__NodeInstance:1}
}
 ##^##*/
