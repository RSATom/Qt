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
    Rectangle {
        width: 40
        height: 40
        color: "blue"
        MouseArea {
            anchors.fill: parent
            onClicked: timeline.enabled = !timeline.enabled
        }
    }

    Rectangle {
        x: 80
        width: 40
        height: 40
        color: "blue"
        MouseArea {
            anchors.fill: parent
            onClicked: animation.restart()
        }
    }

    Item {
        width: 480
        height: 480

        Timeline {
            id: timeline

            startFrame: 0
            endFrame: 100
            currentFrame: 50

            enabled: true

            animations: [
                TimelineAnimation {
                    id: animation
                    duration: 2000
                    from: 0
                    to: 100
                    running: false
                }

            ]

            KeyframeGroup {
                target: rectangle
                property: "x"

                Keyframe {
                    frame: 0
                    value: 0
                }

                Keyframe {
                    frame: 50
                    value: 100
                }

                Keyframe {
                    frame: 100
                    value: 200
                }
            }

            KeyframeGroup {
                target: rectangle
                property: "y"

                Keyframe {
                    frame: 0
                    value: 0
                }

                Keyframe {
                    frame: 50
                    value: 100
                    easing.type: Easing.InBounce
                }

                Keyframe {
                    frame: 100
                    value: 200
                }
            }

            KeyframeGroup {
                target: rectangle
                property: "color"

                Keyframe {
                    frame: 0
                    value: "red"
                }

                Keyframe {
                    frame: 50
                    value: "blue"
                }

                Keyframe {
                    frame: 100
                    value: "yellow"
                }
            }
        }

        Rectangle {
            id: rectangle
            width: 20
            height: 20
            color: "red"
        }
    }
}
