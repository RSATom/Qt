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

import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Timeline 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Loader {
        id: loader
        anchors.fill: parent
    }

    Row {
        x: 8
        y: 457

        Text {
            text: "Test 01"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test01.qml"
            }
        }

        Text {
            text: "Test 02"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test02.qml"
            }
        }

        Text {
            text: "Test 03"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test03.qml"
            }
        }

        Text {
            text: "Test 04"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test04.qml"
            }
        }

        Text {
            text: "Test 05"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test05.qml"
            }
        }

        Text {
            text: "Test 06"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test06.qml"
            }
        }

        Text {
            text: "Test 07"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test07.qml"
            }
        }

        Text {
            text: "Test 08"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test08.qml"
            }
        }

        Text {
            text: "Test 09"
            font.pixelSize: 12
            MouseArea {
                anchors.fill: parent
                onClicked: loader.source = "test09.qml"
            }
        }
    }
}
