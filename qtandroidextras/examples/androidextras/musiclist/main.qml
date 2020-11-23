/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtAndroidExtras module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.14

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt Android Music List")

    Text {
        id: resultText
        height: 40
        text: "Music library list:"
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        font.pointSize: 20
        verticalAlignment: Text.AlignTop
    }

    ToolSeparator {
        id: toolSeparator
        width: parent.width
        anchors.bottom: listView.top
        anchors.bottomMargin: 10
        anchors.top: resultText.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
        orientation: Qt.Horizontal
    }

    ListView {
        id: listView
        width: parent.width
        clip: true
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: resultText.bottom
        anchors.topMargin: 30
        anchors.bottom: parent.bottom

        model: musiclist
        delegate: Rectangle {
            height: 25
            width: ListView.view.width
            color: index % 2 == 0 ? "#EDEDED" : "#FFFFFF"

            function toTimeFormat(sec_num) {
                sec_num = Math.floor(sec_num / 1000);
                var hours   = Math.floor(sec_num / 3600);
                var minutes = Math.floor((sec_num - (hours * 3600)) / 60);
                var seconds = sec_num - (hours * 3600) - (minutes * 60);

                if (hours < 10) { hours = "0" + hours; }
                if (minutes < 10) { minutes = "0" + minutes; }
                if (seconds < 10) { seconds = "0" + seconds; }

                return hours+':'+minutes+':'+seconds;
            }

            Text { text: model.modelData.title + " | " + model.modelData.artist + " | " + toTimeFormat(model.modelData.duration) }
        }
    }
}
