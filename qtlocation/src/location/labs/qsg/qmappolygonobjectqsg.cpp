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

#include "qmappolygonobjectqsg_p_p.h"
#include <QtQuick/qsgnode.h>
#include <QtQuick/qsgsimplerectnode.h>
#include <QtPositioning/private/qgeopolygon_p.h>

QT_BEGIN_NAMESPACE

QMapPolygonObjectPrivateQSG::QMapPolygonObjectPrivateQSG(QGeoMapObject *q)
    : QMapPolygonObjectPrivateDefault(q)
{

}

QMapPolygonObjectPrivateQSG::QMapPolygonObjectPrivateQSG(const QMapPolygonObjectPrivate &other)
    : QMapPolygonObjectPrivateDefault(other)
{
    // Data already cloned by the *Default copy constructor, but necessary
    // update operations triggered only by setters overrides
    markSourceDirty();
    updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

QMapPolygonObjectPrivateQSG::~QMapPolygonObjectPrivateQSG()
{
    if (m_map)
        m_map->removeMapObject(q);
}

void QMapPolygonObjectPrivateQSG::setPath(const QList<QGeoCoordinate> &p)
{
    if (p == path())
        return;
    QMapPolygonObjectPrivateDefault::setPath(p);
    markSourceDirty();
    updateGeometry();

    if (m_map)
        emit m_map->sgNodeChanged();
    emit static_cast<QMapPolygonObject *>(q)->pathChanged();
}

void QMapPolygonObjectPrivateQSG::setFillColor(const QColor &color)
{
    QMapPolygonObjectPrivateDefault::setFillColor(color);

    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapPolygonObjectPrivateQSG::setBorderColor(const QColor &color)
{
    QMapPolygonObjectPrivateDefault::setBorderColor(color);

    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapPolygonObjectPrivateQSG::setBorderWidth(qreal width)
{
    QMapPolygonObjectPrivateDefault::setBorderWidth(width);

    if (m_map)
        emit m_map->sgNodeChanged();
}

QGeoMapObjectPrivate *QMapPolygonObjectPrivateQSG::clone()
{
    return new QMapPolygonObjectPrivateQSG(static_cast<QMapPolygonObjectPrivate &>(*this));
}

void QMapPolygonObjectPrivateQSG::setGeoShape(const QGeoShape &shape)
{
    if (shape == m_path)
        return;

    m_path = QGeoPathEager(shape);
    markSourceDirty();
    updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
    emit static_cast<QMapPolygonObject *>(q)->pathChanged();
}

// This is called both when data changes and when viewport changes.
// so handle both cases (sourceDirty, !sourceDirty)
void QMapPolygonObjectPrivateQSG::updateGeometry()
{
    if (!m_map || m_map->geoProjection().projectionType() != QGeoProjection::ProjectionWebMercator)
        return;

    if (m_path.path().length() == 0) { // Possibly cleared
        m_geometry.clear();
        m_borderGeometry.clear();
        return;
    }
    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(m_map->geoProjection());
    if (m_geometry.isSourceDirty() || m_borderGeometry.isSourceDirty()) {
        // This works a bit differently than MapPolygon:
        // the "screen bounds" aren't needed, so update only sources,
        // regardless of the color, as color changes won't trigger polish(),
        // and remember to flag m_dataChanged, that is in principle the same as
        // sourceDirty_, but in practice is cleared in two different codepaths.
        // sourceDirty_ is cleared in any case, dataChanged only if the primitive
        // is effectively visible (e.g., not transparent or border not null)
        m_geometry.setPreserveGeometry(true, m_path.boundingGeoRectangle().topLeft());
        m_borderGeometry.setPreserveGeometry(true, m_path.boundingGeoRectangle().topLeft());
        m_geometry.m_dataChanged = m_borderGeometry.m_dataChanged = true;
        m_geometry.updateSourcePoints(*m_map, m_path);
        m_borderGeometry.updateSourcePoints(*m_map, m_path);
        m_leftBoundMercator = p.geoToMapProjection(m_geometry.origin());
    }
    m_geometry.markScreenDirty();       // ToDo: this needs refactor. It's useless, remove screenDirty_ altogether.
    m_borderGeometry.markScreenDirty();
    m_borderGeometry.m_wrapOffset = m_geometry.m_wrapOffset = p.projectionWrapFactor(m_leftBoundMercator) + 1;
}

QSGNode *QMapPolygonObjectPrivateQSG::updateMapObjectNode(QSGNode *oldNode,
                                                          VisibleNode **visibleNode,
                                                          QSGNode *root,
                                                          QQuickWindow * /*window*/)
{
    if (!m_rootNode || !oldNode) {
        m_rootNode = new QDeclarativePolygonMapItemPrivateOpenGL::RootNode();
        m_node = new MapPolygonNodeGL();
        m_rootNode->appendChildNode(m_node);
        m_polylinenode = new MapPolylineNodeOpenGLExtruded();
        m_rootNode->appendChildNode(m_polylinenode);
        m_rootNode->markDirty(QSGNode::DirtyNodeAdded);
        *visibleNode = static_cast<VisibleNode *>(m_rootNode);
        if (oldNode)
            delete oldNode;
    } else {
        m_rootNode = static_cast<QDeclarativePolygonMapItemPrivateOpenGL::RootNode *>(oldNode);
    }

    const QMatrix4x4 &combinedMatrix = m_map->geoProjection().qsgTransform();
    const QDoubleVector3D &cameraCenter = m_map->geoProjection().centerMercator();

    if (m_borderGeometry.isScreenDirty()) {
        /* Do the border update first */
        m_polylinenode->update(borderColor(),
                               float(borderWidth()),
                               &m_borderGeometry,
                               combinedMatrix,
                               cameraCenter,
                               Qt::SquareCap,
                               true);
        m_borderGeometry.setPreserveGeometry(false);
        m_borderGeometry.markClean();
    }
    if (m_geometry.isScreenDirty()) {
        m_node->update(fillColor(),
                     &m_geometry,
                     combinedMatrix,
                     cameraCenter);
        m_geometry.setPreserveGeometry(false);
        m_geometry.markClean();
    }

    if (!m_polylinenode->isSubtreeBlocked() || !m_node->isSubtreeBlocked()) {
        m_rootNode->setSubtreeBlocked(false);
        root->appendChildNode(m_rootNode);
        return m_rootNode;
    } else {
        m_rootNode->setSubtreeBlocked(true);
        // If the object is currently invisible, but not gone,
        // it is reasonable to assume it will become visible again.
        // However, better not to retain unused data.
        delete m_rootNode;
        m_rootNode = nullptr;
        m_node = nullptr;
        m_polylinenode = nullptr;
        *visibleNode = nullptr;
        return nullptr;
    }
}

void QMapPolygonObjectPrivateQSG::markSourceDirty()
{
    m_geometry.markSourceDirty();
    m_borderGeometry.markSourceDirty();
}

QT_END_NAMESPACE
