/****************************************************************************
 **
 ** Copyright (C) 2015 The Qt Company Ltd.
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
#include "qdeclarativepolygonmapitem_p.h"
#include "qdeclarativepolylinemapitem_p_p.h"
#include "qdeclarativepolygonmapitem_p_p.h"
#include "qdeclarativerectanglemapitem_p_p.h"
#include "qlocationutils_p.h"
#include "error_messages_p.h"
#include "locationvaluetypehelper_p.h"
#include <QtLocation/private/qgeomap_p.h>

#include <QtCore/QScopedValueRollback>
#include <QtGui/private/qtriangulator_p.h>
#include <QtQml/QQmlInfo>
#include <QtQml/private/qqmlengine_p.h>
#include <QPainter>
#include <QPainterPath>
#include <qnumeric.h>

#include <QtPositioning/private/qdoublevector2d_p.h>
#include <QtPositioning/private/qclipperutils_p.h>
#include <QtPositioning/private/qgeopolygon_p.h>
#include <QtPositioning/private/qwebmercator_p.h>
#include <QtQuick/private/qsgmaterialshader_p.h>
#include <QtQuick/private/qquickitem_p.h>
#include <QtQuick/qsgnode.h>

/* poly2tri triangulator includes */
#include <clip2tri.h>
#include <earcut.hpp>
#include <array>

QT_BEGIN_NAMESPACE

/*!
    \qmltype MapPolygon
    \instantiates QDeclarativePolygonMapItem
    \inqmlmodule QtLocation
    \ingroup qml-QtLocation5-maps
    \since QtLocation 5.5

    \brief The MapPolygon type displays a polygon on a Map.

    The MapPolygon type displays a polygon on a Map, specified in terms of an ordered list of
    \l {QtPositioning::coordinate}{coordinates}. For best appearance and results, polygons should be
    simple (not self-intersecting).

    The \l {QtPositioning::coordinate}{coordinates} on the path cannot be directly changed after
    being added to the Polygon.  Instead, copy the \l path into a var, modify the copy and reassign
    the copy back to the \l path.

    \code
    var path = mapPolygon.path;
    path[0].latitude = 5;
    mapPolygon.path = path;
    \endcode

    Coordinates can also be added and removed at any time using the \l addCoordinate and
    \l removeCoordinate methods.

    For drawing rectangles with "straight" edges (same latitude across one
    edge, same latitude across the other), the \l MapRectangle type provides
    a simpler, two-point API.

    By default, the polygon is displayed as a 1 pixel black border with no
    fill. To change its appearance, use the \l color, \l border.color and
    \l border.width properties.

    \note Since MapPolygons are geographic items, dragging a MapPolygon
    (through the use of \l MouseArea) causes its vertices to be
    recalculated in the geographic coordinate space. The edges retain the
    same geographic lengths (latitude and longitude differences between the
    vertices), but they remain straight. Apparent stretching of the item occurs
    when dragged to a different latitude.

    \section2 Performance

    MapPolygons have a rendering cost that is O(n) with respect to the number
    of vertices. This means that the per frame cost of having a Polygon on the
    Map grows in direct proportion to the number of points on the Polygon. There
    is an additional triangulation cost (approximately O(n log n)) which is
    currently paid with each frame, but in future may be paid only upon adding
    or removing points.

    Like the other map objects, MapPolygon is normally drawn without a smooth
    appearance. Setting the \l {Item::opacity}{opacity} property will force the object to
    be blended, which decreases performance considerably depending on the hardware in use.

    \section2 Example Usage

    The following snippet shows a MapPolygon being used to display a triangle,
    with three vertices near Brisbane, Australia. The triangle is filled in
    green, with a 1 pixel black border.

    \code
    Map {
        MapPolygon {
            color: 'green'
            path: [
                { latitude: -27, longitude: 153.0 },
                { latitude: -27, longitude: 154.1 },
                { latitude: -28, longitude: 153.5 }
            ]
        }
    }
    \endcode

    \image api-mappolygon.png
*/

/*!
    \qmlproperty bool QtLocation::MapPolygon::autoFadeIn

    This property holds whether the item automatically fades in when zooming into the map
    starting from very low zoom levels. By default this is \c true.
    Setting this property to \c false causes the map item to always have the opacity specified
    with the \l QtQuick::Item::opacity property, which is 1.0 by default.

    \since 5.14
*/

QGeoMapPolygonGeometry::QGeoMapPolygonGeometry()
:   assumeSimple_(false)
{
}

/*!
    \internal
*/
void QGeoMapPolygonGeometry::updateSourcePoints(const QGeoMap &map,
                                                const QList<QDoubleVector2D> &path)
{
    if (!sourceDirty_)
        return;
    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(map.geoProjection());
    srcPath_ = QPainterPath();

    // build the actual path
    // The approach is the same as described in QGeoMapPolylineGeometry::updateSourcePoints
    srcOrigin_ = geoLeftBound_;
    double unwrapBelowX = 0;
    QDoubleVector2D leftBoundWrapped = p.wrapMapProjection(p.geoToMapProjection(geoLeftBound_));
    if (preserveGeometry_)
        unwrapBelowX = leftBoundWrapped.x();

    QList<QDoubleVector2D> wrappedPath;
    wrappedPath.reserve(path.size());
    QDoubleVector2D wrappedLeftBound(qInf(), qInf());
    // 1)
    for (int i = 0; i < path.size(); ++i) {
        const QDoubleVector2D &coord = path.at(i);
        QDoubleVector2D wrappedProjection = p.wrapMapProjection(coord);

        // We can get NaN if the map isn't set up correctly, or the projection
        // is faulty -- probably best thing to do is abort
        if (!qIsFinite(wrappedProjection.x()) || !qIsFinite(wrappedProjection.y()))
            return;

        const bool isPointLessThanUnwrapBelowX = (wrappedProjection.x() < leftBoundWrapped.x());
        // unwrap x to preserve geometry if moved to border of map
        if (preserveGeometry_ && isPointLessThanUnwrapBelowX) {
            double distance = wrappedProjection.x() - unwrapBelowX;
            if (distance < 0.0)
                distance += 1.0;
            wrappedProjection.setX(unwrapBelowX + distance);
        }
        if (wrappedProjection.x() < wrappedLeftBound.x() || (wrappedProjection.x() == wrappedLeftBound.x() && wrappedProjection.y() < wrappedLeftBound.y())) {
            wrappedLeftBound = wrappedProjection;
        }
        wrappedPath.append(wrappedProjection);
    }

    // 2)
    QList<QList<QDoubleVector2D> > clippedPaths;
    const QList<QDoubleVector2D> &visibleRegion = p.projectableGeometry();
    if (visibleRegion.size()) {
        c2t::clip2tri clipper;
        clipper.addSubjectPath(QClipperUtils::qListToPath(wrappedPath), true);
        clipper.addClipPolygon(QClipperUtils::qListToPath(visibleRegion));
        Paths res = clipper.execute(c2t::clip2tri::Intersection, QtClipperLib::pftEvenOdd, QtClipperLib::pftEvenOdd);
        clippedPaths = QClipperUtils::pathsToQList(res);

        // 2.1) update srcOrigin_ and leftBoundWrapped with the point with minimum X
        QDoubleVector2D lb(qInf(), qInf());
        for (const QList<QDoubleVector2D> &path: clippedPaths)
            for (const QDoubleVector2D &p: path)
                if (p.x() < lb.x() || (p.x() == lb.x() && p.y() < lb.y()))
                    // y-minimization needed to find the same point on polygon and border
                    lb = p;

        if (qIsInf(lb.x())) // e.g., when the polygon is clipped entirely
            return;

        // 2.2) Prevent the conversion to and from clipper from introducing negative offsets which
        //      in turn will make the geometry wrap around.
        lb.setX(qMax(wrappedLeftBound.x(), lb.x()));
        leftBoundWrapped = lb;
        srcOrigin_ = p.mapProjectionToGeo(p.unwrapMapProjection(lb));
    } else {
        clippedPaths.append(wrappedPath);
    }

    // 3)
    QDoubleVector2D origin = p.wrappedMapProjectionToItemPosition(leftBoundWrapped);
    for (const QList<QDoubleVector2D> &path: clippedPaths) {
        QDoubleVector2D lastAddedPoint;
        for (int i = 0; i < path.size(); ++i) {
            QDoubleVector2D point = p.wrappedMapProjectionToItemPosition(path.at(i));
            point = point - origin; // (0,0) if point == geoLeftBound_

            if (i == 0) {
                srcPath_.moveTo(point.toPointF());
                lastAddedPoint = point;
            } else {
                if ((point - lastAddedPoint).manhattanLength() > 3 ||
                        i == path.size() - 1) {
                    srcPath_.lineTo(point.toPointF());
                    lastAddedPoint = point;
                }
            }
        }
        srcPath_.closeSubpath();
    }

    if (!assumeSimple_)
        srcPath_ = srcPath_.simplified();

    sourceBounds_ = srcPath_.boundingRect();
}

/*!
    \internal
*/
void QGeoMapPolygonGeometry::updateScreenPoints(const QGeoMap &map, qreal strokeWidth)
{
    if (!screenDirty_)
        return;

    if (map.viewportWidth() == 0 || map.viewportHeight() == 0) {
        clear();
        return;
    }

    // The geometry has already been clipped against the visible region projection in wrapped mercator space.
    QPainterPath ppi = srcPath_;
    clear();

    // a polygon requires at least 3 points;
    if (ppi.elementCount() < 3)
        return;

    // translate the path into top-left-centric coordinates
    QRectF bb = ppi.boundingRect();
    ppi.translate(-bb.left(), -bb.top());
    firstPointOffset_ = -1 * bb.topLeft();

    ppi.closeSubpath();
    screenOutline_ = ppi;

    using Coord = double;
    using N = uint32_t;
    using Point = std::array<Coord, 2>;

    std::vector<std::vector<Point>> polygon;
    polygon.push_back(std::vector<Point>());
    std::vector<Point> &poly = polygon.front();
    // ... fill polygon structure with actual data

    for (int i = 0; i < ppi.elementCount(); ++i) {
        const QPainterPath::Element e = ppi.elementAt(i);
        if (e.isMoveTo() || i == ppi.elementCount() - 1
                || (qAbs(e.x - poly.front()[0]) < 0.1
                    && qAbs(e.y - poly.front()[1]) < 0.1)) {
            Point p = {{ e.x, e.y }};
            poly.push_back( p );
        } else if (e.isLineTo()) {
            Point p = {{ e.x, e.y }};
            poly.push_back( p );
        } else {
            qWarning("Unhandled element type in polygon painterpath");
        }
    }

    if (poly.size() > 2) {
        // Run tessellation
        // Returns array of indices that refer to the vertices of the input polygon.
        // Three subsequent indices form a triangle.
        screenVertices_.clear();
        screenIndices_.clear();
        for (const auto &p : poly)
            screenVertices_ << QPointF(p[0], p[1]);
        std::vector<N> indices = qt_mapbox::earcut<N>(polygon);
        for (const auto &i: indices)
            screenIndices_ << quint32(i);
    }

    screenBounds_ = ppi.boundingRect();
    if (strokeWidth != 0.0)
        this->translate(QPointF(strokeWidth, strokeWidth));
}

QGeoMapPolygonGeometryOpenGL::QGeoMapPolygonGeometryOpenGL(){
}

void QGeoMapPolygonGeometryOpenGL::updateSourcePoints(const QGeoMap &map, const QList<QDoubleVector2D> &path)
{
    QList<QGeoCoordinate> geopath;
    for (const auto &c: path)
        geopath.append(QWebMercator::mercatorToCoord(c));
    updateSourcePoints(map, geopath);
}

// wrapPath always preserves the geometry
// This one handles holes
static void wrapPath(const QGeoPolygon &poly
                     ,const QGeoCoordinate &geoLeftBound
                     ,const QGeoProjectionWebMercator &p
                     ,QList<QList<QDoubleVector2D> > &wrappedPaths
                     ,QDoubleVector2D *leftBoundWrapped = nullptr)
{
    QList<QList<QDoubleVector2D> > paths;
    for (int i = 0; i < 1+poly.holesCount(); ++i) {
        QList<QDoubleVector2D> path;
        if (!i) {
            for (const QGeoCoordinate &c : poly.path())
                path << p.geoToMapProjection(c);
        } else {
            for (const QGeoCoordinate &c : poly.holePath(i-1))
                path << p.geoToMapProjection(c);
        }
        paths.append(path);
    }

    const QDoubleVector2D leftBound = p.geoToMapProjection(geoLeftBound);
    wrappedPaths.clear();

    QList<QDoubleVector2D> wrappedPath;
    // compute 3 sets of "wrapped" coordinates: one w regular mercator, one w regular mercator +- 1.0
    for (int j = 0; j < paths.size(); ++j) {
        const QList<QDoubleVector2D> &path = paths.at(j);
        wrappedPath.clear();
        for (int i = 0; i < path.size(); ++i) {
            QDoubleVector2D coord = path.at(i);

            // We can get NaN if the map isn't set up correctly, or the projection
            // is faulty -- probably best thing to do is abort
            if (!qIsFinite(coord.x()) || !qIsFinite(coord.y())) {
                wrappedPaths.clear();
                return;
            }

            const bool isPointLessThanUnwrapBelowX = (coord.x() < leftBound.x());
            // unwrap x to preserve geometry if moved to border of map
            if (isPointLessThanUnwrapBelowX)
                coord.setX(coord.x() + 1.0);
            wrappedPath.append(coord);
        }
        wrappedPaths.append(wrappedPath);
    }

    if (leftBoundWrapped)
        *leftBoundWrapped = leftBound;
}

static void cutPathEars(const QList<QList<QDoubleVector2D>> &wrappedPaths,
                        QVector<QDeclarativeGeoMapItemUtils::vec2> &screenVertices,
                        QVector<quint32> &screenIndices)
{
    using Coord = double;
    using N = uint32_t;
    using Point = std::array<Coord, 2>;
    screenVertices.clear();
    screenIndices.clear();

    std::vector<std::vector<Point>> polygon;
    std::vector<Point> poly;

    for (const QList<QDoubleVector2D> &wrappedPath: wrappedPaths) {
        poly.clear();
        for (const QDoubleVector2D &v: wrappedPath) {
            screenVertices << v;
            Point pt = {{ v.x(), v.y() }};
            poly.push_back( pt );
        }
        polygon.push_back(poly);
    }

    std::vector<N> indices = qt_mapbox::earcut<N>(polygon);

    for (const auto &i: indices)
        screenIndices << quint32(i);
}

static void cutPathEars(const QList<QDoubleVector2D> &wrappedPath,
                        QVector<QDeclarativeGeoMapItemUtils::vec2> &screenVertices,
                        QVector<quint32> &screenIndices)
{
    using Coord = double;
    using N = uint32_t;
    using Point = std::array<Coord, 2>;
    screenVertices.clear();
    screenIndices.clear();

    std::vector<std::vector<Point>> polygon;
    std::vector<Point> poly;

    for (const QDoubleVector2D &v: wrappedPath) {
        screenVertices << v;
        Point pt = {{ v.x(), v.y() }};
        poly.push_back( pt );
    }
    polygon.push_back(poly);

    std::vector<N> indices = qt_mapbox::earcut<N>(polygon);

    for (const auto &i: indices)
        screenIndices << quint32(i);
}

/*!
    \internal
*/
// This one does only a perimeter
void QGeoMapPolygonGeometryOpenGL::updateSourcePoints(const QGeoMap &map,
                                                const QList<QGeoCoordinate> &perimeter)
{
    if (!sourceDirty_)
        return;
    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(map.geoProjection());

    // build the actual path
    // The approach is the same as described in QGeoMapPolylineGeometry::updateSourcePoints
    srcOrigin_ = geoLeftBound_;

    QDoubleVector2D leftBoundWrapped;
    // 1) pre-compute 3 sets of "wrapped" coordinates: one w regular mercator, one w regular mercator +- 1.0
    QList<QDoubleVector2D> wrappedPath;
    QDeclarativeGeoMapItemUtils::wrapPath(perimeter, geoLeftBound_, p,
             wrappedPath, &leftBoundWrapped);

    // 1.1) do the same for the bbox
    QList<QDoubleVector2D> wrappedBbox, wrappedBboxPlus1, wrappedBboxMinus1;
    QGeoPolygon bbox(QGeoPath(perimeter).boundingGeoRectangle());
    QDeclarativeGeoMapItemUtils::wrapPath(bbox.path(), bbox.boundingGeoRectangle().topLeft(), p,
             wrappedBbox, wrappedBboxMinus1, wrappedBboxPlus1, &m_bboxLeftBoundWrapped);

    // 2) Store the triangulated polygon, and the wrapped bbox paths.
    //    the triangulations can be used as they are, as they "bypass" the QtQuick display chain
    //    the bbox wraps have to be however clipped, and then projected, in order to figure out the geometry.
    //    Note that this might still cause the geometryChanged method to fail under some extreme conditions.
    cutPathEars(wrappedPath, m_screenVertices, m_screenIndices);

    m_wrappedPolygons.resize(3);
    m_wrappedPolygons[0].wrappedBboxes = wrappedBboxMinus1;
    m_wrappedPolygons[1].wrappedBboxes = wrappedBbox;
    m_wrappedPolygons[2].wrappedBboxes = wrappedBboxPlus1;
}

// This one handles whole QGeoPolygon w. holes
void QGeoMapPolygonGeometryOpenGL::updateSourcePoints(const QGeoMap &map, const QGeoPolygon &poly)
{
    if (!sourceDirty_)
        return;
    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(map.geoProjection());

    // build the actual path
    // The approach is the same as described in QGeoMapPolylineGeometry::updateSourcePoints
    srcOrigin_ = geoLeftBound_;

    QDoubleVector2D leftBoundWrapped;
    QList<QList<QDoubleVector2D>> wrappedPath;
    // 1) pre-compute 3 sets of "wrapped" coordinates: one w regular mercator, one w regular mercator +- 1.0
    wrapPath(poly, geoLeftBound_, p,
             wrappedPath, &leftBoundWrapped);

    // 1.1) do the same for the bbox
    QList<QDoubleVector2D> wrappedBbox, wrappedBboxPlus1, wrappedBboxMinus1;
    QGeoPolygon bbox(poly.boundingGeoRectangle());
    QDeclarativeGeoMapItemUtils::wrapPath(bbox.path(), bbox.boundingGeoRectangle().topLeft(), p,
             wrappedBbox, wrappedBboxMinus1, wrappedBboxPlus1, &m_bboxLeftBoundWrapped);

    // 2) Store the triangulated polygon, and the wrapped bbox paths.
    //    the triangulations can be used as they are, as they "bypass" the QtQuick display chain
    //    the bbox wraps have to be however clipped, and then projected, in order to figure out the geometry.
    //    Note that this might still cause the geometryChanged method to fail under some extreme conditions.
    cutPathEars(wrappedPath, m_screenVertices, m_screenIndices);
    m_wrappedPolygons.resize(3);
    m_wrappedPolygons[0].wrappedBboxes = wrappedBboxMinus1;
    m_wrappedPolygons[1].wrappedBboxes = wrappedBbox;
    m_wrappedPolygons[2].wrappedBboxes = wrappedBboxPlus1;
}

void QGeoMapPolygonGeometryOpenGL::updateSourcePoints(const QGeoMap &map, const QGeoRectangle &rect)
{
    if (!sourceDirty_)
        return;
    const QList<QGeoCoordinate> perimeter = QDeclarativeRectangleMapItemPrivateCPU::path(rect);
    updateSourcePoints(map, perimeter);
}

/*!
    \internal
*/
void QGeoMapPolygonGeometryOpenGL::updateScreenPoints(const QGeoMap &map, qreal strokeWidth , const QColor &strokeColor)
{
    if (map.viewportWidth() == 0 || map.viewportHeight() == 0) {
        clear();
        return;
    }

    // 1) identify which set to use: std, +1 or -1
    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(map.geoProjection());
    const QDoubleVector2D leftBoundMercator = p.geoToMapProjection(srcOrigin_);
    m_wrapOffset = p.projectionWrapFactor(leftBoundMercator) + 1; // +1 to get the offset into QLists

    // 1.1) select geometry set
    // This could theoretically be skipped for those polygons whose bbox is not even projectable.
    // However, such optimization could only be introduced if not calculating bboxes lazily.
    // Hence not doing it.
    if (sourceDirty_) {
        m_dataChanged = true;
    }

    if (strokeWidth == 0.0 || strokeColor.alpha() == 0) // or else the geometry of the border is used, so no point in calculating 2 of them
        updateQuickGeometry(p, strokeWidth);
}

void QGeoMapPolygonGeometryOpenGL::updateQuickGeometry(const QGeoProjectionWebMercator &p, qreal /*strokeWidth*/)
{
     // 2) clip bbox
    // BBox handling --  this is related to the bounding box geometry
    // that has to inevitably follow the old projection codepath
    // As it needs to provide projected coordinates for QtQuick interaction.
    // This could be futher optimized to be updated in a lazy fashion.
    const QList<QDoubleVector2D> &wrappedBbox = m_wrappedPolygons.at(m_wrapOffset).wrappedBboxes;
    QList<QList<QDoubleVector2D> > clippedBbox;
    QDoubleVector2D bboxLeftBoundWrapped = m_bboxLeftBoundWrapped;
    bboxLeftBoundWrapped.setX(bboxLeftBoundWrapped.x() + double(m_wrapOffset - 1));
    QDeclarativeGeoMapItemUtils::clipPolygon(wrappedBbox, p, clippedBbox, &bboxLeftBoundWrapped);

    // 3) project bbox
    QPainterPath ppi;
    if (!clippedBbox.size() || clippedBbox.first().size() < 3) {
        sourceBounds_ = screenBounds_ = QRectF();
        firstPointOffset_ = QPointF();
        screenOutline_ = ppi;
        return;
    }

    QDeclarativeGeoMapItemUtils::projectBbox(clippedBbox.first(), p, ppi); // Using first because a clipped box should always result in one polygon
    const QRectF brect = ppi.boundingRect();
    firstPointOffset_ = QPointF(brect.topLeft());
    screenOutline_ = ppi;

    // 4) Set Screen bbox
    screenBounds_ = brect;
    sourceBounds_.setX(0);
    sourceBounds_.setY(0);
    sourceBounds_.setWidth(brect.width());
    sourceBounds_.setHeight(brect.height());
}

/*
 * QDeclarativePolygonMapItem Private Implementations
 */

QDeclarativePolygonMapItemPrivate::~QDeclarativePolygonMapItemPrivate() {}

QDeclarativePolygonMapItemPrivateCPU::~QDeclarativePolygonMapItemPrivateCPU() {}

QDeclarativePolygonMapItemPrivateOpenGL::~QDeclarativePolygonMapItemPrivateOpenGL() {}

/*
 * QDeclarativePolygonMapItem Implementation
 */

struct PolygonBackendSelector
{
    PolygonBackendSelector()
    {
        backend = (qgetenv("QTLOCATION_OPENGL_ITEMS").toInt()) ? QDeclarativePolygonMapItem::OpenGL : QDeclarativePolygonMapItem::Software;
    }
    QDeclarativePolygonMapItem::Backend backend = QDeclarativePolygonMapItem::Software;
};

Q_GLOBAL_STATIC(PolygonBackendSelector, mapPolygonBackendSelector)

QDeclarativePolygonMapItem::QDeclarativePolygonMapItem(QQuickItem *parent)
:   QDeclarativeGeoMapItemBase(parent), m_border(this), m_color(Qt::transparent), m_dirtyMaterial(true),
    m_updatingGeometry(false)
  , m_d(new QDeclarativePolygonMapItemPrivateCPU(*this))

{
    // ToDo: handle envvar, and switch implementation.
    m_itemType = QGeoMap::MapPolygon;
    m_geopoly = QGeoPolygonEager();
    setFlag(ItemHasContents, true);
    QObject::connect(&m_border, SIGNAL(colorChanged(QColor)),
                     this, SLOT(onLinePropertiesChanged())); // ToDo: fix this, only flag material?
    QObject::connect(&m_border, SIGNAL(widthChanged(qreal)),
                     this, SLOT(onLinePropertiesChanged()));
    setBackend(mapPolygonBackendSelector->backend);
}

QDeclarativePolygonMapItem::~QDeclarativePolygonMapItem()
{
}

/*!
    \qmlpropertygroup Location::MapPolygon::border
    \qmlproperty int MapPolygon::border.width
    \qmlproperty color MapPolygon::border.color

    This property is part of the border property group. The border property
    group holds the width and color used to draw the border of the polygon.

    The width is in pixels and is independent of the zoom level of the map.

    The default values correspond to a black border with a width of 1 pixel.
    For no line, use a width of 0 or a transparent color.
*/

QDeclarativeMapLineProperties *QDeclarativePolygonMapItem::border()
{
    return &m_border;
}

/*!
    \qmlproperty MapPolygon.Backend QtLocation::MapPolygon::backend

    This property holds which backend is in use to render the map item.
    Valid values are \b MapPolygon.Software and \b{MapPolygon.OpenGL}.
    The default value is \b{MapPolygon.Software}.

    \note \b{The release of this API with Qt 5.15 is a Technology Preview}.
    Ideally, as the OpenGL backends for map items mature, there will be
    no more need to also offer the legacy software-projection backend.
    So this property will likely disappear at some later point.
    To select OpenGL-accelerated item backends without using this property,
    it is also possible to set the environment variable \b QTLOCATION_OPENGL_ITEMS
    to \b{1}.
    Also note that all current OpenGL backends won't work as expected when enabling
    layers on the individual item, or when running on OpenGL core profiles greater than 2.x.

    \since 5.15
*/
QDeclarativePolygonMapItem::Backend QDeclarativePolygonMapItem::backend() const
{
    return m_backend;
}

void QDeclarativePolygonMapItem::setBackend(QDeclarativePolygonMapItem::Backend b)
{
    if (b == m_backend)
        return;
    m_backend = b;
    QScopedPointer<QDeclarativePolygonMapItemPrivate> d((m_backend == Software)
                                                        ? static_cast<QDeclarativePolygonMapItemPrivate *>(new QDeclarativePolygonMapItemPrivateCPU(*this))
                                                        : static_cast<QDeclarativePolygonMapItemPrivate * >(new QDeclarativePolygonMapItemPrivateOpenGL(*this)));
    m_d.swap(d);
    m_d->onGeoGeometryChanged();
    emit backendChanged();
}

/*!
    \internal
*/
void QDeclarativePolygonMapItem::setMap(QDeclarativeGeoMap *quickMap, QGeoMap *map)
{
    QDeclarativeGeoMapItemBase::setMap(quickMap,map);
    if (map)
        m_d->onMapSet();
}

/*!
    \qmlproperty list<coordinate> MapPolygon::path

    This property holds the ordered list of coordinates which
    define the polygon.
    Having less than 3 different coordinates in the path results in undefined behavior.

    \sa addCoordinate, removeCoordinate
*/
QJSValue QDeclarativePolygonMapItem::path() const
{
    return fromList(this, m_geopoly.path());
}

void QDeclarativePolygonMapItem::setPath(const QJSValue &value)
{
    if (!value.isArray())
        return;

    QList<QGeoCoordinate> pathList = toList(this, value);

    // Equivalent to QDeclarativePolylineMapItem::setPathFromGeoList
    if (m_geopoly.path() == pathList)
        return;

    m_geopoly.setPath(pathList);

    m_d->onGeoGeometryChanged();
    emit pathChanged();
}

/*!
    \qmlmethod void MapPolygon::addCoordinate(coordinate)

    Adds the specified \a coordinate to the path.

    \sa removeCoordinate, path
*/

void QDeclarativePolygonMapItem::addCoordinate(const QGeoCoordinate &coordinate)
{
    if (!coordinate.isValid())
        return;

    m_geopoly.addCoordinate(coordinate);
    m_d->onGeoGeometryUpdated();
    emit pathChanged();
}

/*!
    \qmlmethod void MapPolygon::removeCoordinate(coordinate)

    Removes \a coordinate from the path. If there are multiple instances of the
    same coordinate, the one added last is removed.

    If \a coordinate is not in the path this method does nothing.

    \sa addCoordinate, path
*/
void QDeclarativePolygonMapItem::removeCoordinate(const QGeoCoordinate &coordinate)
{
    int length = m_geopoly.path().length();
    m_geopoly.removeCoordinate(coordinate);
    if (m_geopoly.path().length() == length)
        return;

    m_d->onGeoGeometryChanged();
    emit pathChanged();
}

/*!
    \qmlproperty color MapPolygon::color

    This property holds the color used to fill the polygon.

    The default value is transparent.
*/

QColor QDeclarativePolygonMapItem::color() const
{
    return m_color;
}

void QDeclarativePolygonMapItem::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    m_dirtyMaterial = true;
    polishAndUpdate(); // in case color was transparent and now is not or vice versa
    emit colorChanged(m_color);
}

/*!
    \internal
*/
QSGNode *QDeclarativePolygonMapItem::updateMapItemPaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    return m_d->updateMapItemPaintNode(oldNode, data);
}

/*!
    \internal
*/
void QDeclarativePolygonMapItem::updatePolish()
{
    if (!map() || map()->geoProjection().projectionType() != QGeoProjection::ProjectionWebMercator)
        return;
    m_d->updatePolish();
}

void QDeclarativePolygonMapItem::setMaterialDirty()
{
    m_dirtyMaterial = true;
    update();
}

void QDeclarativePolygonMapItem::markSourceDirtyAndUpdate()
{
    m_d->markSourceDirtyAndUpdate();
}

void QDeclarativePolygonMapItem::onLinePropertiesChanged()
{
    m_d->onLinePropertiesChanged();
}

/*!
    \internal
*/
void QDeclarativePolygonMapItem::afterViewportChanged(const QGeoMapViewportChangeEvent &event)
{
    if (event.mapSize.isEmpty())
        return;

    m_d->afterViewportChanged();
}

/*!
    \internal
*/
bool QDeclarativePolygonMapItem::contains(const QPointF &point) const
{
    return m_d->contains(point);
}

const QGeoShape &QDeclarativePolygonMapItem::geoShape() const
{
    return m_geopoly;
}

void QDeclarativePolygonMapItem::setGeoShape(const QGeoShape &shape)
{
    if (shape == m_geopoly)
        return;

    m_geopoly = QGeoPolygonEager(shape);
    m_d->onGeoGeometryChanged();
    emit pathChanged();
}

/*!
    \internal
*/
void QDeclarativePolygonMapItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    if (newGeometry.topLeft() == oldGeometry.topLeft() || !map() || !m_geopoly.isValid() || m_updatingGeometry) {
        QDeclarativeGeoMapItemBase::geometryChanged(newGeometry, oldGeometry);
        return;
    }
    // TODO: change the algorithm to preserve the distances and size!
    QGeoCoordinate newCenter = map()->geoProjection().itemPositionToCoordinate(QDoubleVector2D(newGeometry.center()), false);
    QGeoCoordinate oldCenter = map()->geoProjection().itemPositionToCoordinate(QDoubleVector2D(oldGeometry.center()), false);
    if (!newCenter.isValid() || !oldCenter.isValid())
        return;
    double offsetLongi = newCenter.longitude() - oldCenter.longitude();
    double offsetLati = newCenter.latitude() - oldCenter.latitude();
    if (offsetLati == 0.0 && offsetLongi == 0.0)
        return;

    m_geopoly.translate(offsetLati, offsetLongi);
    m_d->onGeoGeometryChanged();
    emit pathChanged();

    // Not calling QDeclarativeGeoMapItemBase::geometryChanged() as it will be called from a nested
    // call to this function.
}

//////////////////////////////////////////////////////////////////////

QSGMaterialShader *MapPolygonMaterial::createShader() const
{
    return new MapPolygonShader();
}

int MapPolygonMaterial::compare(const QSGMaterial *other) const
{
    const MapPolygonMaterial &o = *static_cast<const MapPolygonMaterial *>(other);
    if (o.m_center == m_center && o.m_geoProjection == m_geoProjection && o.m_wrapOffset == m_wrapOffset)
        return QSGFlatColorMaterial::compare(other);
    return -1;
}

QSGMaterialType *MapPolygonMaterial::type() const
{
    static QSGMaterialType type;
    return &type;
}

MapPolygonNode::MapPolygonNode() :
    border_(new MapPolylineNode()),
    geometry_(QSGGeometry::defaultAttributes_Point2D(), 0)
{
    geometry_.setDrawingMode(QSGGeometry::DrawTriangles);
    QSGGeometryNode::setMaterial(&fill_material_);
    QSGGeometryNode::setGeometry(&geometry_);

    appendChildNode(border_);
}

MapPolygonNode::~MapPolygonNode()
{
}

/*!
    \internal
*/
void MapPolygonNode::update(const QColor &fillColor, const QColor &borderColor,
                            const QGeoMapItemGeometry *fillShape,
                            const QGeoMapItemGeometry *borderShape)
{
    /* Do the border update first */
    border_->update(borderColor, borderShape);

    /* If we have neither fill nor border with valid points, block the whole
     * tree. We can't just block the fill without blocking the border too, so
     * we're a little conservative here (maybe at the expense of rendering
     * accuracy) */
    if (fillShape->size() == 0 && borderShape->size() == 0) {
            setSubtreeBlocked(true);
            return;
    }
    setSubtreeBlocked(false);


    // TODO: do this only if the geometry has changed!!
    // No need to do this every frame.
    // Then benchmark the difference!
    QSGGeometry *fill = QSGGeometryNode::geometry();
    fillShape->allocateAndFill(fill);
    markDirty(DirtyGeometry);

    if (fillColor != fill_material_.color()) {
        fill_material_.setColor(fillColor);
        setMaterial(&fill_material_);
        markDirty(DirtyMaterial);
    }
}

MapPolygonNodeGL::MapPolygonNodeGL() :
    //fill_material_(this),
    fill_material_(),
    geometry_(QSGGeometry::defaultAttributes_Point2D(), 0)
{
    geometry_.setDrawingMode(QSGGeometry::DrawTriangles);
    QSGGeometryNode::setMaterial(&fill_material_);
    QSGGeometryNode::setGeometry(&geometry_);
}

MapPolygonNodeGL::~MapPolygonNodeGL()
{
}

/*!
    \internal
*/
void MapPolygonNodeGL::update(const QColor &fillColor,
                            const QGeoMapPolygonGeometryOpenGL *fillShape,
                            const QMatrix4x4 &geoProjection,
                            const QDoubleVector3D &center)
{
    if (fillShape->m_screenIndices.size() < 3 || fillColor.alpha() == 0) {
        setSubtreeBlocked(true);
        return;
    }
    setSubtreeBlocked(false);

    QSGGeometry *fill = QSGGeometryNode::geometry();
    if (fillShape->m_dataChanged || !fill->vertexCount()) {
        fillShape->allocateAndFillPolygon(fill);
        markDirty(DirtyGeometry);
        fillShape->m_dataChanged = false;
    }

    //if (fillColor != fill_material_.color()) // Any point in optimizing this?
    {
        fill_material_.setColor(fillColor);
        fill_material_.setGeoProjection(geoProjection);
        fill_material_.setCenter(center);
        fill_material_.setWrapOffset(fillShape->m_wrapOffset - 1);
        setMaterial(&fill_material_);
        markDirty(DirtyMaterial);
    }
}

MapPolygonShader::MapPolygonShader() : QSGMaterialShader(*new QSGMaterialShaderPrivate)
{

}

void MapPolygonShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newEffect, QSGMaterial *oldEffect)
{
    Q_ASSERT(oldEffect == nullptr || newEffect->type() == oldEffect->type());
    MapPolygonMaterial *oldMaterial = static_cast<MapPolygonMaterial *>(oldEffect);
    MapPolygonMaterial *newMaterial = static_cast<MapPolygonMaterial *>(newEffect);

    const QColor &c = newMaterial->color();
    const QMatrix4x4 &geoProjection = newMaterial->geoProjection();
    const QDoubleVector3D &center = newMaterial->center();

    QVector3D vecCenter, vecCenter_lowpart;
    for (int i = 0; i < 3; i++)
        QLocationUtils::split_double(center.get(i), &vecCenter[i], &vecCenter_lowpart[i]);

    if (oldMaterial == nullptr || c != oldMaterial->color() || state.isOpacityDirty()) {
        float opacity = state.opacity() * c.alphaF();
        QVector4D v(c.redF() * opacity,
                    c.greenF() *  opacity,
                    c.blueF() * opacity,
                    opacity);
        program()->setUniformValue(m_color_id, v);
    }

    if (state.isMatrixDirty())
    {
        program()->setUniformValue(m_matrix_id, state.projectionMatrix());
    }

    program()->setUniformValue(m_mapProjection_id, geoProjection);

    program()->setUniformValue(m_center_id, vecCenter);
    program()->setUniformValue(m_center_lowpart_id, vecCenter_lowpart);
    program()->setUniformValue(m_wrapOffset_id, float(newMaterial->wrapOffset()));
}

QT_END_NAMESPACE
