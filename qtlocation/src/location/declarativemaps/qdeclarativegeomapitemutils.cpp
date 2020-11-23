/****************************************************************************
**
** Copyright (C) 2020 Paolo Angelelli <paolo.angelelli@gmail.com>
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativegeomapitemutils_p.h"
#include <QtPositioning/private/qdoublevector3d_p.h>
#include <QtPositioning/private/qdoublematrix4x4_p.h>
#include <QtPositioning/QGeoCoordinate>
#include <QPointF>
#include <QMatrix4x4>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QtGui/private/qvectorpath_p.h>
#include <QtGui/private/qtriangulatingstroker_p.h>
#include <QtGui/private/qtriangulator_p.h>
#include <QtPositioning/private/qclipperutils_p.h>

QT_BEGIN_NAMESPACE

void QDeclarativeGeoMapItemUtils::wrapPath(const QList<QGeoCoordinate> &perimeter,
                                           const QGeoCoordinate &geoLeftBound,
                                           const QGeoProjectionWebMercator &p,
                                           QList<QDoubleVector2D> &wrappedPath,
                                           QList<QDoubleVector2D> &wrappedPathMinus1,
                                           QList<QDoubleVector2D> &wrappedPathPlus1,
                                           QDoubleVector2D *leftBoundWrapped)
{
    QList<QDoubleVector2D> path;
    for (const QGeoCoordinate &c : perimeter)
        path << p.geoToMapProjection(c);
    const QDoubleVector2D leftBound = p.geoToMapProjection(geoLeftBound);
    wrappedPath.clear();
    wrappedPathPlus1.clear();
    wrappedPathMinus1.clear();
    // compute 3 sets of "wrapped" coordinates: one w regular mercator, one w regular mercator +- 1.0
    for (int i = 0; i < path.size(); ++i) {
        QDoubleVector2D coord = path.at(i);

        // We can get NaN if the map isn't set up correctly, or the projection
        // is faulty -- probably best thing to do is abort
        if (!qIsFinite(coord.x()) || !qIsFinite(coord.y()))
            return;

        const bool isPointLessThanUnwrapBelowX = (coord.x() < leftBound.x());
        // unwrap x to preserve geometry if moved to border of map
        if (isPointLessThanUnwrapBelowX)
            coord.setX(coord.x() + 1.0);

        QDoubleVector2D coordP1(coord.x() + 1.0, coord.y());
        QDoubleVector2D coordM1(coord.x() - 1.0, coord.y());

        wrappedPath.append(coord);
        wrappedPathPlus1.append(coordP1);
        wrappedPathMinus1.append(coordM1);
    }
    if (leftBoundWrapped)
        *leftBoundWrapped = leftBound;
}

void QDeclarativeGeoMapItemUtils::wrapPath(const QList<QGeoCoordinate> &perimeter,
                                           const QGeoCoordinate &geoLeftBound,
                                           const QGeoProjectionWebMercator &p,
                                           QList<QDoubleVector2D> &wrappedPath,
                                           QDoubleVector2D *leftBoundWrapped)
{
    QList<QDoubleVector2D> path;
    for (const QGeoCoordinate &c : perimeter)
        path << p.geoToMapProjection(c);
    const QDoubleVector2D leftBound = p.geoToMapProjection(geoLeftBound);
    wrapPath(path, leftBound,wrappedPath);
    if (leftBoundWrapped)
        *leftBoundWrapped = leftBound;
}

void QDeclarativeGeoMapItemUtils::wrapPath(const QList<QDoubleVector2D> &path,
                                           const QDoubleVector2D &geoLeftBound,
                                           QList<QDoubleVector2D> &wrappedPath)
{
    wrappedPath.clear();
    // compute 3 sets of "wrapped" coordinates: one w regular mercator, one w regular mercator +- 1.0
    for (int i = 0; i < path.size(); ++i) {
        QDoubleVector2D coord = path.at(i);

        // We can get NaN if the map isn't set up correctly, or the projection
        // is faulty -- probably best thing to do is abort
        if (!qIsFinite(coord.x()) || !qIsFinite(coord.y()))
            return;

        const bool isPointLessThanUnwrapBelowX = (coord.x() < geoLeftBound.x());
        // unwrap x to preserve geometry if moved to border of map
        if (isPointLessThanUnwrapBelowX)
            coord.setX(coord.x() + 1.0);

        wrappedPath.append(coord);
    }
}

void QDeclarativeGeoMapItemUtils::clipPolygon(const QList<QDoubleVector2D> &wrappedPath, const QGeoProjectionWebMercator &p, QList<QList<QDoubleVector2D> > &clippedPaths, QDoubleVector2D *leftBoundWrapped, const bool closed)
{
    // 2) Clip bounding box
    clippedPaths.clear();
    const QList<QDoubleVector2D> &visibleRegion = p.projectableGeometry();
    if (visibleRegion.size()) {
        c2t::clip2tri clipper;
        clipper.addSubjectPath(QClipperUtils::qListToPath(wrappedPath), closed);
        clipper.addClipPolygon(QClipperUtils::qListToPath(visibleRegion));
        Paths res = clipper.execute(c2t::clip2tri::Intersection, QtClipperLib::pftEvenOdd, QtClipperLib::pftEvenOdd);
        clippedPaths = QClipperUtils::pathsToQList(res);

        if (leftBoundWrapped) {
            // 2.1) update srcOrigin_ and leftBoundWrapped with the point with minimum X
            QDoubleVector2D lb(qInf(), qInf());
            for (const QList<QDoubleVector2D> &path: clippedPaths)
                for (const QDoubleVector2D &p: path)
                    if (p.x() < lb.x() || (p.x() == lb.x() && p.y() < lb.y()))
                        // y-minimization needed to find the same point on polygon and border
                        lb = p;

            if (qIsInf(lb.x())) // e.g., when the polygon is clipped entirely
                return;

            // 2.2) Prevent the conversion to and from clipper from introducing tiny negative offsets which,
            //      in turn will make the geometry wrap around.
            lb.setX(qMax(leftBoundWrapped->x(), lb.x()));

            *leftBoundWrapped = lb;
            // srcOrigin_ = p.mapProjectionToGeo(p.unwrapMapProjection(lb));
        }
    } else {
        clippedPaths.append(wrappedPath);
    }
}

void QDeclarativeGeoMapItemUtils::projectBbox(const QList<QDoubleVector2D> &clippedBbox, const QGeoProjectionWebMercator &p, QPainterPath &projectedBbox)
{
    projectedBbox = QPainterPath(); // clear() is added in 5.13..
    for (int i = 0; i < clippedBbox.size(); ++i) {
        QDoubleVector2D point = p.wrappedMapProjectionToItemPosition(clippedBbox.at(i));
        if (i == 0)
            projectedBbox.moveTo(point.toPointF());
        else
            projectedBbox.lineTo(point.toPointF());
    }
    projectedBbox.closeSubpath();
}

QT_END_NAMESPACE
