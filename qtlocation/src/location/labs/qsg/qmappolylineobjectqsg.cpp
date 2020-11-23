/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
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

#include "qmappolylineobjectqsg_p_p.h"
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgsimplerectnode.h>

QT_BEGIN_NAMESPACE

/*
    Note: never use q, since this class is also used inside QMapRouteObjectPrivateQSG!
*/

QMapPolylineObjectPrivateQSG::QMapPolylineObjectPrivateQSG(QGeoMapObject *q)
    : QMapPolylineObjectPrivateDefault(q)
{

}

QMapPolylineObjectPrivateQSG::QMapPolylineObjectPrivateQSG(const QMapPolylineObjectPrivate &other)
    : QMapPolylineObjectPrivateDefault(other)
{
    // rest of the data already cloned by the *Default copy constructor, but necessary
    // update operations triggered only by setters overrides
    markSourceDirty();
    updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

QMapPolylineObjectPrivateQSG::~QMapPolylineObjectPrivateQSG()
{
    if (m_map)
        m_map->removeMapObject(q);
}

QList<QDoubleVector2D> QMapPolylineObjectPrivateQSG::projectPath()
{
    QList<QDoubleVector2D> geopathProjected_;
    if (!m_map || m_map->geoProjection().projectionType() != QGeoProjection::ProjectionWebMercator)
        return geopathProjected_;

    const QGeoProjectionWebMercator &p =
            static_cast<const QGeoProjectionWebMercator&>(m_map->geoProjection());
    geopathProjected_.reserve(m_path.path().size());
    for (const QGeoCoordinate &c : m_path.path())
        geopathProjected_ << p.geoToMapProjection(c);
    return geopathProjected_;
}

void QMapPolylineObjectPrivateQSG::updateGeometry()
{
    if (!m_map || m_map->geoProjection().projectionType() != QGeoProjection::ProjectionWebMercator)
        return;

    if (m_path.path().length() == 0) { // Possibly cleared
        m_borderGeometry.clear();
        return;
    }

    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(m_map->geoProjection());
    if (m_borderGeometry.isSourceDirty()) {
        m_borderGeometry.setPreserveGeometry(true, m_path.boundingGeoRectangle().topLeft());
        m_borderGeometry.m_dataChanged = true;
        m_borderGeometry.updateSourcePoints(*m_map, m_path);
        m_leftBoundMercator = p.geoToMapProjection(m_borderGeometry.origin());
    }
    m_borderGeometry.markScreenDirty();
    m_borderGeometry.m_wrapOffset = p.projectionWrapFactor(m_leftBoundMercator) + 1;
}

/*!
  \internal
*/
unsigned int QMapPolylineObjectPrivateQSG::zoomForLOD(int zoom) const
{
    // LOD Threshold currently fixed to 12 for MapPolylineObject(QSG).
    // ToDo: Consider allowing to change this via DynamicParameter.
    if (zoom >= 12)
        return 30;
    return uint(zoom);
}

QSGNode *QMapPolylineObjectPrivateQSG::updateMapObjectNode(QSGNode *oldNode,
                                                           VisibleNode **visibleNode,
                                                           QSGNode *root,
                                                           QQuickWindow * /*window*/)
{
    if (!m_polylinenode || !oldNode) {
        m_polylinenode = new MapPolylineNodeOpenGLExtruded();
        *visibleNode = static_cast<VisibleNode *>(m_polylinenode);
        if (oldNode)
            delete oldNode;
    } else {
        m_polylinenode = static_cast<MapPolylineNodeOpenGLExtruded *>(oldNode);
    }

    const QMatrix4x4 &combinedMatrix = m_map->geoProjection().qsgTransform();
    const QDoubleVector3D &cameraCenter = m_map->geoProjection().centerMercator();

    if (m_borderGeometry.isScreenDirty()) {
        /* Do the border update first */
        m_polylinenode->update(color(),
                               float(width()),
                               &m_borderGeometry,
                               combinedMatrix,
                               cameraCenter,
                               Qt::SquareCap,
                               true,
                               zoomForLOD(int(m_map->cameraData().zoomLevel())));
        m_borderGeometry.setPreserveGeometry(false);
        m_borderGeometry.markClean();
    }

    if (!m_polylinenode->isSubtreeBlocked() ) {
        m_polylinenode->setSubtreeBlocked(false);
        root->appendChildNode(m_polylinenode);
        return m_polylinenode;
    } else {
        delete m_polylinenode;
        m_polylinenode = nullptr;
        *visibleNode = nullptr;
        return nullptr;
    }
}

QList<QGeoCoordinate> QMapPolylineObjectPrivateQSG::path() const
{
    return m_path.path();
}

void QMapPolylineObjectPrivateQSG::setPath(const QList<QGeoCoordinate> &path)
{
    m_path.setPath(path);
    markSourceDirty();
    updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapPolylineObjectPrivateQSG::setColor(const QColor &color)
{
    QMapPolylineObjectPrivateDefault::setColor(color);

    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapPolylineObjectPrivateQSG::setWidth(qreal width)
{
    QMapPolylineObjectPrivateDefault::setWidth(width);

    if (m_map)
        emit m_map->sgNodeChanged();
}

QGeoMapObjectPrivate *QMapPolylineObjectPrivateQSG::clone()
{
    return new QMapPolylineObjectPrivateQSG(static_cast<QMapPolylineObjectPrivate &>(*this));
}

QGeoShape QMapPolylineObjectPrivateQSG::geoShape() const
{
    return m_path;
}

void QMapPolylineObjectPrivateQSG::markSourceDirty()
{
    m_borderGeometry.markSourceDirty();
}

QT_END_NAMESPACE
