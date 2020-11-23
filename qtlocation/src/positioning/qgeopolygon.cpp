/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtPositioning module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qgeopolygon.h"
#include "qgeopolygon_p.h"
#include "qgeopath_p.h"
#include "qgeocircle.h"

#include "qgeocoordinate.h"
#include "qnumeric.h"
#include "qlocationutils_p.h"
#include "qwebmercator_p.h"

#include "qdoublevector2d_p.h"
#include "qdoublevector3d_p.h"
#include "qwebmercator_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGeoPolygon
    \inmodule QtPositioning
    \ingroup QtPositioning-positioning
    \since 5.10

    \brief The QGeoPolygon class defines a geographic polygon.

    The polygon is defined by an ordered list of QGeoCoordinates representing its perimeter.

    Each two adjacent elements in this list are intended to be connected
    together by the shortest line segment of constant bearing passing
    through both elements.
    This type of connection can cross the date line in the longitudinal direction,
    but never crosses the poles.

    This is relevant for the calculation of the bounding box returned by
    \l QGeoShape::boundingGeoRectangle() for this shape, which will have the latitude of
    the top left corner set to the maximum latitude in the path point set.
    Similarly, the latitude of the bottom right corner will be the minimum latitude
    in the path point set.

    This class is a \l Q_GADGET.
    It can be \l{Cpp_value_integration_positioning}{directly used from C++ and QML}.
*/

/*
    \property QGeoPolygon::path
    \brief This property holds the list of coordinates for the geo polygon.

    The polygon is both invalid and empty if it contains no coordinate.

    A default constructed QGeoPolygon is therefore invalid.
*/

inline QGeoPolygonPrivate *QGeoPolygon::d_func()
{
    return static_cast<QGeoPolygonPrivate *>(d_ptr.data());
}

inline const QGeoPolygonPrivate *QGeoPolygon::d_func() const
{
    return static_cast<const QGeoPolygonPrivate *>(d_ptr.constData());
}

struct PolygonVariantConversions
{
    PolygonVariantConversions()
    {
        QMetaType::registerConverter<QGeoShape, QGeoPolygon>();
        QMetaType::registerConverter<QGeoPolygon, QGeoShape>();
    }
};

Q_GLOBAL_STATIC(PolygonVariantConversions, initPolygonConversions)

/*!
    Constructs a new, empty geo polygon.
*/
QGeoPolygon::QGeoPolygon()
:   QGeoShape(new QGeoPolygonPrivate())
{
    initPolygonConversions();
}

/*!
    Constructs a new geo polygon from the coordinates specified
    in \a path.
*/
QGeoPolygon::QGeoPolygon(const QList<QGeoCoordinate> &path)
:   QGeoShape(new QGeoPolygonPrivate(path))
{
    initPolygonConversions();
}

/*!
    Constructs a new geo polygon from the contents of \a other.
*/
QGeoPolygon::QGeoPolygon(const QGeoPolygon &other)
:   QGeoShape(other)
{
    initPolygonConversions();
}

static void calculatePeripheralPoints(QList<QGeoCoordinate> &path,
                                      const QGeoCircle &circle,
                                      int steps)
{
    const QGeoCoordinate &center = circle.center();
    const qreal distance = circle.radius();
    // Calculate points based on great-circle distance
    // Calculation is the same as GeoCoordinate's atDistanceAndAzimuth function
    // but tweaked here for computing multiple points

    // pre-calculations
    steps = qMax(steps, 3);
    qreal centerLon = center.longitude();
    qreal latRad = QLocationUtils::radians(center.latitude());
    qreal lonRad = QLocationUtils::radians(centerLon);
    qreal cosLatRad = std::cos(latRad);
    qreal sinLatRad = std::sin(latRad);
    qreal ratio = (distance / QLocationUtils::earthMeanRadius());
    qreal cosRatio = std::cos(ratio);
    qreal sinRatio = std::sin(ratio);
    qreal sinLatRad_x_cosRatio = sinLatRad * cosRatio;
    qreal cosLatRad_x_sinRatio = cosLatRad * sinRatio;
    for (int i = 0; i < steps; ++i) {
        qreal azimuthRad = 2 * M_PI * i / steps;
        qreal resultLatRad = std::asin(sinLatRad_x_cosRatio
                                   + cosLatRad_x_sinRatio * std::cos(azimuthRad));
        qreal resultLonRad = lonRad + std::atan2(std::sin(azimuthRad) * cosLatRad_x_sinRatio,
                                       cosRatio - sinLatRad * std::sin(resultLatRad));
        qreal lat2 = QLocationUtils::degrees(resultLatRad);
        qreal lon2 = QLocationUtils::wrapLong(QLocationUtils::degrees(resultLonRad));

        path << QGeoCoordinate(lat2, lon2, center.altitude());
    }
}

/*!
    Constructs a new geo polygon from the contents of \a other.
*/
QGeoPolygon::QGeoPolygon(const QGeoShape &other)
:   QGeoShape(other)
{
    initPolygonConversions();
    if (type() != QGeoShape::PolygonType) {
        QGeoPolygonPrivate *poly = new QGeoPolygonPrivate();
        if (type() == QGeoShape::CircleType) {
            const QGeoCircle &circle = static_cast<const QGeoCircle &>(other);
            QList<QGeoCoordinate> perimeter;
            calculatePeripheralPoints(perimeter, circle, 128);
            poly->setPath(perimeter);
        } else if (type() == QGeoShape::RectangleType) {
            const QGeoRectangle &rect = static_cast<const QGeoRectangle &>(other);
            QList<QGeoCoordinate> perimeter;
            perimeter << rect.topLeft() << rect.topRight()
                      << rect.bottomRight() << rect.bottomLeft();
            poly->setPath(perimeter);
        }
        d_ptr = poly;
    }
}

/*!
    Destroys this polygon.
*/
QGeoPolygon::~QGeoPolygon() {}

/*!
    Assigns \a other to this geo polygon and returns a reference to this geo polygon.
*/
QGeoPolygon &QGeoPolygon::operator=(const QGeoPolygon &other)
{
    QGeoShape::operator=(other);
    return *this;
}

/*!
    Returns whether this geo polygon is equal to \a other.
*/
bool QGeoPolygon::operator==(const QGeoPolygon &other) const
{
    Q_D(const QGeoPolygon);
    return *d == *other.d_func();
}

/*!
    Returns whether this geo polygon is not equal to \a other.
*/
bool QGeoPolygon::operator!=(const QGeoPolygon &other) const
{
    Q_D(const QGeoPolygon);
    return !(*d == *other.d_func());
}

/*!
    Sets the \a path for the polygon.
*/
void QGeoPolygon::setPath(const QList<QGeoCoordinate> &path)
{
    Q_D(QGeoPolygon);
    return d->setPath(path);
}

/*!
    Returns all the elements of the polygon's boundary.
*/
const QList<QGeoCoordinate> &QGeoPolygon::path() const
{
    Q_D(const QGeoPolygon);
    return d->path();
}

/*!
    Sets all the elements of the polygon's perimeter
    based on a list of coordinates (\a path).
.

    \since QtPositioning 5.12
*/
void QGeoPolygon::setPerimeter(const QVariantList &path)
{
    Q_D(QGeoPolygon);
    QList<QGeoCoordinate> p;
    for (const auto &c: path) {
        if (c.canConvert<QGeoCoordinate>())
            p << c.value<QGeoCoordinate>();
    }
    d->setPath(p);
}

/*!
    Returns all the elements of the polygon's perimeter.

    \since QtPositioning 5.12
*/
QVariantList QGeoPolygon::perimeter() const
{
    Q_D(const QGeoPolygon);
    QVariantList p;
    for (const auto &c: d->path())
        p << QVariant::fromValue(c);
    return p;
}

/*!
    Translates this geo polygon by \a degreesLatitude northwards and \a degreesLongitude eastwards.

    Negative values of \a degreesLatitude and \a degreesLongitude correspond to
    southward and westward translation respectively.
*/
void QGeoPolygon::translate(double degreesLatitude, double degreesLongitude)
{
    Q_D(QGeoPolygon);
    d->translate(degreesLatitude, degreesLongitude);
}

/*!
    Returns a copy of this geo polygon translated by \a degreesLatitude northwards and
    \a degreesLongitude eastwards.

    Negative values of \a degreesLatitude and \a degreesLongitude correspond to
    southward and westward translation respectively.

    \sa translate()
*/
QGeoPolygon QGeoPolygon::translated(double degreesLatitude, double degreesLongitude) const
{
    QGeoPolygon result(*this);
    result.translate(degreesLatitude, degreesLongitude);
    return result;
}

/*!
    Returns the length of the polygon's perimeter, in meters, from the element \a indexFrom to the element \a indexTo.
    The length is intended to be the sum of the shortest distances for each pair of adjacent points.
*/
double QGeoPolygon::length(int indexFrom, int indexTo) const
{
    Q_D(const QGeoPolygon);
    return d->length(indexFrom, indexTo);
}

/*!
    Returns the number of elements in the polygon.

    \since 5.10
*/
int QGeoPolygon::size() const
{
    Q_D(const QGeoPolygon);
    return d->size();
}

/*!
    Appends \a coordinate to the polygon.
*/
void QGeoPolygon::addCoordinate(const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPolygon);
    d->addCoordinate(coordinate);
}

/*!
    Inserts \a coordinate at the specified \a index.
*/
void QGeoPolygon::insertCoordinate(int index, const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPolygon);
    d->insertCoordinate(index, coordinate);
}

/*!
    Replaces the path element at the specified \a index with \a coordinate.
*/
void QGeoPolygon::replaceCoordinate(int index, const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPolygon);
    d->replaceCoordinate(index, coordinate);
}

/*!
    Returns the coordinate at \a index .
*/
QGeoCoordinate QGeoPolygon::coordinateAt(int index) const
{
    Q_D(const QGeoPolygon);
    return d->coordinateAt(index);
}

/*!
    Returns true if the polygon's perimeter contains \a coordinate as one of the elements.
*/
bool QGeoPolygon::containsCoordinate(const QGeoCoordinate &coordinate) const
{
    Q_D(const QGeoPolygon);
    return d->containsCoordinate(coordinate);
}

/*!
    Removes the last occurrence of \a coordinate from the polygon.
*/
void QGeoPolygon::removeCoordinate(const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPolygon);
    d->removeCoordinate(coordinate);
}

/*!
    Removes element at position \a index from the polygon.
*/
void QGeoPolygon::removeCoordinate(int index)
{
    Q_D(QGeoPolygon);
    d->removeCoordinate(index);
}

/*!
    Returns the geo polygon properties as a string.
*/
QString QGeoPolygon::toString() const
{
    if (type() != QGeoShape::PolygonType) {
        qWarning("Not a polygon");
        return QStringLiteral("QGeoPolygon(not a polygon)");
    }

    QString pathString;
    for (const auto &p : path())
        pathString += p.toString() + QLatin1Char(',');

    return QStringLiteral("QGeoPolygon([ %1 ])").arg(pathString);
}

/*!
   Sets the \a holePath for a hole inside the polygon. The hole is a
   QVariant containing a QList<QGeoCoordinate>.

   \since 5.12
*/
void QGeoPolygon::addHole(const QVariant &holePath)
{
    Q_D(QGeoPolygon);
    QList<QGeoCoordinate> qgcHolePath;
    if (holePath.canConvert<QVariantList>()) {
        const QVariantList qvlHolePath = holePath.toList();
        for (const QVariant &vertex : qvlHolePath) {
            if (vertex.canConvert<QGeoCoordinate>())
                qgcHolePath << vertex.value<QGeoCoordinate>();
        }
    }
    //ToDo: add QGeoShape support
    return d->addHole(qgcHolePath);
}

/*!
   Overloaded method. Sets the \a holePath for a hole inside the polygon. The hole is a QList<QGeoCoordinate>.

   \since 5.12
*/
void QGeoPolygon::addHole(const QList<QGeoCoordinate> &holePath)
{
    Q_D(QGeoPolygon);
    return d->addHole(holePath);
}

/*!
    Returns a QVariant containing a QVariant containing a QList<QGeoCoordinate>
    which represents the hole at \a index.

    \since 5.12
*/
const QVariantList QGeoPolygon::hole(int index) const
{
    Q_D(const QGeoPolygon);
    QVariantList holeCoordinates;
    for (const QGeoCoordinate &coords: d->holePath(index))
        holeCoordinates << QVariant::fromValue(coords);
    return holeCoordinates;
}

/*!
    Returns a QList<QGeoCoordinate> which represents the hole at \a index.

    \since 5.12
*/
const QList<QGeoCoordinate> QGeoPolygon::holePath(int index) const
{
    Q_D(const QGeoPolygon);
    return d->holePath(index);
}

/*!
    Removes element at position \a index from the holes QList.

    \since 5.12
*/
void QGeoPolygon::removeHole(int index)
{
    Q_D(QGeoPolygon);
    return d->removeHole(index);
}

/*!
    Returns the number of holes.

    \since 5.12
*/
int QGeoPolygon::holesCount() const
{
    Q_D(const QGeoPolygon);
    return d->holesCount();
}

/*******************************************************************************
 *
 * QGeoPathPrivate & friends
 *
*******************************************************************************/

QGeoPolygonPrivate::QGeoPolygonPrivate()
:   QGeoPathPrivate()
{
    type = QGeoShape::PolygonType;
}

QGeoPolygonPrivate::QGeoPolygonPrivate(const QList<QGeoCoordinate> &path)
:   QGeoPathPrivate(path)
{
    type = QGeoShape::PolygonType;
}

QGeoPolygonPrivate::~QGeoPolygonPrivate() {}

QGeoShapePrivate *QGeoPolygonPrivate::clone() const
{
    return new QGeoPolygonPrivate(*this);
}

bool QGeoPolygonPrivate::isValid() const
{
    return path().size() > 2;
}

bool QGeoPolygonPrivate::contains(const QGeoCoordinate &coordinate) const
{
    return polygonContains(coordinate);
}

inline static void translatePoly(   QList<QGeoCoordinate> &m_path,
                                    QList<QList<QGeoCoordinate>> &m_holesList,
                                    QGeoRectangle &m_bbox,
                                    double degreesLatitude,
                                    double degreesLongitude,
                                    double m_maxLati,
                                    double m_minLati)
{
    if (degreesLatitude > 0.0)
        degreesLatitude = qMin(degreesLatitude, 90.0 - m_maxLati);
    else
        degreesLatitude = qMax(degreesLatitude, -90.0 - m_minLati);
    for (QGeoCoordinate &p: m_path) {
        p.setLatitude(p.latitude() + degreesLatitude);
        p.setLongitude(QLocationUtils::wrapLong(p.longitude() + degreesLongitude));
    }
    if (!m_holesList.isEmpty()){
        for (QList<QGeoCoordinate> &hole: m_holesList){
            for (QGeoCoordinate &holeVertex: hole){
                holeVertex.setLatitude(holeVertex.latitude() + degreesLatitude);
                holeVertex.setLongitude(QLocationUtils::wrapLong(holeVertex.longitude() + degreesLongitude));
            }
        }
    }
    m_bbox.translate(degreesLatitude, degreesLongitude);
}

void QGeoPolygonPrivate::translate(double degreesLatitude, double degreesLongitude)
{
    // Need min/maxLati, so update bbox
    QVector<double> m_deltaXs;
    double m_minX, m_maxX, m_minLati, m_maxLati;
    m_bboxDirty = false; // Updated in translatePoly
    computeBBox(m_path, m_deltaXs, m_minX, m_maxX, m_minLati, m_maxLati, m_bbox);
    translatePoly(m_path, m_holesList, m_bbox, degreesLatitude, degreesLongitude, m_maxLati, m_minLati);
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
    m_clipperDirty = true;
}

bool QGeoPolygonPrivate::operator==(const QGeoShapePrivate &other) const
{
    if (!QGeoShapePrivate::operator==(other)) // checks type
        return false;

    const QGeoPolygonPrivate &otherPath = static_cast<const QGeoPolygonPrivate &>(other);
    if (m_path.size() != otherPath.m_path.size()
            || m_holesList.size() != otherPath.m_holesList.size())
        return false;
    return  m_path == otherPath.m_path && m_holesList == otherPath.m_holesList;
}

void QGeoPolygonPrivate::addHole(const QList<QGeoCoordinate> &holePath)
{
    for (const QGeoCoordinate &holeVertex: holePath)
        if (!holeVertex.isValid())
            return;

    m_holesList << holePath;
    // ToDo: mark clipper dirty when hole caching gets added
}

const QList<QGeoCoordinate> QGeoPolygonPrivate::holePath(int index) const
{
    return m_holesList.at(index);
}

void QGeoPolygonPrivate::removeHole(int index)
{
    if (index < 0 || index >= m_holesList.size())
        return;

    m_holesList.removeAt(index);
    // ToDo: mark clipper dirty when hole caching gets added
}

int QGeoPolygonPrivate::holesCount() const
{
    return m_holesList.size();
}

bool QGeoPolygonPrivate::polygonContains(const QGeoCoordinate &coordinate) const
{
    if (m_clipperDirty)
        const_cast<QGeoPolygonPrivate *>(this)->updateClipperPath(); // this one updates bbox too if needed

    QDoubleVector2D coord = QWebMercator::coordToMercator(coordinate);

    if (coord.x() < m_leftBoundWrapped)
        coord.setX(coord.x() + 1.0);


    IntPoint intCoord = QClipperUtils::toIntPoint(coord);
    if (!c2t::clip2tri::pointInPolygon(intCoord, m_clipperPath))
        return false;

    // else iterates the holes List checking whether the point is contained inside the holes
    for (const QList<QGeoCoordinate> &holePath : qAsConst(m_holesList)) {
        // ToDo: cache these
        QGeoPolygon holePolygon;
        holePolygon.setPath(holePath);
        if (holePolygon.contains(coordinate))
            return false;
    }
    return true;
}

void QGeoPolygonPrivate::markDirty()
{
    m_bboxDirty = m_clipperDirty = true;
}

void QGeoPolygonPrivate::updateClipperPath()
{
    if (m_bboxDirty)
        computeBoundingBox();
    m_clipperDirty = false;

    QList<QDoubleVector2D> preservedPath;
    for (const QGeoCoordinate &c : m_path) {
        QDoubleVector2D crd = QWebMercator::coordToMercator(c);
        if (crd.x() < m_leftBoundWrapped)
            crd.setX(crd.x() + 1.0);
        preservedPath << crd;
    }
    m_clipperPath = QClipperUtils::qListToPath(preservedPath);
}

QGeoPolygonPrivateEager::QGeoPolygonPrivateEager() : QGeoPolygonPrivate()
{
    m_bboxDirty = false; // never dirty on the eager version
}

QGeoPolygonPrivateEager::QGeoPolygonPrivateEager(const QList<QGeoCoordinate> &path) : QGeoPolygonPrivate(path)
{
    m_bboxDirty = false; // never dirty on the eager version
}

QGeoPolygonPrivateEager::~QGeoPolygonPrivateEager()
{

}

QGeoShapePrivate *QGeoPolygonPrivateEager::clone() const
{
    return new QGeoPolygonPrivate(*this);
}

void QGeoPolygonPrivateEager::translate(double degreesLatitude, double degreesLongitude)
{
    translatePoly(m_path, m_holesList, m_bbox, degreesLatitude, degreesLongitude, m_maxLati, m_minLati);
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
    m_clipperDirty = true;
}

void QGeoPolygonPrivateEager::markDirty()
{
    m_clipperDirty = true;
    computeBoundingBox();
}

void QGeoPolygonPrivateEager::addCoordinate(const QGeoCoordinate &coordinate)
{
    if (!coordinate.isValid())
        return;
    m_path.append(coordinate);
    m_clipperDirty = true;
    updateBoundingBox(); // do not markDirty as it uses computeBoundingBox instead
}

void QGeoPolygonPrivateEager::computeBoundingBox()
{
    computeBBox(m_path, m_deltaXs, m_minX, m_maxX, m_minLati, m_maxLati, m_bbox);
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
}

void QGeoPolygonPrivateEager::updateBoundingBox()
{
    updateBBox(m_path, m_deltaXs, m_minX, m_maxX, m_minLati, m_maxLati, m_bbox);
}

QGeoPolygonEager::QGeoPolygonEager() : QGeoPolygon()
{
    initPolygonConversions();
    d_ptr = new QGeoPolygonPrivateEager;
}

QGeoPolygonEager::QGeoPolygonEager(const QList<QGeoCoordinate> &path) : QGeoPolygon()
{
    initPolygonConversions();
    d_ptr = new QGeoPolygonPrivateEager(path);
}

QGeoPolygonEager::QGeoPolygonEager(const QGeoPolygon &other) : QGeoPolygon()
{
    initPolygonConversions();
    // without being able to dynamic_cast the d_ptr, only way to be sure is to reconstruct a new QGeoPolygonPrivateEager
    d_ptr = new QGeoPolygonPrivateEager;
    setPath(other.path());
    for (int i = 0; i < other.holesCount(); i++)
        addHole(other.holePath(i));
}

QGeoPolygonEager::QGeoPolygonEager(const QGeoShape &other) : QGeoPolygon()
{
    initPolygonConversions();
    if (other.type() == QGeoShape::PolygonType)
        *this = QGeoPolygonEager(QGeoPolygon(other));
    else
        d_ptr = new QGeoPolygonPrivateEager;
}

QGeoPolygonEager::~QGeoPolygonEager()
{

}

QT_END_NAMESPACE
