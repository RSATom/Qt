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
