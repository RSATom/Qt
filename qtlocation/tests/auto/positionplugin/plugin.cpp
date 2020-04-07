/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
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
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtPositioning/qgeopositioninfosource.h>
#include <QtPositioning/private/qgeopositioninfosource_p.h>
#include <QtPositioning/qgeopositioninfosourcefactory.h>
#include <QObject>
#include <QtPlugin>
#include <QTimer>

QT_USE_NAMESPACE

class DummySource : public QGeoPositionInfoSource
{
    Q_OBJECT

public:
    DummySource(const QVariantMap &parameters, QObject *parent=0);
    ~DummySource();

    void startUpdates();
    void stopUpdates();
    void requestUpdate(int timeout=5000);

    QGeoPositionInfo lastKnownPosition(bool fromSatellitePositioningMethodsOnly) const;
    PositioningMethods supportedPositioningMethods() const;

    void setUpdateInterval(int msec);
    int minimumUpdateInterval() const;
    Error error() const;

private:
    QTimer *timer;
    QTimer *timeoutTimer;
    QTimer *singleTimer;
    QGeoPositionInfo lastPosition;
    QDateTime lastUpdateTime;

private slots:
    void updatePosition();
    void doTimeout();
};

class DummySourcePrivate : public QGeoPositionInfoSourcePrivate
{
public:
    bool setBackendProperty(const QString &name, const QVariant &value) override
    {
        if (name == QStringLiteral("altitude")) {
            m_altitude = value.toReal();
            return true;
        }
        return false;
    }
    QVariant backendProperty(const QString &name) const override
    {
        if (name == QStringLiteral("altitude"))
            return m_altitude;
        return QVariant();
    }

    qreal m_altitude = 0.0;
};

DummySource::DummySource(const QVariantMap &parameters, QObject *parent) :
    QGeoPositionInfoSource(*new DummySourcePrivate, parent),
    timer(new QTimer(this)),
    timeoutTimer(new QTimer(this)),
    singleTimer(new QTimer(this)),
    lastPosition(QGeoCoordinate(0,0), QDateTime::currentDateTime())
{
    DummySourcePrivate *dd = static_cast<DummySourcePrivate *>(QGeoPositionInfoSourcePrivate::get(*this));
    if (parameters.contains(QStringLiteral("test.source.altitude"))) {
        const qreal alti = parameters.value(QStringLiteral("test.source.altitude")).toReal();
        dd->m_altitude = alti;
        QGeoCoordinate crd = lastPosition.coordinate();
        crd.setAltitude(alti);
        lastPosition.setCoordinate(crd);
    }
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(updatePosition()));
    connect(singleTimer, SIGNAL(timeout()),
            this, SLOT(updatePosition()));
    connect(timeoutTimer, SIGNAL(timeout()),
            this, SLOT(doTimeout()));
}

QGeoPositionInfoSource::Error DummySource::error() const
{
    return QGeoPositionInfoSource::NoError;
}


void DummySource::setUpdateInterval(int msec)
{
    if (msec == 0) {
        timer->setInterval(1000);
    } else if (msec < 1000) {
        msec = 1000;
        timer->setInterval(msec);
    } else {
        timer->setInterval(msec);
    }

    QGeoPositionInfoSource::setUpdateInterval(msec);
}

int DummySource::minimumUpdateInterval() const
{
    return 1000;
}

QGeoPositionInfo DummySource::lastKnownPosition(bool fromSatellitePositioningMethodsOnly) const
{
    Q_UNUSED(fromSatellitePositioningMethodsOnly);
    return lastPosition;
}

QGeoPositionInfoSource::PositioningMethods DummySource::supportedPositioningMethods() const
{
    return QGeoPositionInfoSource::AllPositioningMethods;
}

void DummySource::startUpdates()
{
    timer->start();
}

void DummySource::stopUpdates()
{
    timer->stop();
}

void DummySource::requestUpdate(int timeout)
{
    if (timeout == 0)
        timeout = 5000;
    if (timeout < 0)
        timeout = 0;

    timeoutTimer->setInterval(timeout);
    timeoutTimer->start();

    if (timer->isActive()) {
        timer->stop();
        timer->start();
    }

    singleTimer->setInterval(1000);
    singleTimer->start();
}

DummySource::~DummySource()
{}

void DummySource::updatePosition()
{
    DummySourcePrivate *dd = static_cast<DummySourcePrivate *>(QGeoPositionInfoSourcePrivate::get(*this));
    timeoutTimer->stop();
    singleTimer->stop();

    const QDateTime now = QDateTime::currentDateTime();

    QGeoCoordinate coord(lastPosition.coordinate().latitude() + 0.1,
                         lastPosition.coordinate().longitude() + 0.1,
                         dd->m_altitude);

    QGeoPositionInfo info(coord, now);
    info.setAttribute(QGeoPositionInfo::Direction, lastPosition.coordinate().azimuthTo(coord));
    if (lastUpdateTime.isValid()) {
        double speed = lastPosition.coordinate().distanceTo(coord) / lastUpdateTime.msecsTo(now);
        info.setAttribute(QGeoPositionInfo::GroundSpeed, 1000 * speed);
    }

    lastUpdateTime = now;
    lastPosition = info;
    emit positionUpdated(info);
}

void DummySource::doTimeout()
{
    timeoutTimer->stop();
    singleTimer->stop();
    emit updateTimeout();
}


class QGeoPositionInfoSourceFactoryTest : public QObject, public QGeoPositionInfoSourceFactoryV2
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.qt.position.sourcefactory/5.0"
                      FILE "plugin.json")
    Q_INTERFACES(QGeoPositionInfoSourceFactoryV2)

public:
    QGeoPositionInfoSource *positionInfoSource(QObject *parent);
    QGeoSatelliteInfoSource *satelliteInfoSource(QObject *parent);
    QGeoAreaMonitorSource *areaMonitor(QObject *parent);

    QGeoPositionInfoSource *positionInfoSourceWithParameters(QObject *parent, const QVariantMap &parameters);
    QGeoSatelliteInfoSource *satelliteInfoSourceWithParameters(QObject *parent, const QVariantMap &parameters);
    QGeoAreaMonitorSource *areaMonitorWithParameters(QObject *parent, const QVariantMap &parameters);
};

QGeoPositionInfoSource *QGeoPositionInfoSourceFactoryTest::positionInfoSource(QObject *parent)
{
    return new DummySource(QVariantMap(), parent);
}

QGeoSatelliteInfoSource *QGeoPositionInfoSourceFactoryTest::satelliteInfoSource(QObject *parent)
{
    return satelliteInfoSourceWithParameters(parent, QVariantMap());
}

QGeoAreaMonitorSource *QGeoPositionInfoSourceFactoryTest::areaMonitor(QObject* parent)
{
    return areaMonitorWithParameters(parent, QVariantMap());
}

QGeoPositionInfoSource *QGeoPositionInfoSourceFactoryTest::positionInfoSourceWithParameters(QObject *parent, const QVariantMap &parameters)
{
    return new DummySource(parameters, parent);
}

QGeoSatelliteInfoSource *QGeoPositionInfoSourceFactoryTest::satelliteInfoSourceWithParameters(QObject *parent, const QVariantMap &parameters)
{
    Q_UNUSED(parent);
    Q_UNUSED(parameters)
    return nullptr;
}

QGeoAreaMonitorSource *QGeoPositionInfoSourceFactoryTest::areaMonitorWithParameters(QObject *parent, const QVariantMap &parameters)
{
    Q_UNUSED(parent);
    Q_UNUSED(parameters)
    return nullptr;
}

#include "plugin.moc"
