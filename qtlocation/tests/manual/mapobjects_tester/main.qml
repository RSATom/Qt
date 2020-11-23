/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2 as C2
import QtPositioning 5.6
import QtLocation 5.15
import Qt.labs.location 1.0

Window {
    id: win
    objectName: "win"
    visible: true
    width: 1440
    height: 720
    title: qsTr("MapObjects tester")


    property real initialZL: 5
    property var initialCenter:  QtPositioning.coordinate(66.9961, -175.012)
    property real rotation : 0

    Shortcut {
        sequence: "Ctrl+R"
        onActivated: {
            rotation = 57
        }
    }

    function syncMaps(mFrom, mTo, propName)
    {
        if (mTo[propName] !== mFrom[propName]) {
            mTo[propName] = mFrom[propName]
        }
    }

    Plugin {
        id: osm // use only one plugin, to avoid messing up the cache
        name: "osm"
    }
    Rectangle {
        id: mapContainer
        rotation: win.rotation
        anchors.fill: parent
        color: "lightsteelblue"

        Map {
            id: map
            rotation: win.rotation
            gesture.enabled: true
            objectName: "map1"
            anchors {
                bottom: parent.bottom
                top: parent.top
                left: parent.left
                right: parent.horizontalCenter
            }

            onCenterChanged:        syncMaps(map, map2, "center")
            onTiltChanged:          syncMaps(map, map2, "tilt")
            onBearingChanged:       syncMaps(map, map2, "bearing")
            onZoomLevelChanged:     syncMaps(map, map2, "zoomLevel")
            onFieldOfViewChanged:   syncMaps(map, map2, "fieldOfView")

            opacity: 1.0
            color: 'transparent'
            plugin: osm
            center: initialCenter
            activeMapType: map.supportedMapTypes[2]
            zoomLevel: initialZL
            z : parent.z + 1
            copyrightsVisible: false

            Component.onCompleted: {
                var o = movComponent.createObject(map1MainMOV)
                map1MainMOV.addMapObject(o);
            }
            MapObjectView {
                id: map1MainMOV
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mouse.accepted = false
                    var crd = map.toCoordinate(Qt.point(mouse.x, mouse.y))
                    var s = crd.toString(0)
                    console.log("Clicked on ",s)
                }
            }
        }
        Map {
            id: map2
            rotation: win.rotation
            gesture.enabled: true
            objectName: "map2"
            anchors {
                bottom: parent.bottom
                top: parent.top
                left: parent.horizontalCenter
                right: parent.right
            }

            onCenterChanged:        syncMaps(map2, map, "center")
            onTiltChanged:          syncMaps(map2, map, "tilt")
            onBearingChanged:       syncMaps(map2, map, "bearing")
            onZoomLevelChanged:     syncMaps(map2, map, "zoomLevel")
            onFieldOfViewChanged:   syncMaps(map2, map, "fieldOfView")

            color: 'transparent'
            plugin: osm
            activeMapType: map.supportedMapTypes[2]
            center: initialCenter
            zoomLevel: initialZL
            copyrightsVisible: false

            Component.onCompleted: {
                var o = migComponent.createObject(map2)
                o.glPolygons  = Qt.binding(function() {return switchPolygons2.checked})
                o.glPolylines  = Qt.binding(function() {return switchPolylines2.currentText})
                o.glCircles  = Qt.binding(function() {return switchCircles2.checked})
                o.glRectangles  = Qt.binding(function() {return switchRectangles2.checked})
                map2.addMapItemGroup(o);
            }

            C2.Switch {
                text: qsTr("OGL Polygons")
                id: switchPolygons2
                checked: false
                anchors {
                    top: parent.top
                    right: parent.right
                    topMargin: 12
                    rightMargin: 12
                }
            }
            C2.ComboBox {
                model: ['Software','OpenGL','Triangulated']
                id: switchPolylines2
                anchors {
                    top: switchPolygons2.bottom
                    right: parent.right
                    topMargin: 12
                    rightMargin: 12
                }
                onCurrentTextChanged: console.log("CURRENT TEXT CHANGED ",currentText)
            }
            C2.Switch {
                text: qsTr("OGL Circles")
                id: switchCircles2
                anchors {
                    top: switchPolylines2.bottom
                    right: parent.right
                    topMargin: 12
                    rightMargin: 12
                }
            }
            C2.Switch {
                text: qsTr("OGL Rectangles")
                id: switchRectangles2
                anchors {
                    top: switchCircles2.bottom
                    right: parent.right
                    topMargin: 12
                    rightMargin: 12
                }
            }
        }
    }

    Component {
        id: migComponent
        MapItemGroup {
            id: polyGroup
            property bool glPolygons : true
            property string glPolylines : "Software"
            property bool glCircles : true
            property bool glRectangles : true
            objectName: parent.objectName + "_MIG_"
            function polylineBackend()
            {
                return (polyGroup.glPolylines === "OpenGL")
                       ? MapPolyline.OpenGLLineStrip
                       :  ((polyGroup.glPolylines === "Software")
                           ? MapPolyline.Software : MapPolyline.OpenGLExtruded)
            }

            function polygonBackend()
            {
                return (polyGroup.glPolygons)
                       ? MapPolygon.OpenGL : MapPolygon.Software
            }

            function rectangleBackend()
            {
                return (polyGroup.glRectangles)
                       ? MapRectangle.OpenGL : MapRectangle.Software
            }

            function circleBackend()
            {
                return (polyGroup.glCircles)
                       ? MapCircle.OpenGL : MapCircle.Software
            }
            MapPolyline {
                id: tstPolyLine // to verify the polygon stays where it's supposed to
                line.color: 'black'
                objectName: parent.objectName + "black"
                line.width: 1
                opacity: 1.0
                backend: polylineBackend()
                path: [
                    { latitude: 76.9965, longitude: -175.012 },
                    { latitude: 26.9965, longitude: -175.012 }
                ]
            }

            MapPolyline {
                id: timeline
                line.color: "red"
                objectName: parent.objectName + "timeline"
                line.width: 4
                backend: polylineBackend()
                path: [
                    { latitude: 90, longitude: 180 },
                    { latitude: -90, longitude: -180 }
                ]
            }

            MapPolygon {
                id: poly1
                color: "red"
                objectName: parent.objectName + "red"
                backend: polygonBackend()
                path: [
                    { latitude: 55, longitude: 170 },
                    { latitude: 66.9965, longitude: -175.012 },
                    { latitude: 55, longitude: -160 },
                    { latitude: 40, longitude: -165 },
                    { latitude: 45, longitude: 178 }
                ]
                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("poly1 in "+parent.parent.objectName + "clicked")

                    Rectangle { // this is technically unsupported, but practically works.
                        color: "transparent"
                        border.color: "red"
                        anchors.fill: parent
                    }
                }
            }

            MapPolygon {
                id: selfIntersectingPolygon
                color: 'darkmagenta'
                objectName: parent.objectName + "darkmagenta"
                backend: polygonBackend()
                opacity: 1.0
                path: [
                    { latitude: 19, longitude: 49 },
                    { latitude: 18, longitude: 49 },
                    { latitude: 18, longitude: 51 },
                    { latitude: 20, longitude: 51 },
                    { latitude: 20, longitude: 50 },
                    { latitude: 18.5, longitude: 50 },
                    { latitude: 18.5, longitude: 52 },
                    { latitude: 19, longitude: 52 }
                ]

                MouseArea{
                    anchors.fill: parent
                    drag.target: parent // This one might glitch as the tessellation done by earcut might be
                                        // inaccurate, so clicking in an area that is covered by earcut but
                                        // does not exist will not trigger the panning

                    Rectangle { // this is technically unsupported, but practically works.
                        color: "transparent"
                        border.color: "red"
                        anchors.fill: parent
                    }
                }
            }

            MapPolygon {
                id: poly2
                color: "green"
                border.color: "black"
                border.width: 8
                objectName: parent.objectName + "green"
                backend: polygonBackend()
                path: [
                    { latitude: -45, longitude: -170 },
                    { latitude: -55, longitude: -155 },
                    { latitude: -45, longitude: -130 },
                    { latitude: -35, longitude: -155 }
                ]
                MouseArea{
                    anchors.fill: parent
                    drag.target: parent
                    Rectangle { // this is technically unsupported, but practically works.
                        color: "transparent"
                        border.color: "red"
                        anchors.fill: parent
                    }
                }
            }

            MapPolygon {
                id: poly3
                color: "deepskyblue"
                objectName: parent.objectName + "deepskyblue"
                backend: polygonBackend()
                opacity: 0.2
                path: [
                    { latitude: 65, longitude: -20 },
                    { latitude: 75, longitude: 140 },
                    { latitude: 65, longitude: 80 },
                    { latitude: 55, longitude: -30 }
                ]
            }

            MapRectangle {
                id: rect
                color: 'tomato'
                border.color: 'black'
                border.width: 6
                topLeft: QtPositioning.coordinate(10,-10)
                bottomRight: QtPositioning.coordinate(-10,10)
                backend: rectangleBackend()
                MouseArea {
                    anchors.fill: parent
                    Rectangle {
                        color: "transparent"
                        border.color: "red"
                        border.width: 1
                        anchors.fill: parent
                    }
                }
            }

            LongPolyline {
                id: longPoly
                backend: polylineBackend()
                Component.onCompleted: longPolyPath = path
            }

            MapCircle {
                center: QtPositioning.coordinate(52, 0)
                radius: sliRadius.value
                color: 'deepskyblue'
                border.width: 6
                border.color: 'firebrick'
                backend: circleBackend()
                MouseArea {
                    anchors.fill: parent
                    Rectangle {
                        color: "transparent"
                        border.color: "red"
                        border.width: 1
                        anchors.fill: parent
                    }
                }
            }

            MapCircle {
                id: circle1
                border.color: 'deepskyblue'
                border.width: 26
                backend: circleBackend()
                center: QtPositioning.coordinate(17, 44);
                radius: 200*1000
                color: "firebrick"
//                layer.enabled: (backend == MapCircle.Software)
//                layer.samples: 4
            }
        }
    }

    Component {
        id: movComponent
        MapObjectView {
            id: polyGroup
            MapPolylineObject {
                id: tstPolyLine // to verify the polygon stays where it's supposed to
                line.color: 'black'
                objectName: parent.objectName + "black"
                line.width: 1
                path: [
                    { latitude: 76.9965, longitude: -175.012 },
                    { latitude: 26.9965, longitude: -175.012 }
                ]
            }

            MapPolylineObject {
                id: timeline
                line.color: "red"
                objectName: parent.objectName + "timeline"
                line.width: 4
                path: [
                    { latitude: 90, longitude: 180 },
                    { latitude: -90, longitude: -180 }
                ]
            }

            MapPolygonObject {
                id: poly1
                color: "red"
                objectName: parent.objectName + "red"
                path: [
                    { latitude: 55, longitude: 170 },
                    { latitude: 66.9965, longitude: -175.012 },
                    { latitude: 55, longitude: -160 },
                    { latitude: 40, longitude: -165 },
                    { latitude: 45, longitude: 178 }
                ]
            }

            MapPolygonObject {
                id: selfIntersectingPolygon
                color: 'darkmagenta'
                objectName: parent.objectName + "darkmagenta"
                path: [
                    { latitude: 19, longitude: 49 },
                    { latitude: 18, longitude: 49 },
                    { latitude: 18, longitude: 51 },
                    { latitude: 20, longitude: 51 },
                    { latitude: 20, longitude: 50 },
                    { latitude: 18.5, longitude: 50 },
                    { latitude: 18.5, longitude: 52 },
                    { latitude: 19, longitude: 52 }
                ]
            }

            MapPolygonObject {
                id: poly2
                color: "green"
                border.color: "black"
                border.width: 8
                objectName: parent.objectName + "green"
                path: [
                    { latitude: -45, longitude: -170 },
                    { latitude: -55, longitude: -155 },
                    { latitude: -45, longitude: -130 },
                    { latitude: -35, longitude: -155 }
                ]
            }

            MapPolygonObject {
                id: poly3
                color: Qt.rgba(0, 191.0/255.0, 1, 0.3) //"deepskyblue"
                objectName: parent.objectName + "deepskyblue"
                path: [
                    { latitude: 65, longitude: -20 },
                    { latitude: 75, longitude: 140 },
                    { latitude: 65, longitude: 80 },
                    { latitude: 55, longitude: -30 }
                ]
            }

            MapCircleObject {
                center: QtPositioning.coordinate(52, 0)
                radius: sliRadius.value
                color: 'deepskyblue'
                border.width: 6
                border.color: 'firebrick'
            }

            MapPolylineObject {
                id: longPolyline
                line.color: "firebrick"
                objectName: parent.objectName + "longPolyline"
                line.width: 10
                path: longPolyPath
            }

            MapCircleObject {
                id: circle1
                border.color: 'deepskyblue'
                border.width: 26
                center: QtPositioning.coordinate(17, 44);
                radius: 200*1000
                color: "firebrick"
            }
        }
    }

    property var longPolyPath

    C2.Slider {
        id: sliRadius
        orientation: Qt.Vertical
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            topMargin: 10
            leftMargin: 10
            bottomMargin: 10
        }
        from: 30 * 1000
        to: 600 * 1000
        value: 100 * 1000
    }
}
