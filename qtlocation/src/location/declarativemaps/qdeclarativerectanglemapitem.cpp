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

#include "qdeclarativerectanglemapitem_p.h"
#include "qdeclarativerectanglemapitem_p_p.h"
#include "qdeclarativepolygonmapitem_p.h"
#include "qlocationutils_p.h"
#include <QPainterPath>
#include <qnumeric.h>
#include <QRectF>
#include <QPointF>
#include <QtPositioning/private/qwebmercator_p.h>
#include <QtLocation/private/qgeomap_p.h>
#include <QtPositioning/private/qdoublevector2d_p.h>
#include <QtCore/QScopedValueRollback>

QT_BEGIN_NAMESPACE

/*!
    \qmltype MapRectangle
    \instantiates QDeclarativeRectangleMapItem
    \inqmlmodule QtLocation
    \ingroup qml-QtLocation5-maps
    \since QtLocation 5.5

    \brief The MapRectangle type displays a rectangle on a Map.

    The MapRectangle type displays a rectangle on a Map. Rectangles are a
    special case of Polygon with exactly 4 vertices and 4 "straight" edges. In
    this case, "straight" means that the top-left point has the same latitude
    as the top-right point (the top edge), and the bottom-left point has the
    same latitude as the bottom-right point (the bottom edge). Similarly, the
    points on the left side have the same longitude, and the points on the
    right side have the same longitude.

    To specify the rectangle, it requires a \l topLeft and \l bottomRight point,
    both given by a \l {coordinate}.

    By default, the rectangle is displayed with transparent fill and a 1-pixel
    thick black border. This can be changed using the \l color, \l border.color
    and \l border.width properties.

    \note Similar to the \l MapPolygon type, MapRectangles are geographic
    items, thus dragging a MapRectangle causes its vertices to be recalculated
    in the geographic coordinate space. Apparent stretching of the item
    occurs when dragged to the a different latitude, however, its edges
    remain straight.

    \section2 Performance

    MapRectangles have a rendering cost identical to a MapPolygon with 4
    vertices.

    Like the other map objects, MapRectangle is normally drawn without a smooth
    appearance. Setting the \l opacity property will force the object to be
    blended, which decreases performance considerably depending on the hardware
    in use.

    \section2 Example Usage

    The following snippet shows a map containing a MapRectangle, spanning
    from (-27, 153) to (-28, 153.5), near Brisbane, Australia. The rectangle
    is filled in green, with a 2 pixel black border.

    \code
    Map {
        MapRectangle {
            color: 'green'
            border.width: 2
            topLeft {
                latitude: -27
                longitude: 153
            }
            bottomRight {
                latitude: -28
                longitude: 153.5
            }
        }
    }
    \endcode

    \image api-maprectangle.png
*/

/*!
    \qmlproperty bool QtLocation::MapRectangle::autoFadeIn

    This property holds whether the item automatically fades in when zooming into the map
    starting from very low zoom levels. By default this is \c true.
    Setting this property to \c false causes the map item to always have the opacity specified
    with the \l QtQuick::Item::opacity property, which is 1.0 by default.

    \since 5.14
*/

struct RectangleBackendSelector
{
    RectangleBackendSelector()
    {
        backend = (qgetenv("QTLOCATION_OPENGL_ITEMS").toInt()) ? QDeclarativeRectangleMapItem::OpenGL : QDeclarativeRectangleMapItem::Software;
    }
    QDeclarativeRectangleMapItem::Backend backend = QDeclarativeRectangleMapItem::Software;
};

Q_GLOBAL_STATIC(RectangleBackendSelector, mapRectangleBackendSelector)

QDeclarativeRectangleMapItem::QDeclarativeRectangleMapItem(QQuickItem *parent)
:   QDeclarativeGeoMapItemBase(parent), m_border(this), m_color(Qt::transparent), m_dirtyMaterial(true),
    m_updatingGeometry(false)
  , m_d(new QDeclarativeRectangleMapItemPrivateCPU(*this))
{
    // ToDo: handle envvar, and switch implementation.
    m_itemType = QGeoMap::MapRectangle;
    setFlag(ItemHasContents, true);
    QObject::connect(&m_border, SIGNAL(colorChanged(QColor)),
                     this, SLOT(onLinePropertiesChanged()));
    QObject::connect(&m_border, SIGNAL(widthChanged(qreal)),
                     this, SLOT(onLinePropertiesChanged()));
    setBackend(mapRectangleBackendSelector->backend);
}

QDeclarativeRectangleMapItem::~QDeclarativeRectangleMapItem()
{
}

/*!
    \qmlproperty MapRectangle.Backend QtLocation::MapRectangle::backend

    This property holds which backend is in use to render the map item.
    Valid values are \b MapRectangle.Software and \b{MapRectangle.OpenGL}.
    The default value is \b{MapRectangle.Software}.

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
QDeclarativeRectangleMapItem::Backend QDeclarativeRectangleMapItem::backend() const
{
    return m_backend;
}

void QDeclarativeRectangleMapItem::setBackend(QDeclarativeRectangleMapItem::Backend b)
{
    if (b == m_backend)
        return;
    m_backend = b;
    QScopedPointer<QDeclarativeRectangleMapItemPrivate> d((m_backend == Software)
                                                        ? static_cast<QDeclarativeRectangleMapItemPrivate *>(new QDeclarativeRectangleMapItemPrivateCPU(*this))
                                                        : static_cast<QDeclarativeRectangleMapItemPrivate * >(new QDeclarativeRectangleMapItemPrivateOpenGL(*this)));
    m_d.swap(d);
    m_d->onGeoGeometryChanged();
    emit backendChanged();
}

/*!
    \internal
*/
void QDeclarativeRectangleMapItem::setMap(QDeclarativeGeoMap *quickMap, QGeoMap *map)
{
    QDeclarativeGeoMapItemBase::setMap(quickMap,map);
    if (!map)
        return;
    m_d->onMapSet();
}

/*!
    \qmlpropertygroup Location::MapRectangle::border
    \qmlproperty int MapRectangle::border.width
    \qmlproperty color MapRectangle::border.color

    This property is part of the border property group. The border property group
    holds the width and color used to draw the border of the rectangle.
    The width is in pixels and is independent of the zoom level of the map.

    The default values correspond to a black border with a width of 1 pixel.
    For no line, use a width of 0 or a transparent color.
*/
QDeclarativeMapLineProperties *QDeclarativeRectangleMapItem::border()
{
    return &m_border;
}

/*!
    \qmlproperty coordinate MapRectangle::topLeft

    This property holds the top-left coordinate of the MapRectangle which
    can be used to retrieve its longitude, latitude and altitude.
*/
void QDeclarativeRectangleMapItem::setTopLeft(const QGeoCoordinate &topLeft)
{
    if (m_rectangle.topLeft() == topLeft)
        return;

    m_rectangle.setTopLeft(topLeft);
    m_d->onGeoGeometryChanged();
    emit topLeftChanged(topLeft);
}

QGeoCoordinate QDeclarativeRectangleMapItem::topLeft()
{
    return m_rectangle.topLeft();
}

/*!
    \internal
*/
void QDeclarativeRectangleMapItem::markSourceDirtyAndUpdate()
{
    m_d->markSourceDirtyAndUpdate();
}

void QDeclarativeRectangleMapItem::onLinePropertiesChanged()
{
    m_d->onLinePropertiesChanged();
}

/*!
    \qmlproperty coordinate MapRectangle::bottomRight

    This property holds the bottom-right coordinate of the MapRectangle which
    can be used to retrieve its longitude, latitude and altitude.
*/
void QDeclarativeRectangleMapItem::setBottomRight(const QGeoCoordinate &bottomRight)
{
    if (m_rectangle.bottomRight() == bottomRight)
        return;

    m_rectangle.setBottomRight(bottomRight);
    m_d->onGeoGeometryChanged();
    emit bottomRightChanged(bottomRight);
}

QGeoCoordinate QDeclarativeRectangleMapItem::bottomRight()
{
    return m_rectangle.bottomRight();
}

/*!
    \qmlproperty color MapRectangle::color

    This property holds the fill color of the rectangle. For no fill, use
    a transparent color.
*/
QColor QDeclarativeRectangleMapItem::color() const
{
    return m_color;
}

void QDeclarativeRectangleMapItem::setColor(const QColor &color)
{
    if (m_color == color)
        return;
    m_color = color;
    m_dirtyMaterial = true;
    polishAndUpdate();
    emit colorChanged(m_color);
}

/*!
  \qmlproperty real MapRectangle::opacity

  This property holds the opacity of the item.  Opacity is specified as a
  number between 0 (fully transparent) and 1 (fully opaque).  The default is 1.

  An item with 0 opacity will still receive mouse events. To stop mouse events, set the
  visible property of the item to false.
*/

/*!
    \internal
*/
QSGNode *QDeclarativeRectangleMapItem::updateMapItemPaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    return m_d->updateMapItemPaintNode(oldNode, data);
}

/*!
    \internal
*/
void QDeclarativeRectangleMapItem::updatePolish()
{
    if (!map() || map()->geoProjection().projectionType() != QGeoProjection::ProjectionWebMercator)
        return;
    m_d->updatePolish();
}

/*!
    \internal
*/
void QDeclarativeRectangleMapItem::afterViewportChanged(const QGeoMapViewportChangeEvent &event)
{
    if (event.mapSize.width() <= 0 || event.mapSize.height() <= 0)
        return;
    m_d->afterViewportChanged();
}

/*!
    \internal
*/
bool QDeclarativeRectangleMapItem::contains(const QPointF &point) const
{
    return m_d->contains(point);
}

const QGeoShape &QDeclarativeRectangleMapItem::geoShape() const
{
    return m_rectangle;
}

void QDeclarativeRectangleMapItem::setGeoShape(const QGeoShape &shape)
{
    if (shape == m_rectangle)
        return;

    const QGeoRectangle rectangle = m_rectangle.boundingGeoRectangle();
    const bool tlHasChanged = rectangle.topLeft() != m_rectangle.topLeft();
    const bool brHasChanged = rectangle.bottomRight() != m_rectangle.bottomRight();
    m_rectangle = rectangle;

    m_d->onGeoGeometryChanged();
    if (tlHasChanged)
        emit topLeftChanged(m_rectangle.topLeft());
    if (brHasChanged)
        emit bottomRightChanged(m_rectangle.bottomRight());
}

/*!
    \internal
*/
void QDeclarativeRectangleMapItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    if (!map() || !m_rectangle.isValid() || m_updatingGeometry || newGeometry.topLeft() == oldGeometry.topLeft()) {
        QDeclarativeGeoMapItemBase::geometryChanged(newGeometry, oldGeometry);
        return;
    }
    // TODO: change the algorithm to preserve the distances and size
    QGeoCoordinate newCenter = map()->geoProjection().itemPositionToCoordinate(QDoubleVector2D(newGeometry.center()), false);
    QGeoCoordinate oldCenter = map()->geoProjection().itemPositionToCoordinate(QDoubleVector2D(oldGeometry.center()), false);
    if (!newCenter.isValid() || !oldCenter.isValid())
        return;
    double offsetLongi = newCenter.longitude() - oldCenter.longitude();
    double offsetLati = newCenter.latitude() - oldCenter.latitude();
    if (offsetLati == 0.0 && offsetLongi == 0.0)
        return;

    m_rectangle.translate(offsetLati, offsetLongi);
    m_d->onItemGeometryChanged();
    emit topLeftChanged(m_rectangle.topLeft());
    emit bottomRightChanged(m_rectangle.bottomRight());

    // Not calling QDeclarativeGeoMapItemBase::geometryChanged() as it will be called from a nested
    // call to this function.
}

QDeclarativeRectangleMapItemPrivate::~QDeclarativeRectangleMapItemPrivate() {}

QDeclarativeRectangleMapItemPrivateCPU::~QDeclarativeRectangleMapItemPrivateCPU() {}

QDeclarativeRectangleMapItemPrivateOpenGL::~QDeclarativeRectangleMapItemPrivateOpenGL() {}

QT_END_NAMESPACE
