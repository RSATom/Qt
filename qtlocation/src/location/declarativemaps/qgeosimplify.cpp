/****************************************************************************
**
** Qt adaptation of geosimplify-js
** Copyright (C) 2017 Daniel Patterson
** See 3rdParty/geosimplify.js for the original license.
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

#include "qgeosimplify_p.h"
#include <QtPositioning/private/qlocationutils_p.h>

QT_BEGIN_NAMESPACE

double QGeoSimplify::getDist(const QGeoCoordinate &p1, const QGeoCoordinate &p2)
{
    return p1.distanceTo(p2);
}

QDoubleVector2D QGeoSimplify::closestPoint(const QDoubleVector2D &p, const QDoubleVector2D &a, const QDoubleVector2D &b)
{
    if (a == b)
        return a;

    const double u = ((p.x() - a.x()) * (b.x() - a.x()) + (p.y() - a.y()) * (b.y() - a.y()) ) / (b - a).lengthSquared();
    const QDoubleVector2D intersection(a.x() + u * (b.x() - a.x()) , a.y() + u * (b.y() - a.y()) );
    QDoubleVector2D candidate = ( (p-a).length() < (p-b).length() ) ? a : b;
    if (u > 0 && u < 1
            && (p-intersection).length() < (p-candidate).length()  ) // And it falls in the segment
        candidate = intersection;
    return candidate;
}

QGeoCoordinate QGeoSimplify::closestPoint(const QGeoCoordinate &pc, const QGeoCoordinate &ac, const QGeoCoordinate &bc, const double &leftBound)
{
    QDoubleVector2D p = QWebMercator::coordToMercator(pc);
    if (p.x() < leftBound)
        p.setX(p.x() + leftBound); // unwrap X

    QDoubleVector2D a = QWebMercator::coordToMercator(ac);
    if (a.x() < leftBound)
        a.setX(a.x() + leftBound);  // unwrap X

    QDoubleVector2D b = QWebMercator::coordToMercator(bc);
    if (b.x() < leftBound)
        b.setX(b.x() + leftBound);  // unwrap X

    QDoubleVector2D intersection = closestPoint(p, a, b);
    if (intersection.x() > 1.0)
        intersection.setX(intersection.x() - leftBound); // wrap X

    const QGeoCoordinate closest = QWebMercator::mercatorToCoord(intersection);
    return closest;
}

double QGeoSimplify::getSegDist(const QGeoCoordinate &pc, const QGeoCoordinate &ac, const QGeoCoordinate &bc, const double &leftBound)
{
    const QGeoCoordinate closest = closestPoint(pc, ac, bc, leftBound);
    const double distanceMeters = pc.distanceTo(closest);
    return distanceMeters;
}

double QGeoSimplify::getSegDist(const QDoubleVector2D &p, const QDoubleVector2D &a, const QDoubleVector2D &b, const double &leftBound)
{
    QDoubleVector2D intersection = closestPoint(p, a, b);
    return getDist(intersection, p, leftBound);
}

void QGeoSimplify::simplifyDPStep(const QList<QGeoCoordinate> &points, const double &leftBound, int first, int last, double offsetTolerance, QList<QGeoCoordinate> &simplified)
{
    double maxDistanceFound = offsetTolerance;
    int index = 0;

    for (int i = first + 1; i < last; i++) {
        const double distance = getSegDist(points.at(i),
                                           points.at(first),
                                           points.at(last),
                                           leftBound);

        if (distance > maxDistanceFound) {
            index = i;
            maxDistanceFound = distance;
        }
    }

    if (index > 0) {
        if (index - first > 1)
            simplifyDPStep(points,
                           leftBound,
                           first,
                           index,
                           offsetTolerance,
                           simplified);
        simplified.append(points.at(index));
        if (last - index > 1)
            simplifyDPStep(points,
                           leftBound,
                           index,
                           last,
                           offsetTolerance,
                           simplified);
    }
}

double QGeoSimplify::getDist(QDoubleVector2D a, QDoubleVector2D b, const double &leftBound)
{
    if (a.x() > 1.0)
        a.setX(a.x() - leftBound); // wrap X
    if (b.x() > 1.0)
        b.setX(b.x() - leftBound); // wrap X
    return QWebMercator::mercatorToCoord(a).distanceTo(
                QWebMercator::mercatorToCoord(b));
}

void QGeoSimplify::simplifyDPStep(const QList<QDoubleVector2D> &points,
                                  const double &leftBound,
                                  int first,
                                  int last,
                                  double offsetTolerance,
                                  QList<QDoubleVector2D> &simplified)
{
    double maxDistanceFound = offsetTolerance;
    int index = 0;

    for (int i = first + 1; i < last; i++) {
        const double distance = getSegDist(points.at(i),
                                           points.at(first),
                                           points.at(last),
                                           leftBound);

        if (distance > maxDistanceFound) {
            index = i;
            maxDistanceFound = distance;
        }
    }

    if (index > 0) {
        if (index - first > 1)
            simplifyDPStep(points,
                           leftBound,
                           first,
                           index,
                           offsetTolerance,
                           simplified);
        simplified.append(points.at(index));
        if (last - index > 1)
            simplifyDPStep(points,
                           leftBound,
                           index,
                           last,
                           offsetTolerance,
                           simplified);
    }
}

static double pixelDistanceAtZoomAndLatitude(int zoom, double latitude)
{
    const double den = double((1 << (zoom + 8)));
    const double pixelDist = (QLocationUtils::earthMeanCircumference() *
                                std::cos(QLocationUtils::radians(latitude))) / den;
    return pixelDist;
}

static QGeoCoordinate unwrappedToGeo(QDoubleVector2D p, double leftBound)
{
    if (p.x() > 1.0)
        p.setX(p.x() - leftBound);
    return QWebMercator::mercatorToCoord(p);
}

void QGeoSimplify::simplifyDPStepZL(const QList<QDoubleVector2D> &points,
                                  const double &leftBound,
                                  int first,
                                  int last,
                                  int zoomLevel,
                                  QList<QDoubleVector2D> &simplified)
{
    const QGeoCoordinate firstC = unwrappedToGeo(points.at(first), leftBound);
    const QGeoCoordinate lastC = unwrappedToGeo(points.at(last), leftBound);
    double maxDistanceFound = (pixelDistanceAtZoomAndLatitude(zoomLevel, firstC.latitude())
                        + pixelDistanceAtZoomAndLatitude(zoomLevel, lastC.latitude())) * 0.5;
    int index = 0;

    for (int i = first + 1; i < last; i++) {
        const double distance = getSegDist(points.at(i),
                                           points.at(first),
                                           points.at(last),
                                           leftBound);

        if (distance > maxDistanceFound) {
            index = i;
            maxDistanceFound = distance;
        }
    }

    if (index > 0) {
        if (index - first > 1)
            simplifyDPStepZL(points,
                           leftBound,
                           first,
                           index,
                           zoomLevel,
                           simplified);
        simplified.append(points.at(index));
        if (last - index > 1)
            simplifyDPStepZL(points,
                           leftBound,
                           index,
                           last,
                           zoomLevel,
                           simplified);
    }
}

QList<QGeoCoordinate> QGeoSimplify::simplifyDouglasPeucker(const QList<QGeoCoordinate> &points,
                                                           const double &leftBound,
                                                           double offsetTolerance) {
    const int last = points.size() - 1;
    QList<QGeoCoordinate> simplified { points.first() };
    simplifyDPStep(points, leftBound, 0, last, offsetTolerance, simplified);
    simplified.append(points.at(last));
    return simplified;
}

QList<QDoubleVector2D> QGeoSimplify::simplifyDouglasPeucker(const QList<QDoubleVector2D> &points,
                                                            const double &leftBound,
                                                            double offsetTolerance) {
    const int last = points.size() - 1;
    QList<QDoubleVector2D> simplified { points.first() };
    simplifyDPStep(points, leftBound, 0, last, offsetTolerance, simplified);
    simplified.append(points.at(last));
    return simplified;
}

QList<QDoubleVector2D> QGeoSimplify::simplifyDouglasPeuckerZL(const QList<QDoubleVector2D> &points,
                                                            const double &leftBound,
                                                            int zoomLevel)
{
    const int last = points.size() - 1;
    QList<QDoubleVector2D> simplified { points.first() };
    simplifyDPStepZL(points, leftBound, 0, last, zoomLevel, simplified);
    simplified.append(points.at(last));
    return simplified;
}

QList<QGeoCoordinate> QGeoSimplify::geoSimplify(const QList<QGeoCoordinate> &points,
                                             const double &leftBound,
                                             double offsetTolerance)    // also in meters
{
    if (points.size() <= 2)
        return points;
    return simplifyDouglasPeucker(points,
                                  leftBound,
                                  offsetTolerance);
}

QList<QDoubleVector2D> QGeoSimplify::geoSimplify(const QList<QDoubleVector2D> &points,
                                              const double &leftBound,
                                              double offsetTolerance)    // also in meters
{
    if (points.size() <= 2)
        return points;
    return simplifyDouglasPeucker(points,
                                  leftBound,
                                  offsetTolerance);
}

QList<QDoubleVector2D> QGeoSimplify::geoSimplifyZL(const QList<QDoubleVector2D> &points,
                                                 const double &leftBound,
                                                 int zoomLevel)
{
    if (points.size() <= 2)
        return points;
    return simplifyDouglasPeuckerZL(points,
                                  leftBound,
                                  zoomLevel);
}


QT_END_NAMESPACE

