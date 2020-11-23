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

#include "qmapcircleobjectqsg_p_p.h"

QT_BEGIN_NAMESPACE

static const int CircleSamples = 128;

QMapCircleObjectPrivateQSG::QMapCircleObjectPrivateQSG(QGeoMapObject *q)
    : QMapCircleObjectPrivateDefault(q), m_dataCPU(new CircleDataCPU)
{

}

QMapCircleObjectPrivateQSG::QMapCircleObjectPrivateQSG(const QMapCircleObjectPrivate &other)
    : QMapCircleObjectPrivateDefault(other), m_dataCPU(new CircleDataCPU)
{
    // Data already cloned by the *Default copy constructor, but necessary
    // update operations triggered only by setters overrides
    if (!QDeclarativeCircleMapItemPrivateCPU::crossEarthPole(center(), radius()))
        switchToGL(); // this marks source dirty

    updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

QMapCircleObjectPrivateQSG::~QMapCircleObjectPrivateQSG()
{
    if (m_map)
        m_map->removeMapObject(q);
}

void QMapCircleObjectPrivateQSG::updateGeometry()
{
    if (!m_dataGL.isNull())
        updateGeometryGL();
    else
        updateGeometryCPU();
}

void QMapCircleObjectPrivateQSG::CircleDataCPU::updateCirclePath(const QGeoCoordinate &center, qreal radius, const QGeoProjectionWebMercator &p)
{
    QList<QGeoCoordinate> path;
    QDeclarativeCircleMapItemPrivateCPU::calculatePeripheralPoints(path, center, radius, CircleSamples, m_leftBound);
    m_circlePath.clear();
    for (const QGeoCoordinate &c : path)
        m_circlePath << p.geoToMapProjection(c);
}

void QMapCircleObjectPrivateQSG::updateGeometryCPU()
{
    if (!m_map || m_map->geoProjection().projectionType() != QGeoProjection::ProjectionWebMercator
            || !qIsFinite(radius()) || !center().isValid())
        return;

    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(m_map->geoProjection());
    QScopedValueRollback<bool> rollback(m_dataCPU->m_updatingGeometry);
    m_dataCPU->m_updatingGeometry = true;

    m_dataCPU->updateCirclePath(center(), radius(), p);
    QList<QDoubleVector2D> circlePath = m_dataCPU->m_circlePath;

    int pathCount = circlePath.size();
    bool preserve = QDeclarativeCircleMapItemPrivateCPU::preserveCircleGeometry(circlePath, center(), radius(), p);
    // using leftBound_ instead of the analytically calculated circle_.boundingGeoRectangle().topLeft());
    // to fix QTBUG-62154
    m_dataCPU->m_geometry.markSourceDirty();
    m_dataCPU->m_geometry.setPreserveGeometry(true, m_dataCPU->m_leftBound); // to set the geoLeftBound_
    m_dataCPU->m_geometry.setPreserveGeometry(preserve, m_dataCPU->m_leftBound);

    bool invertedCircle = false;
    if (QDeclarativeCircleMapItemPrivateCPU::crossEarthPole(center(), radius()) && circlePath.size() == pathCount) {
        m_dataCPU->m_geometry.updateScreenPointsInvert(circlePath, *m_map); // invert fill area for really huge circles
        invertedCircle = true;
    } else {
        m_dataCPU->m_geometry.updateSourcePoints(*m_map, circlePath);
        m_dataCPU->m_geometry.updateScreenPoints(*m_map);
    }

    m_dataCPU->m_borderGeometry.clear();

    //if (borderColor() != Qt::transparent && borderWidth() > 0)
    {
        QList<QDoubleVector2D> closedPath = circlePath;
        closedPath << closedPath.first();

        if (invertedCircle) {
            closedPath = m_dataCPU->m_circlePath;
            closedPath << closedPath.first();
            std::reverse(closedPath.begin(), closedPath.end());
        }

        m_dataCPU->m_borderGeometry.markSourceDirty();
        m_dataCPU->m_borderGeometry.setPreserveGeometry(true, m_dataCPU->m_leftBound);
        m_dataCPU->m_borderGeometry.setPreserveGeometry(preserve, m_dataCPU->m_leftBound);

        // Use srcOrigin_ from fill geometry after clipping to ensure that translateToCommonOrigin won't fail.
        const QGeoCoordinate &geometryOrigin = m_dataCPU->m_geometry.origin();

        m_dataCPU->m_borderGeometry.clearSource();

        QDoubleVector2D borderLeftBoundWrapped;
        QList<QList<QDoubleVector2D > > clippedPaths =
                m_dataCPU->m_borderGeometry.clipPath(*m_map, closedPath, borderLeftBoundWrapped);
        if (clippedPaths.size()) {
            borderLeftBoundWrapped = p.geoToWrappedMapProjection(geometryOrigin);
            m_dataCPU->m_borderGeometry.pathToScreen(*m_map, clippedPaths, borderLeftBoundWrapped);
            m_dataCPU->m_borderGeometry.updateScreenPoints(*m_map, borderWidth(), false);
        } else {
            m_dataCPU->m_borderGeometry.clear();
        }
    }

    QPointF origin = m_map->geoProjection().coordinateToItemPosition(m_dataCPU->m_geometry.origin(), false).toPointF();
    m_dataCPU->m_geometry.translate(origin - m_dataCPU->m_geometry.firstPointOffset());
    m_dataCPU->m_borderGeometry.translate(origin - m_dataCPU->m_borderGeometry.firstPointOffset());
}

void QMapCircleObjectPrivateQSG::CircleDataGL::updateCirclePath(const QGeoCoordinate &center, qreal radius, const QGeoProjectionWebMercator &p)
{
    m_circlePath.clear();
    if (radius < 0.001) // 1mm is small enough, probably already way too small.
        return;
    QDeclarativeCircleMapItemPrivate::calculatePeripheralPoints(m_circlePath,
                                                                center,
                                                                radius,
                                                                CircleSamples,
                                                                m_leftBound);

    m_leftBoundMercator = p.geoToMapProjection(m_leftBound);
    m_geometry.setPreserveGeometry(true, m_leftBound);
    m_borderGeometry.setPreserveGeometry(true, m_leftBound);
}

void QMapCircleObjectPrivateQSG::updateGeometryGL()
{
    if (!m_map || m_map->geoProjection().projectionType() != QGeoProjection::ProjectionWebMercator)
        return;

    const QGeoProjectionWebMercator &p = static_cast<const QGeoProjectionWebMercator&>(m_map->geoProjection());
    if (m_dataGL->m_geometry.isSourceDirty()
            || m_dataGL->m_borderGeometry.isSourceDirty()) {
        m_dataGL->updateCirclePath(center(), radius(), p);

        if (m_dataGL->m_circlePath.length() == 0) { // Possibly cleared
            m_dataGL->m_geometry.clear();
            m_dataGL->m_borderGeometry.clear();
            return;
        }
        m_dataGL->m_geometry.m_dataChanged = m_dataGL->m_borderGeometry.m_dataChanged = true;
        m_dataGL->m_geometry.updateSourcePoints(*m_map, m_dataGL->m_circlePath);
        m_dataGL->m_borderGeometry.updateSourcePoints(*m_map, QGeoCircle(center(), radius()));
        m_dataGL->m_circlePath.clear(); // not needed anymore
    }
    m_dataGL->m_geometry.markScreenDirty();       // ToDo: this needs refactor. It's useless, remove screenDirty_ altogether.
    m_dataGL->m_borderGeometry.markScreenDirty();
    m_dataGL->m_borderGeometry.m_wrapOffset = m_dataGL->m_geometry.m_wrapOffset = p.projectionWrapFactor(m_dataGL->m_leftBoundMercator) + 1;
}

QGeoMapObjectPrivate *QMapCircleObjectPrivateQSG::clone()
{
    return new QMapCircleObjectPrivateQSG(static_cast<QMapCircleObjectPrivate &>(*this));
}

void QMapCircleObjectPrivateQSG::switchToGL()
{
    if (!m_dataGL.isNull())
        return;
    QScopedPointer<CircleDataGL> data(new CircleDataGL);
    m_dataGL.swap(data);
    m_dataGL->markSourceDirty();
    m_dataCPU.reset(nullptr);
}

void QMapCircleObjectPrivateQSG::switchToCPU()
{
    if (!m_dataCPU.isNull())
        return;
    QScopedPointer<CircleDataCPU> data(new CircleDataCPU);
    m_dataCPU.swap(data);
    m_dataGL.reset(nullptr);
}

QSGNode *QMapCircleObjectPrivateQSG::updateMapObjectNode(QSGNode *oldNode,
                                                         VisibleNode **visibleNode,
                                                         QSGNode *root,
                                                         QQuickWindow * window)
{
    if (!m_dataGL.isNull())
        return updateMapObjectNodeGL(oldNode, visibleNode, root, window);
    else
        return updateMapObjectNodeCPU(oldNode, visibleNode, root, window);
}

QSGNode *QMapCircleObjectPrivateQSG::updateMapObjectNodeCPU(QSGNode *oldNode,
                                                            VisibleNode **visibleNode,
                                                            QSGNode *root,
                                                            QQuickWindow */*window*/)
{
    if (!m_dataCPU->m_node || !oldNode) {
        m_dataCPU->m_node = new MapPolygonNode();
        *visibleNode = static_cast<VisibleNode *>(m_dataCPU->m_node);
        if (oldNode)
            delete oldNode;
    } else {
        m_dataCPU->m_node = static_cast<MapPolygonNode *>(oldNode);
    }

    if (!m_dataCPU->m_geometry.size() && !m_dataCPU->m_borderGeometry.size()) {
        visibleNode = nullptr;
        return nullptr;
    }

    //TODO: update only material
    if (m_dataCPU->m_geometry.isScreenDirty() || m_dataCPU->m_borderGeometry.isScreenDirty() || oldNode != m_dataCPU->m_node) {
        //QMapPolygonObject *p = static_cast<QMapPolygonObject *>(q);
        m_dataCPU->m_node->update(color(), borderColor(), &m_dataCPU->m_geometry, &m_dataCPU->m_borderGeometry);
        m_dataCPU->m_geometry.setPreserveGeometry(false);
        m_dataCPU->m_borderGeometry.setPreserveGeometry(false);
        m_dataCPU->m_geometry.markClean();
        m_dataCPU->m_borderGeometry.markClean();
    }

    if (m_dataCPU->m_geometry.size() || m_dataCPU->m_borderGeometry.size()) {
        root->appendChildNode(m_dataCPU->m_node);
    } else {
        delete m_dataCPU->m_node;
        m_dataCPU->m_node = nullptr;
        visibleNode = nullptr;
        return nullptr;
    }
    return m_dataCPU->m_node;
}

QSGNode *QMapCircleObjectPrivateQSG::updateMapObjectNodeGL(QSGNode *oldNode,
                                                           VisibleNode **visibleNode,
                                                           QSGNode *root,
                                                           QQuickWindow */*window*/)
{
    if (!m_dataGL->m_rootNode || !oldNode) {
        m_dataGL->m_rootNode = new QDeclarativePolygonMapItemPrivateOpenGL::RootNode();
        m_dataGL->m_node = new MapPolygonNodeGL();
        m_dataGL->m_rootNode->appendChildNode(m_dataGL->m_node);
        m_dataGL->m_polylinenode = new MapPolylineNodeOpenGLExtruded();
        m_dataGL->m_rootNode->appendChildNode(m_dataGL->m_polylinenode);
        m_dataGL->m_rootNode->markDirty(QSGNode::DirtyNodeAdded);
        *visibleNode = static_cast<VisibleNode *>(m_dataGL->m_rootNode);
        if (oldNode)
            delete oldNode;
    } else {
        m_dataGL->m_rootNode = static_cast<QDeclarativePolygonMapItemPrivateOpenGL::RootNode *>(oldNode);
    }

    const QMatrix4x4 &combinedMatrix = m_map->geoProjection().qsgTransform();
    const QDoubleVector3D &cameraCenter = m_map->geoProjection().centerMercator();

    if (m_dataGL->m_borderGeometry.isScreenDirty()) {
        /* Do the border update first */
        m_dataGL->m_polylinenode->update(borderColor(),
                               float(borderWidth()),
                               &m_dataGL->m_borderGeometry,
                               combinedMatrix,
                               cameraCenter,
                               Qt::SquareCap,
                               true); // No LOD for circles ATM
        m_dataGL->m_borderGeometry.setPreserveGeometry(false);
        m_dataGL->m_borderGeometry.markClean();
    }
    if (m_dataGL->m_geometry.isScreenDirty()) {
        m_dataGL->m_node->update(color(),
                     &m_dataGL->m_geometry,
                     combinedMatrix,
                     cameraCenter);
        m_dataGL->m_geometry.setPreserveGeometry(false);
        m_dataGL->m_geometry.markClean();
    }

    if (!m_dataGL->m_polylinenode->isSubtreeBlocked() || !m_dataGL->m_node->isSubtreeBlocked()) {
        m_dataGL->m_rootNode->setSubtreeBlocked(false);
        root->appendChildNode(m_dataGL->m_rootNode);
        return m_dataGL->m_rootNode;
    } else {
        delete m_dataGL->m_rootNode;
        m_dataGL->m_rootNode = nullptr;
        m_dataGL->m_node = nullptr;
        m_dataGL->m_polylinenode = nullptr;
        *visibleNode = nullptr;
        return nullptr;
    }
}

void QMapCircleObjectPrivateQSG::setCenter(const QGeoCoordinate &center)
{
    QMapCircleObjectPrivateDefault::setCenter(center);
    if (!QDeclarativeCircleMapItemPrivate::crossEarthPole(this->center(), this->radius())) // Switching implementation for circles crossing/not crossing poles
        switchToGL();
    else
        switchToCPU();

    if (!m_dataGL.isNull())
        m_dataGL->markSourceDirty();

    updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapCircleObjectPrivateQSG::setRadius(qreal radius)
{
    QMapCircleObjectPrivateDefault::setRadius(radius);
    if (!QDeclarativeCircleMapItemPrivate::crossEarthPole(this->center(), this->radius())) // Switching implementation for circles crossing/not crossing poles
        switchToGL();
    else
        switchToCPU();

    if (!m_dataGL.isNull())
        m_dataGL->markSourceDirty();

    updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapCircleObjectPrivateQSG::setColor(const QColor &color)
{
    QMapCircleObjectPrivateDefault::setColor(color);
    if (!m_dataCPU.isNull())
        updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapCircleObjectPrivateQSG::setBorderColor(const QColor &color)
{
    QMapCircleObjectPrivateDefault::setBorderColor(color);
    if (!m_dataCPU.isNull())
        updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

void QMapCircleObjectPrivateQSG::setBorderWidth(qreal width)
{
    QMapCircleObjectPrivateDefault::setBorderWidth(width);
    if (!m_dataCPU.isNull())
        updateGeometry();
    if (m_map)
        emit m_map->sgNodeChanged();
}

QT_END_NAMESPACE
