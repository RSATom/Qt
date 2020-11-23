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

#include "qgeopath.h"
#include "qgeopolygon.h"
#include "qgeopath_p.h"

#include "qgeocoordinate.h"
#include "qnumeric.h"
#include "qlocationutils_p.h"
#include "qwebmercator_p.h"

#include "qdoublevector2d_p.h"
#include "qdoublevector3d_p.h"
QT_BEGIN_NAMESPACE

/*!
    \class QGeoPath
    \inmodule QtPositioning
    \ingroup QtPositioning-positioning
    \since 5.9

    \brief The QGeoPath class defines a geographic path.

    The path is defined by an ordered list of QGeoCoordinates.

    Each two adjacent elements in the path are intended to be connected
    together by the shortest line segment of constant bearing passing
    through both elements.
    This type of connection can cross the dateline in the longitudinal direction,
    but never crosses the poles.

    This is relevant for the calculation of the bounding box returned by
    \l QGeoShape::boundingGeoRectangle() for this shape, which will have the latitude of
    the top left corner set to the maximum latitude in the path point set.
    Similarly, the latitude of the bottom right corner will be the minimum latitude
    in the path point set.

    This class is a \l Q_GADGET.
    It can be \l{Cpp_value_integration_positioning}{directly used from C++ and QML}.

    A QGeoPath is both invalid and empty if it contains no coordinate.

    \note A default constructed QGeoPath is both invalid and empty as it does not contain any coordinates.
*/

/*!
    \property QGeoPath::path
    \brief This property holds the list of coordinates for the geo path.

    \note The coordinates cannot be processed in place. To change the value
    of this property, retrieve the complete list of coordinates, process them,
    and assign the new value to the property.
*/

inline QGeoPathPrivate *QGeoPath::d_func()
{
    return static_cast<QGeoPathPrivate *>(d_ptr.data());
}

inline const QGeoPathPrivate *QGeoPath::d_func() const
{
    return static_cast<const QGeoPathPrivate *>(d_ptr.constData());
}

struct PathVariantConversions
{
    PathVariantConversions()
    {
        QMetaType::registerConverter<QGeoShape, QGeoPath>();
        QMetaType::registerConverter<QGeoPath, QGeoShape>();
    }
};

Q_GLOBAL_STATIC(PathVariantConversions, initPathConversions)

/*!
    Constructs a new, empty geo path.
*/
QGeoPath::QGeoPath()
:   QGeoShape(new QGeoPathPrivate())
{
    initPathConversions();
}

/*!
    Constructs a new geo path from a list of coordinates
    (\a path and \a width).
*/
QGeoPath::QGeoPath(const QList<QGeoCoordinate> &path, const qreal &width)
:   QGeoShape(new QGeoPathPrivate(path, width))
{
    initPathConversions();
}

/*!
    Constructs a new geo path from the contents of \a other.
*/
QGeoPath::QGeoPath(const QGeoPath &other)
:   QGeoShape(other)
{
    initPathConversions();
}

/*!
    Constructs a new geo path from the contents of \a other.
*/
QGeoPath::QGeoPath(const QGeoShape &other)
:   QGeoShape(other)
{
    initPathConversions();
    if (type() != QGeoShape::PathType)
        d_ptr = new QGeoPathPrivate();
}

/*!
    Destroys this path.
*/
QGeoPath::~QGeoPath() {}

/*!
    Assigns \a other to this geo path and returns a reference to this geo path.
*/
QGeoPath &QGeoPath::operator=(const QGeoPath &other)
{
    QGeoShape::operator=(other);
    return *this;
}

/*!
    Returns whether this geo path is equal to \a other.
*/
bool QGeoPath::operator==(const QGeoPath &other) const
{
    Q_D(const QGeoPath);
    return *d == *other.d_func();
}

/*!
    Returns whether this geo path is not equal to \a other.
*/
bool QGeoPath::operator!=(const QGeoPath &other) const
{
    Q_D(const QGeoPath);
    return !(*d == *other.d_func());
}

/*!
    Sets all the elements of the \a path.
*/
void QGeoPath::setPath(const QList<QGeoCoordinate> &path)
{
    Q_D(QGeoPath);
    return d->setPath(path);
}

/*!
    Returns all the elements of the path.
*/
const QList<QGeoCoordinate> &QGeoPath::path() const
{
    Q_D(const QGeoPath);
    return d->path();
}

/*!
    Clears the path.

    \since 5.12
*/
void QGeoPath::clearPath()
{
    Q_D(QGeoPath);
    d->clearPath();
}

/*!
    Sets all the elements of the path.

    \internal
*/
void QGeoPath::setVariantPath(const QVariantList &path)
{
    Q_D(QGeoPath);
    QList<QGeoCoordinate> p;
    for (const auto &c: path) {
        if (c.canConvert<QGeoCoordinate>())
            p << c.value<QGeoCoordinate>();
    }
    d->setPath(p);
}
/*!
    Returns all the elements of the path.

    \internal
*/
QVariantList QGeoPath::variantPath() const
{
    Q_D(const QGeoPath);
    QVariantList p;
    for (const auto &c: d->path())
        p << QVariant::fromValue(c);
    return p;
}


/*!
    \property QGeoPath::width

    \brief the width of the path in meters.
*/
void QGeoPath::setWidth(const qreal &width)
{
    Q_D(QGeoPath);
    d->setWidth(width);
}

/*!
    Returns the width of the path, in meters. This information is used in the \l contains method.
    The default value is 0.
*/
qreal QGeoPath::width() const
{
    Q_D(const QGeoPath);
    return d->width();
}

/*!
    Translates this geo path by \a degreesLatitude northwards and \a degreesLongitude eastwards.

    Negative values of \a degreesLatitude and \a degreesLongitude correspond to
    southward and westward translation respectively.
*/
void QGeoPath::translate(double degreesLatitude, double degreesLongitude)
{
    Q_D(QGeoPath);
    d->translate(degreesLatitude, degreesLongitude);
}

/*!
    Returns a copy of this geo path translated by \a degreesLatitude northwards and
    \a degreesLongitude eastwards.

    Negative values of \a degreesLatitude and \a degreesLongitude correspond to
    southward and westward translation respectively.

    \sa translate()
*/
QGeoPath QGeoPath::translated(double degreesLatitude, double degreesLongitude) const
{
    QGeoPath result(*this);
    result.translate(degreesLatitude, degreesLongitude);
    return result;
}

/*!
    Returns the length of the path, in meters, from the element \a indexFrom to the element \a indexTo.
    The length is intended to be the sum of the shortest distances for each pair of adjacent points.

    If \a indexTo is -1 (the default value), the length will be including the distance between last coordinate
    and the first (closed loop).
    To retrieve the length for the path, use 0 for \a indexFrom and \l QGeoPath::size() - 1 for \a indexTo.
*/
double QGeoPath::length(int indexFrom, int indexTo) const
{
    Q_D(const QGeoPath);
    return d->length(indexFrom, indexTo);
}

/*!
    Returns the number of elements in the path.

    \since 5.10
*/
int QGeoPath::size() const
{
    Q_D(const QGeoPath);
    return d->size();
}

/*!
    Appends \a coordinate to the path.
*/
void QGeoPath::addCoordinate(const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPath);
    d->addCoordinate(coordinate);
}

/*!
    Inserts \a coordinate at the specified \a index.
*/
void QGeoPath::insertCoordinate(int index, const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPath);
    d->insertCoordinate(index, coordinate);
}

/*!
    Replaces the path element at the specified \a index with \a coordinate.
*/
void QGeoPath::replaceCoordinate(int index, const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPath);
    d->replaceCoordinate(index, coordinate);
}

/*!
    Returns the coordinate at \a index .
*/
QGeoCoordinate QGeoPath::coordinateAt(int index) const
{
    Q_D(const QGeoPath);
    return d->coordinateAt(index);
}

/*!
    Returns true if the path contains \a coordinate as one of the elements.
*/
bool QGeoPath::containsCoordinate(const QGeoCoordinate &coordinate) const
{
    Q_D(const QGeoPath);
    return d->containsCoordinate(coordinate);
}

/*!
    Removes the last occurrence of \a coordinate from the path.
*/
void QGeoPath::removeCoordinate(const QGeoCoordinate &coordinate)
{
    Q_D(QGeoPath);
    d->removeCoordinate(coordinate);
}

/*!
    Removes element at position \a index from the path.
*/
void QGeoPath::removeCoordinate(int index)
{
    Q_D(QGeoPath);
    d->removeCoordinate(index);
}

/*!
    Returns the geo path properties as a string.
*/
QString QGeoPath::toString() const
{
    if (type() != QGeoShape::PathType) {
        qWarning("Not a path");
        return QStringLiteral("QGeoPath(not a path)");
    }

    QString pathString;
    for (const auto &p : path())
        pathString += p.toString() + QLatin1Char(',');

    return QStringLiteral("QGeoPath([ %1 ])").arg(pathString);
}

/*******************************************************************************
 *
 * QGeoPathPrivate & friends
 *
*******************************************************************************/

QGeoPathPrivate::QGeoPathPrivate()
:   QGeoShapePrivate(QGeoShape::PathType)
{

}

QGeoPathPrivate::QGeoPathPrivate(const QList<QGeoCoordinate> &path, const qreal width)
:   QGeoShapePrivate(QGeoShape::PathType)
{
    setPath(path);
    setWidth(width);
}

QGeoPathPrivate::~QGeoPathPrivate()
{

}

QGeoShapePrivate *QGeoPathPrivate::clone() const
{
    return new QGeoPathPrivate(*this);
}

bool QGeoPathPrivate::isValid() const
{
    return !isEmpty();
}

bool QGeoPathPrivate::isEmpty() const
{
    return path().isEmpty(); // this should perhaps return geometric emptiness, less than 2 points for line, or empty polygon for polygons
}

QGeoCoordinate QGeoPathPrivate::center() const
{
    return boundingGeoRectangle().center();
}

void QGeoPathPrivate::extendShape(const QGeoCoordinate &coordinate)
{
    if (!coordinate.isValid() || contains(coordinate))
        return;
    addCoordinate(coordinate);
}

bool QGeoPathPrivate::operator==(const QGeoShapePrivate &other) const
{
    if (!QGeoShapePrivate::operator==(other))
        return false;

    const QGeoPathPrivate &otherPath = static_cast<const QGeoPathPrivate &>(other);
    if (m_path.size() != otherPath.m_path.size())
        return false;
    return m_width == otherPath.m_width && m_path == otherPath.m_path;
}

const QList<QGeoCoordinate> &QGeoPathPrivate::path() const
{
    return m_path;
}

bool QGeoPathPrivate::lineContains(const QGeoCoordinate &coordinate) const
{
    // Unoptimized approach:
    // - consider each segment of the path
    // - project it into mercator space (rhumb lines are straight in mercator space)
    // - find closest point to coordinate
    // - unproject the closest point
    // - calculate coordinate to closest point distance with distanceTo()
    // - if not within lineRadius, advance
    //
    // To keep wrapping into the equation:
    //   If the mercator x value of a coordinate of the line, or the coordinate parameter, is less
    // than mercator(m_bbox).x, add that to the conversion.

    if (m_bboxDirty)
        const_cast<QGeoPathPrivate &>(*this).computeBoundingBox();

    double lineRadius = qMax(width() * 0.5, 0.2); // minimum radius: 20cm

    if (!m_path.size())
        return false;
    else if (m_path.size() == 1)
        return (m_path[0].distanceTo(coordinate) <= lineRadius);

    QDoubleVector2D p = QWebMercator::coordToMercator(coordinate);
    if (p.x() < m_leftBoundWrapped)
        p.setX(p.x() + m_leftBoundWrapped);  // unwrap X

    QDoubleVector2D a;
    QDoubleVector2D b;
    if (m_path.size()) {
        a = QWebMercator::coordToMercator(m_path[0]);
        if (a.x() < m_leftBoundWrapped)
            a.setX(a.x() + m_leftBoundWrapped);  // unwrap X
    }
    for (int i = 1; i < m_path.size(); i++) {
        b = QWebMercator::coordToMercator(m_path[i]);
        if (b.x() < m_leftBoundWrapped)
            b.setX(b.x() + m_leftBoundWrapped);  // unwrap X
        if (b == a)
            continue;

        double u = ((p.x() - a.x()) * (b.x() - a.x()) + (p.y() - a.y()) * (b.y() - a.y()) ) / (b - a).lengthSquared();
        QDoubleVector2D intersection(a.x() + u * (b.x() - a.x()) , a.y() + u * (b.y() - a.y()) );

        QDoubleVector2D candidate = ( (p-a).length() < (p-b).length() ) ? a : b;

        if (u > 0 && u < 1
            && (p-intersection).length() < (p-candidate).length()  ) // And it falls in the segment
                candidate = intersection;


        if (candidate.x() > 1.0)
            candidate.setX(candidate.x() - m_leftBoundWrapped); // wrap X

        QGeoCoordinate closest = QWebMercator::mercatorToCoord(candidate);

        double distanceMeters = coordinate.distanceTo(closest);
        if (distanceMeters <= lineRadius)
            return true;

        // swap
        a = b;
    }

    // Last check if the coordinate is on the left of leftBoundMercator, but close enough to
    // m_path[0]
    return (m_path[0].distanceTo(coordinate) <= lineRadius);
}

bool QGeoPathPrivate::contains(const QGeoCoordinate &coordinate) const
{
    return lineContains(coordinate);
}

qreal QGeoPathPrivate::width() const
{
    return m_width;
}

void QGeoPathPrivate::setWidth(const qreal &width)
{
    if (qIsNaN(width) || width < 0.0)
        return;
    m_width = width;
}

double QGeoPathPrivate::length(int indexFrom, int indexTo) const
{
    if (path().isEmpty())
        return 0.0;

    bool wrap = indexTo == -1;
    if (indexTo < 0 || indexTo >= path().size())
        indexTo = path().size() - 1;
    double len = 0.0;
    // TODO: consider calculating the length of the actual rhumb line segments
    // instead of the shortest path from A to B.
    for (int i = indexFrom; i < indexTo; i++)
        len += m_path[i].distanceTo(m_path[i+1]);
    if (wrap)
        len += m_path.last().distanceTo(m_path.first());
    return len;
}

int QGeoPathPrivate::size() const
{
    return m_path.size();
}

QGeoCoordinate QGeoPathPrivate::coordinateAt(int index) const
{
    if (index < 0 || index >= m_path.size())
        return QGeoCoordinate();

    return m_path.at(index);
}

bool QGeoPathPrivate::containsCoordinate(const QGeoCoordinate &coordinate) const
{
    return m_path.indexOf(coordinate) > -1;
}

void QGeoPathPrivate::translate(double degreesLatitude, double degreesLongitude)
{
    // Need min/maxLati, so update bbox
    QVector<double> m_deltaXs;
    double m_minX, m_maxX, m_minLati, m_maxLati;
    m_bboxDirty = false;
    computeBBox(m_path, m_deltaXs, m_minX, m_maxX, m_minLati, m_maxLati, m_bbox);

    if (degreesLatitude > 0.0)
        degreesLatitude = qMin(degreesLatitude, 90.0 - m_maxLati);
    else
        degreesLatitude = qMax(degreesLatitude, -90.0 - m_minLati);
    for (QGeoCoordinate &p: m_path) {
        p.setLatitude(p.latitude() + degreesLatitude);
        p.setLongitude(QLocationUtils::wrapLong(p.longitude() + degreesLongitude));
    }
    m_bbox.translate(degreesLatitude, degreesLongitude);
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
}

QGeoRectangle QGeoPathPrivate::boundingGeoRectangle() const
{
    if (m_bboxDirty)
        const_cast<QGeoPathPrivate &>(*this).computeBoundingBox();
    return m_bbox;
}

void QGeoPathPrivate::setPath(const QList<QGeoCoordinate> &path)
{
    for (const QGeoCoordinate &c: path)
        if (!c.isValid())
            return;
    m_path = path;
    markDirty();
}

void QGeoPathPrivate::clearPath()
{
    m_path.clear();
    markDirty();
}

void QGeoPathPrivate::addCoordinate(const QGeoCoordinate &coordinate)
{
    if (!coordinate.isValid())
        return;
    m_path.append(coordinate);
    markDirty();
}

void QGeoPathPrivate::insertCoordinate(int index, const QGeoCoordinate &coordinate)
{
    if (index < 0 || index > m_path.size() || !coordinate.isValid())
        return;
    m_path.insert(index, coordinate);
    markDirty();
}

void QGeoPathPrivate::replaceCoordinate(int index, const QGeoCoordinate &coordinate)
{
    if (index < 0 || index >= m_path.size() || !coordinate.isValid())
        return;
    m_path[index] = coordinate;
    markDirty();
}

void QGeoPathPrivate::removeCoordinate(const QGeoCoordinate &coordinate)
{
    int index = m_path.lastIndexOf(coordinate);
    removeCoordinate(index);
}

void QGeoPathPrivate::removeCoordinate(int index)
{
    if (index < 0 || index >= m_path.size())
        return;
    m_path.removeAt(index);
    markDirty();
}

void QGeoPathPrivate::markDirty()
{
    m_bboxDirty = true;
}

void QGeoPathPrivate::computeBoundingBox()
{
    QVector<double> m_deltaXs;
    double m_minX, m_maxX, m_minLati, m_maxLati;
    m_bboxDirty = false;
    computeBBox(m_path, m_deltaXs, m_minX, m_maxX, m_minLati, m_maxLati, m_bbox);
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
}

QGeoPathPrivateEager::QGeoPathPrivateEager()
:   QGeoPathPrivate()
{
    m_bboxDirty = false; // never dirty on the eager version
}

QGeoPathPrivateEager::QGeoPathPrivateEager(const QList<QGeoCoordinate> &path, const qreal width)
:   QGeoPathPrivate(path, width)
{
    m_bboxDirty = false; // never dirty on the eager version
}

QGeoPathPrivateEager::~QGeoPathPrivateEager()
{

}

QGeoShapePrivate *QGeoPathPrivateEager::clone() const
{
    return new QGeoPathPrivateEager(*this);
}

void QGeoPathPrivateEager::markDirty()
{
    computeBoundingBox();
}

void QGeoPathPrivateEager::translate(double degreesLatitude, double degreesLongitude)
{
    if (degreesLatitude > 0.0)
        degreesLatitude = qMin(degreesLatitude, 90.0 - m_maxLati);
    else
        degreesLatitude = qMax(degreesLatitude, -90.0 - m_minLati);
    for (QGeoCoordinate &p: m_path) {
        p.setLatitude(p.latitude() + degreesLatitude);
        p.setLongitude(QLocationUtils::wrapLong(p.longitude() + degreesLongitude));
    }
    m_bbox.translate(degreesLatitude, degreesLongitude);
    m_minLati += degreesLatitude;
    m_maxLati += degreesLatitude;
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
}

void QGeoPathPrivateEager::addCoordinate(const QGeoCoordinate &coordinate)
{
    if (!coordinate.isValid())
        return;
    m_path.append(coordinate);
    //m_clipperDirty = true; // clipper not used in polylines
    updateBoundingBox();
}

void QGeoPathPrivateEager::QGeoPathPrivateEager::computeBoundingBox()
{
    computeBBox(m_path, m_deltaXs, m_minX, m_maxX, m_minLati, m_maxLati, m_bbox);
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
}

void QGeoPathPrivateEager::QGeoPathPrivateEager::updateBoundingBox()
{
    updateBBox(m_path, m_deltaXs, m_minX, m_maxX, m_minLati, m_maxLati, m_bbox);
    m_leftBoundWrapped = QWebMercator::coordToMercator(m_bbox.topLeft()).x();
}

QGeoPathEager::QGeoPathEager() : QGeoPath()
{
    initPathConversions();
    d_ptr = new QGeoPathPrivateEager;
}

QGeoPathEager::QGeoPathEager(const QList<QGeoCoordinate> &path, const qreal &width) : QGeoPath()
{
    initPathConversions();
    d_ptr = new QGeoPathPrivateEager(path, width);
}

QGeoPathEager::QGeoPathEager(const QGeoPath &other) : QGeoPath()
{
    initPathConversions();
    d_ptr = new QGeoPathPrivateEager;
    setPath(other.path());
    setWidth(other.width());
}

QGeoPathEager::QGeoPathEager(const QGeoShape &other) : QGeoPath()
{
    initPathConversions();
    if (other.type() == QGeoShape::PathType)
        *this = QGeoPathEager(QGeoPath(other));
    else
        d_ptr = new QGeoPathPrivateEager;
}

QGeoPathEager::~QGeoPathEager() {}

QT_END_NAMESPACE








