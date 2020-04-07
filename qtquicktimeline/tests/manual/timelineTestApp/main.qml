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
