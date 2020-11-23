/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
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
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativenavigator_p.h"
#include "qdeclarativenavigator_p_p.h"
#include <QtLocation/private/qdeclarativegeomap_p.h>
#include <QtLocation/private/qdeclarativegeoserviceprovider_p.h>
#include <QtLocation/private/qnavigationmanager_p.h>
#include <QtLocation/private/qnavigationmanagerengine_p.h>
#include <QtLocation/private/qgeomapparameter_p.h>
#include <QtLocation/private/qdeclarativegeoroute_p.h>
#include <QtLocation/private/qdeclarativegeoroutemodel_p.h>
#include <QtLocation/private/qdeclarativegeoroutesegment_p.h>
#include <QtLocation/qgeoserviceprovider.h>
#include <QtPositioningQuick/private/qdeclarativepositionsource_p.h>
#include <QtQml/qqmlinfo.h>

QT_BEGIN_NAMESPACE

/*!
    \qmlmodule Qt.labs.location 1.0
    \title Qt Labs Location QML Types
    \ingroup qmlmodules
    \brief Provides experimental QtLocation QML types, such as \l Navigator and
    various map objects types (not to be confused with map items).

    To use this module, import the module with the following line:

    \qml
    import Qt.labs.location 1.0
    \endqml

    \note These types are experimental and subject to source-incompatible changes from one
    Qt minor release to the next, until they are ready to be moved to the stable QtLocation QML
    module.
*/

/*!
    \qmltype Navigator
    \instantiates QDeclarativeNavigator
    \inqmlmodule Qt.labs.location
    \ingroup qml-QtLocation5-maps

    \brief The Navigator type offers functionalities to perform turn-by-turn navigation.

    The Navigator purpose is to use a plugin's turn-by-turn navigation implementation in a QML
    application in a seamless manner.
    This object may take control of the map position, orientation, tilting and zoom, as well as changing
    the map style, elements on the map such as direction information.
    In certain cases, it may also restrict user interaction with the Map and with the items on it.
*/

/*!
    \qmlproperty Plugin Qt.labs.location::Navigator::plugin

    This property holds the plugin which provides the navigation functionality.

    This is a write-once property. Once the Navigator has a plugin associated with
    it, any attempted modifications of the plugin property will be ignored.

    \sa Plugin
*/

/*!
    \qmlproperty Map Qt.labs.location::Navigator::map

    This property holds the Map that the navigator is in charge of controlling.

    This is a write-once property. Once the Navigator has a Map associated with
    it, any attempted modifications of the map property will be ignored.

    \sa Map
*/

/*!
    \qmlproperty Route Qt.labs.location::Navigator::route

    This property holds the Route that the navigator is using
    to perform the navigation.

    \note Setting this property while a navigation session is ongoing will
    stop the navigation.

    \sa Route
*/

/*!
    \qmlproperty PositionSource Qt.labs.location::Navigator::positionSource

    This property holds the PositionSource that the navigator will receive position
    updates from to perform the navigation.

    This is a write-once property. Once the Navigator has a PositionSource associated with
    it, any attempted modifications of the positionSource property will be ignored.

    \sa PositionSource
*/

/*!
    \qmlproperty bool Qt.labs.location::Navigator::active

    This property tells whether the Navigator is navigating or not.
    Set this property to \c true to start the navigation.
    Set it to \c false to stop an active navigation session.
*/

/*!
    \qmlproperty bool Qt.labs.location::Navigator::navigatorReady

    This read-only property tells whether the navigator is ready
    to start the navigation or not.
    A Navigator becomes ready once the plugin is attached and a navigation engine has been
    instantiated, and the other required properties are set to valid values.
*/

/*!
    \qmlproperty bool Qt.labs.location::Navigator::trackPositionSource

    This property tells whether the Navigator should control the Map camera to
    keep track of position source updates. This property is enabled (\c true) by
    default, and setting it to \c false is useful in cases where e.g. the user
    starts gesturing over the map area.

    Navigator plugins can also control this property directly e.g. user map
    interaction could trigger the property change. Honoring the user-specified
    value of this property is plugin dependent.
*/

/*!
    \qmlproperty bool Qt.labs.location::Navigator::automaticReroutingEnabled

    This property tells whether the Navigator should automatically recalculate
    the route when the position from \l positionSource ends too far from the route.
    The operation performed in such case is equivalent to calling \l recalculateRoutes.
    The default value is \c true.

    \note Whether this property has any effect is plugin-dependent.
    Also, whether or not it has an effect while the navigator is active is plugin-dependent.
*/

/*!
    \qmlproperty bool Qt.labs.location::Navigator::isOnRoute

    While the Navigator is in active tracking mode, this property tells
    whether the position from \l positionSource is on the route or not.
*/

/*!
    \qmlmethod void Qt.labs.location::Navigator::recalculateRoutes()

    Calling this method forces the backend to trigger a routes recalculation.

    \sa automaticReroutingEnabled
*/

/*!
    \qmlproperty enumeration Qt.labs.location::Navigator::error
    \readonly

    This read-only property holds the latest error value of the geocoding request.

    \value Navigator.NoError
           No error has occurred.
    \value Navigator.NotSupportedError
           Navigation is not supported by the service provider.
    \value Navigator.ConnectionError
           An error occurred while communicating with the service provider.
    \value Navigator.LoaderError
           The geoservice provider library could not be loaded. Setting
           QT_DEBUG_PLUGINS environment variable may help diagnosing the
           problem.
    \value Navigator.UnknownParameterError
           An unknown parameter was specified.
    \value Navigator.MissingRequiredParameterError
           Required parameter was not specified.
    \value Navigator.UnknownError
           Unknown error occurred.
*/

QDeclarativeNavigatorPrivate::QDeclarativeNavigatorPrivate(QParameterizableObject *q_)
    : q(q_), m_params(new QDeclarativeNavigatorParams), m_basicDirections(static_cast<QDeclarativeNavigator *>(q_))
{
}

QDeclarativeNavigator::QDeclarativeNavigator(QObject *parent)
    : QParameterizableObject(parent), d_ptr(new QDeclarativeNavigatorPrivate(this))
{
}

QDeclarativeNavigator::~QDeclarativeNavigator()
{
}

void QDeclarativeNavigator::classBegin()
{
}

void QDeclarativeNavigator::componentComplete()
{
    d_ptr->m_completed = true;
    // Children have been completed
    for (auto param : quickChildren<QGeoMapParameter>())
        d_ptr->m_params->m_parameters.push_back(param);
    if (d_ptr->m_plugin && d_ptr->m_plugin->isAttached())
        pluginReady();
}

QDeclarativeGeoServiceProvider *QDeclarativeNavigator::plugin() const
{
    return d_ptr->m_plugin;
}

void QDeclarativeNavigator::setMap(QDeclarativeGeoMap *map)
{
    if (d_ptr->m_params->m_map || !map) // set once prop
        return;

    d_ptr->m_params->m_map = map;
    connect(map, &QObject::destroyed, this,
            [this]() {
                this->mapChanged();
                this->updateReadyState();
            });
    emit mapChanged();
    updateReadyState();
}

QDeclarativeGeoMap *QDeclarativeNavigator::map() const
{
    return d_ptr->m_params->m_map;
}

void QDeclarativeNavigator::setRoute(QDeclarativeGeoRoute *route)
{
    if (d_ptr->m_params->m_route == route) // This isn't set-once
        return;

    const bool isReady = d_ptr->m_navigator && d_ptr->m_navigator->ready();
    const bool isActive = active();
    if (isReady && isActive)
        setActive(false); // Stop current session

    d_ptr->m_params->m_route = route;
    d_ptr->m_params->m_geoRoute = route ? route->route() : QGeoRoute();
    if (route) {
        connect(route, &QObject::destroyed,
                [this]() {
                    // Do not stop navigation if route disappears. d_ptr->m_geoRoute will still be valid.
                    // Engines can stop navigation if desired.
                    this->routeChanged();
                });
    }
    emit routeChanged();
    updateReadyState();
}

QDeclarativeGeoRoute *QDeclarativeNavigator::route() const
{
    return d_ptr->m_params->m_route;
}

void QDeclarativeNavigator::setPositionSource(QDeclarativePositionSource *positionSource)
{
    if (d_ptr->m_params->m_positionSource || !positionSource) // set once prop
        return;

    d_ptr->m_params->m_positionSource = positionSource;
    QObject::connect(positionSource, &QObject::destroyed,
            [this]() {
                this->positionSourceChanged();
                this->updateReadyState();
            }
    );
    emit positionSourceChanged();
    updateReadyState();
}

QDeclarativePositionSource *QDeclarativeNavigator::positionSource() const
{
    return d_ptr->m_params->m_positionSource;
}

// navigator automatically adjusts route when user leaves it
bool QDeclarativeNavigator::automaticReroutingEnabled() const
{
    if (d_ptr->m_navigator)
        return d_ptr->m_navigator->automaticReroutingEnabled();
    return d_ptr->m_params->m_autoRerouting;
}

// Whether or not it has an effect while the navigator is active should be plugin-dependent
void QDeclarativeNavigator::setAutomaticReroutingEnabled(bool autoRerouting)
{
    const bool autoReroutingOld = automaticReroutingEnabled();
    d_ptr->m_params->m_autoRerouting = autoRerouting;
    // Done this way, and not via signal like setTrackPositionSource because
    // plugins might not support automatic rerouting.
    if (d_ptr->m_navigator)
        d_ptr->m_navigator->setAutomaticReroutingEnabled(autoRerouting);
    if (autoRerouting != autoReroutingOld)
        emit automaticReroutingEnabledChanged();
}


bool QDeclarativeNavigator::navigatorReady() const
{
    if (d_ptr->m_navigator)
        return d_ptr->m_navigator->ready();
    return d_ptr->m_ready;
}

bool QDeclarativeNavigator::trackPositionSource() const
{
    return d_ptr->m_params->m_trackPositionSource;
}

// Navigator is in active tracking mode and the route is being followed.
// This may turn \c false if the user leaves the route.
bool QDeclarativeNavigator::isOnRoute() const
{
    if (d_ptr->m_navigator)
        return d_ptr->m_navigator->isOnRoute();
    return false;
}

void QDeclarativeNavigator::setTrackPositionSource(bool trackPositionSource)
{
    if (trackPositionSource == d_ptr->m_params->m_trackPositionSource)
        return;

    d_ptr->m_params->m_trackPositionSource = trackPositionSource;

    emit trackPositionSourceChanged(trackPositionSource);
}

QDeclarativeNavigationBasicDirections *QDeclarativeNavigator::directions() const
{
    return &d_ptr->m_basicDirections;
}

QDeclarativeNavigator::NavigationError QDeclarativeNavigator::error() const
{
    return d_ptr->m_error;
}

QString QDeclarativeNavigator::errorString() const
{
    return d_ptr->m_errorString;
}

void QDeclarativeNavigator::recalculateRoutes()
{
    if (d_ptr->m_navigator)
        d_ptr->m_navigator->recalculateRoutes();
}

/*  !NOT DOCUMENTED YET!
    \qmlproperty QAbstractNavigator *Qt.labs.location::Navigator::engineHandle

    This property returns a handle to the navigation object created by the engine.
    This object can carry engine-specific properties, signals and methods, to expose
    engine-specific features and data.

    \warning Using this property leads to writing code that's likely to work
             with only a single plugin.
*/
QAbstractNavigator *QDeclarativeNavigator::abstractNavigator() const
{
    return d_ptr->m_navigator.data();

}

bool QDeclarativeNavigator::active() const
{
    return d_ptr->m_active;
}

void QDeclarativeNavigator::setPlugin(QDeclarativeGeoServiceProvider *plugin)
{
    if (d_ptr->m_plugin)
        return; // set once property.

    d_ptr->m_plugin = plugin;
    emit pluginChanged();

    if (d_ptr->m_plugin->isAttached()) {
        pluginReady();
    } else {
        connect(d_ptr->m_plugin, &QDeclarativeGeoServiceProvider::attached,
                this, &QDeclarativeNavigator::pluginReady);
    }
}

void QDeclarativeNavigator::setActive(bool active)
{
    if (d_ptr->m_active == active)
        return;

    d_ptr->m_active = active;
    if (!d_ptr->m_plugin)
        return;

    if (active)
        start();
    else
        stop();
}

void QDeclarativeNavigator::start()
{
    if (!d_ptr->m_ready) {
        qmlWarning(this) << QStringLiteral("Navigation manager not ready.");
        return;
    }

    if (!d_ptr->m_navigator->active())
        d_ptr->m_active = d_ptr->m_navigator->start();
}

void QDeclarativeNavigator::stop()
{
    if (!ensureEngine()) { // If somebody re-set route to null or something, this may become !d_ptr->m_ready
        qmlWarning(this) << QStringLiteral("Navigation manager not ready.");
        return;
    }

    if (d_ptr->m_navigator->active())
        d_ptr->m_active = d_ptr->m_navigator->stop();

    // Cached data are cleared in response to signals emitted by m_navigator upon stop().
    // For example, m_navigator emits currentRouteChanged with an empty route,
    // and QDeclarativeNavigationBasicDirections reacts by clearing the declarative route.
}

void QDeclarativeNavigator::pluginReady()
{
    if (!d_ptr->m_completed)
        return;

    ensureEngine();
    updateReadyState();
    if (d_ptr->m_active)
        start();
}

bool QDeclarativeNavigator::ensureEngine()
{
    if (d_ptr->m_navigator)
        return true;
    if (!d_ptr->m_completed || !d_ptr->m_plugin->isAttached())
        return false;

    QGeoServiceProvider *serviceProvider = d_ptr->m_plugin->sharedGeoServiceProvider();
    // if m_plugin->isAttached(), serviceProvider cannot be null
    QNavigationManager *manager = serviceProvider->navigationManager();

    if (serviceProvider->navigationError() != QGeoServiceProvider::NoError) {
        QDeclarativeNavigator::NavigationError newError = UnknownError;
        switch (serviceProvider->navigationError()) {
        case QGeoServiceProvider::NotSupportedError:
            newError = NotSupportedError; break;
        case QGeoServiceProvider::UnknownParameterError:
            newError = UnknownParameterError; break;
        case QGeoServiceProvider::MissingRequiredParameterError:
            newError = MissingRequiredParameterError; break;
        case QGeoServiceProvider::ConnectionError:
            newError = ConnectionError; break;
        case QGeoServiceProvider::LoaderError:
            newError = LoaderError; break;
        default:
            break;
        }

        setError(newError, serviceProvider->navigationErrorString());
        return false;
    }

    if (!manager) {
        setError(NotSupportedError, tr("Plugin does not support navigation."));
        return false;
    }

    d_ptr->m_navigator.reset(manager->createNavigator(d_ptr->m_params));
    if (!d_ptr->m_navigator) {
        setError(UnknownError, tr("Failed to create a navigator object."));
        return false;
    }

    d_ptr->m_navigator->setLocale(manager->locale());
    d_ptr->m_navigator->setMeasurementSystem(manager->measurementSystem());

    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::activeChanged, this, [this](bool active){
        d_ptr->m_active = active;
        emit activeChanged(active);
    });
    connect(this, &QDeclarativeNavigator::trackPositionSourceChanged, d_ptr->m_navigator.get(), &QAbstractNavigator::setTrackPosition);

    // read-only progress info updates
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::waypointReached,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::waypointReached);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::destinationReached,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::destinationReached);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::currentRouteChanged,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::onCurrentRouteChanged);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::currentRouteLegChanged,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::onCurrentRouteLegChanged);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::currentSegmentChanged,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::currentSegmentChanged);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::nextManeuverIconChanged,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::nextManeuverIconChanged);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::progressInformationChanged,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::progressInformationChanged);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::isOnRouteChanged,
            this, &QDeclarativeNavigator::isOnRouteChanged);
    connect(d_ptr->m_navigator.get(), &QAbstractNavigator::alternativeRoutesChanged,
            &d_ptr->m_basicDirections, &QDeclarativeNavigationBasicDirections::onAlternativeRoutesChanged);

    emit navigatorReadyChanged(true);
    return true;
}

void QDeclarativeNavigator::updateReadyState() {
    const bool oldReady = d_ptr->m_ready;
    if (!d_ptr->m_navigator)
        d_ptr->m_ready = false;
    else
        d_ptr->m_ready = d_ptr->m_navigator->ready();

    if (oldReady != d_ptr->m_ready)
        emit navigatorReadyChanged(d_ptr->m_ready);
}

void QDeclarativeNavigator::setError(QDeclarativeNavigator::NavigationError error, const QString &errorString)
{
    d_ptr->m_error = error;
    d_ptr->m_errorString = errorString;
    emit errorChanged();
}

QDeclarativeNavigationBasicDirections::QDeclarativeNavigationBasicDirections(QDeclarativeNavigator *parent)
:   QObject(parent), m_navigator(parent), m_routes(QByteArrayLiteral("routeData"), this)
{
    if (m_navigator)
        m_navigatorPrivate = m_navigator->d_ptr.data();
}

/*!
    \qmlpropertygroup Qt.labs.location::Navigator::directions
    \readonly
    \qmlproperty Variant Qt.labs.location::Navigator::directions.nextManeuverIcon
    \qmlproperty real Qt.labs.location::Navigator::directions.distanceToNextManeuver
    \qmlproperty real Qt.labs.location::Navigator::directions.remainingTravelDistance
    \qmlproperty real Qt.labs.location::Navigator::directions.remainingTravelDistanceToNextWaypoint
    \qmlproperty real Qt.labs.location::Navigator::directions.traveledDistance
    \qmlproperty int Qt.labs.location::Navigator::directions.timeToNextManeuver
    \qmlproperty int Qt.labs.location::Navigator::directions.remainingTravelTime
    \qmlproperty int Qt.labs.location::Navigator::directions.remainingTravelTimeToNextWaypoint
    \qmlproperty int Qt.labs.location::Navigator::directions.traveledTime
    \qmlproperty Route Qt.labs.location::Navigator::directions.currentRoute
    \qmlproperty RouteLeg Qt.labs.location::Navigator::directions.currentRouteLeg
    \qmlproperty int Qt.labs.location::Navigator::directions.currentSegment
    \qmlproperty model Qt.labs.location::Navigator::directions.alternativeRoutes

    These read-only properties are part of the \e directions property group.
    This property group holds the navigation progress information that can be
    used to access the route data and to extract directions.

    \note Some backends might not provide a full set of navigation progress
          information.

    \list
        \li The \c nextManeuverIcon property holds the next turn icon.
        \li The \c distanceToNextManeuver property holds the distance to the
            next maneuver, in meters.
        \li The \c remainingTravelDistance property holds the remaining travel
            distance, in meters.
        \li The \c remainingTravelDistanceToNextWaypoint property holds the
            remaining travel distance to the next waypoint, in meters.
        \li The \c traveledDistance property holds the traveled distance, in
            meters.
        \li The \c timeToNextManeuver property holds the time to the next
            maneuver, in milliseconds.
        \li The \c remainingTravelTime property holds the remaining travel
            time, in milliseconds.
        \li The \c remainingTravelTimeToNextWaypoint property holds the
            remaining travel time to the next waypoint, in milliseconds.
        \li The \c traveledTime property holds the traveled time, in
            milliseconds.
        \li The \c currentRoute property holds the current route the navigator
            is following. This can be the same as \l route, or can be
            different, if the navigator cannot follow the user-specified route.
            For example, if the position coming from \l positionSource is
            considerably off route, the navigation engine may recalculate and
            start to follow a new route.
        \li The \c currentRouteLeg property holds the current route leg the
            navigator is following. This is always a part of \c currentRoute,
            so the \l {RouteLeg::}{overallRoute} property of \c currentRouteLeg
            holds the same route as \c currentRoute.
        \li The \c currentSegment property holds the index of the current
            RouteSegment in the \c currentRoute.
        \li The \c alternativeRoutes property holds the list of alternative routes provided by
            the engine. If no alternative routes are present, the model will be empty.
    \endlist

    \sa directions.waypointReached(), directions.destinationReached(), Route, RouteLeg, RouteSegment, Waypoint
*/

/*!
    \qmlsignal Qt.labs.location::Navigator::directions.waypointReached(Waypoint waypoint)

    This signal is emitted when a \a waypoint has been reached.

    \sa directions, directions.destinationReached()
*/

/*!
    \qmlsignal Qt.labs.location::Navigator::directions.destinationReached()

    This signal is emitted when the last waypoint of the route, the
    destination, has been reached.

    \sa directions, directions.waypointReached()
*/
QVariant QDeclarativeNavigationBasicDirections::nextManeuverIcon() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->nextManeuverIcon();
    return QVariant();
}

qreal QDeclarativeNavigationBasicDirections::distanceToNextManeuver() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->distanceToNextManeuver();
    return qQNaN();
}

qreal QDeclarativeNavigationBasicDirections::remainingTravelDistance() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->remainingTravelDistance();
    return qQNaN();
}

qreal QDeclarativeNavigationBasicDirections::remainingTravelDistanceToNextWaypoint() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->remainingTravelDistanceToNextWaypoint();
    return qQNaN();
}

qreal QDeclarativeNavigationBasicDirections::traveledDistance() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->traveledDistance();
    return 0;
}

int QDeclarativeNavigationBasicDirections::timeToNextManeuver() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->timeToNextManeuver();
    return -1;
}

int QDeclarativeNavigationBasicDirections::remainingTravelTime() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->remainingTravelTime();
    return -1;
}

int QDeclarativeNavigationBasicDirections::remainingTravelTimeToNextWaypoint() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->remainingTravelTimeToNextWaypoint();
    return -1;
}

int QDeclarativeNavigationBasicDirections::traveledTime() const
{
    if (m_navigatorPrivate->m_navigator)
        return m_navigatorPrivate->m_navigator->traveledTime();
    return 0;
}

QDeclarativeGeoRoute *QDeclarativeNavigationBasicDirections::currentRoute() const
{
    if (!m_navigatorPrivate->m_ready
            || !m_navigatorPrivate->m_navigator
            || !m_navigatorPrivate->m_navigator->active())
        return m_navigatorPrivate->m_params->m_route.data(); // the user-specified route, if any
    return m_currentRoute;
}

QDeclarativeGeoRouteLeg *QDeclarativeNavigationBasicDirections::currentRouteLeg() const
{
    if (!m_navigatorPrivate->m_ready
            || !m_navigatorPrivate->m_navigator
            || !m_navigatorPrivate->m_navigator->active())
        return nullptr;
    return m_currentRouteLeg;
}

int QDeclarativeNavigationBasicDirections::currentSegment() const
{
    if (!m_navigatorPrivate->m_ready
            || !m_navigatorPrivate->m_navigator
            || !m_navigatorPrivate->m_navigator->active())
        return 0;
    return m_navigatorPrivate->m_navigator->currentSegment();
}

QAbstractItemModel *QDeclarativeNavigationBasicDirections::alternativeRoutes()
{
    return &m_routes;
}

void QDeclarativeNavigationBasicDirections::onCurrentRouteChanged()
{
    if (m_currentRoute)
        m_currentRoute->deleteLater();
    m_currentRoute = new QDeclarativeGeoRoute(m_navigatorPrivate->m_navigator->currentRoute(), this);
    emit currentRouteChanged();
}

void QDeclarativeNavigationBasicDirections::onCurrentRouteLegChanged()
{
    if (m_currentRouteLeg)
        m_currentRouteLeg->deleteLater();
    m_currentRouteLeg = new QDeclarativeGeoRouteLeg(m_navigatorPrivate->m_navigator->currentRouteLeg(), this);
    emit currentRouteLegChanged();
}

void QDeclarativeNavigationBasicDirections::onAlternativeRoutesChanged()
{
    const QList<QGeoRoute> &routes = m_navigatorPrivate->m_navigator->alternativeRoutes();
    QList<QDeclarativeGeoRoute *> declarativeRoutes;
    for (int i = 0; i < routes.size(); ++i) {
        QDeclarativeGeoRoute *route = new QDeclarativeGeoRoute(routes.at(i), &m_routes);
        QQmlEngine::setContextForObject(route, QQmlEngine::contextForObject(this));
        declarativeRoutes.append(route);
    }
    m_routes.updateData(declarativeRoutes);
}

QT_END_NAMESPACE

