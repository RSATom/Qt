/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "qnmeasatelliteinfosource_p.h"
#include <QtPositioning/private/qgeosatelliteinfo_p.h>
#include <QtPositioning/private/qgeosatelliteinfosource_p.h>
#include <QtPositioning/private/qlocationutils_p.h>

#include <QIODevice>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QTimer>
#include <array>
#include <QDebug>
#include <QtCore/QtNumeric>


//QT_BEGIN_NAMESPACE

#define USE_NMEA_PIMPL 1

#if USE_NMEA_PIMPL
class QGeoSatelliteInfoPrivateNmea : public QGeoSatelliteInfoPrivate
{
public:
    QGeoSatelliteInfoPrivateNmea(const QGeoSatelliteInfoPrivate &other);
    QGeoSatelliteInfoPrivateNmea(const QGeoSatelliteInfoPrivateNmea &other);
    virtual ~QGeoSatelliteInfoPrivateNmea();
    virtual QGeoSatelliteInfoPrivate *clone() const;

    QList<QByteArray> nmeaSentences;
};

QGeoSatelliteInfoPrivateNmea::QGeoSatelliteInfoPrivateNmea(const QGeoSatelliteInfoPrivate &other)
:   QGeoSatelliteInfoPrivate(other)
{
}

QGeoSatelliteInfoPrivateNmea::QGeoSatelliteInfoPrivateNmea(const QGeoSatelliteInfoPrivateNmea &other)
:   QGeoSatelliteInfoPrivate(other)
{
    nmeaSentences = other.nmeaSentences;
}

QGeoSatelliteInfoPrivateNmea::~QGeoSatelliteInfoPrivateNmea() {}

QGeoSatelliteInfoPrivate *QGeoSatelliteInfoPrivateNmea::clone() const
{
    return new QGeoSatelliteInfoPrivateNmea(*this);
}
#else
typedef QGeoSatelliteInfoPrivate QGeoSatelliteInfoPrivateNmea;
#endif

class QNmeaSatelliteInfoSourcePrivate : public QObject, public QGeoSatelliteInfoSourcePrivate
{
    Q_OBJECT
public:
    QNmeaSatelliteInfoSourcePrivate(QNmeaSatelliteInfoSource *parent);
    ~QNmeaSatelliteInfoSourcePrivate();

    void startUpdates();
    void stopUpdates();
    void requestUpdate(int msec);
    void notifyNewUpdate();

public slots:
    void readyRead();
    void emitPendingUpdate();
    void sourceDataClosed();
    void updateRequestTimeout();


public:
    QGeoSatelliteInfoSource *m_source = nullptr;
    QGeoSatelliteInfoSource::Error m_satelliteError = QGeoSatelliteInfoSource::NoError;
    QPointer<QIODevice> m_device;
    struct Update {
        QList<QGeoSatelliteInfo> m_satellitesInView;
        QList<QGeoSatelliteInfo> m_satellitesInUse;
        QList<int> m_inUse; // temp buffer for GSA received before GSV
        bool m_validInView = false;
        bool m_validInUse = false;
        bool m_fresh = false;
        bool m_updatingGsv = false;
#if USE_NMEA_PIMPL
        QByteArray gsa;
        QList<QByteArray> gsv;
#endif
        void setSatellitesInView(const QList<QGeoSatelliteInfo> &inView)
        {
            m_updatingGsv = false;
            m_satellitesInView = inView;
            m_validInView = m_fresh = true;
            if (m_inUse.size()) {
                m_satellitesInUse.clear();
                m_validInUse = false;
                bool corrupt = false;
                for (const auto i: m_inUse) {
                    bool found = false;
                    for (const auto &s: m_satellitesInView) {
                        if (s.satelliteIdentifier() == i) {
                            m_satellitesInUse.append(s);
                            found = true;
                        }
                    }
                    if (!found) { // received a GSA before a GSV, but it was incorrect or something. Unrelated to this GSV at least.
                        m_satellitesInUse.clear();
                        corrupt = true;
                        break;
                    }
                }
                m_validInUse = !corrupt;
                m_inUse.clear();
            }
        }

//        void setSatellitesInUse(const QList<QGeoSatelliteInfo> &inUse)
//        {
//            m_satellitesInUse = inUse;
//            m_validInUse = true;
//            m_inUse.clear();
//        }

        bool setSatellitesInUse(const QList<int> &inUse)
        {
            m_satellitesInUse.clear();
            m_validInUse = false;
            m_inUse = inUse;
            if (m_updatingGsv) {
                m_satellitesInUse.clear();
                m_validInView =  false;
                return false;
            }
            for (const auto i: inUse) {
                bool found = false;
                for (const auto &s: m_satellitesInView) {
                    if (s.satelliteIdentifier() == i) {
                        m_satellitesInUse.append(s);
                        found = true;
                    }
                }
                if (!found) {                       // if satellites in use aren't in view, the related GSV is still to be received.
                    m_inUse = inUse;        // So clear outdated data, buffer the info, and set it later.
                    m_satellitesInUse.clear();
                    m_satellitesInView.clear();
                    m_validInView =  false;
                    return false;
                }
            }
            m_validInUse = m_fresh = true;
            return true;
        }

        void consume()
        {
            m_fresh = false;
        }

        bool isFresh()
        {
            return m_fresh;
        }

        QSet<int> inUse() const
        {
            QSet<int> res;
            for (const auto &s: m_satellitesInUse)
                res.insert(s.satelliteIdentifier());
            return res;
        }

        void clear()
        {
            m_satellitesInView.clear();
            m_satellitesInUse.clear();
            m_validInView = m_validInUse = false;
        }

        bool isValid()
        {
            return m_validInView || m_validInUse; // GSV without GSA is valid. GSA with outdated but still matching GSV also valid.
        }
    } m_pendingUpdate, m_lastUpdate;
    bool m_fresh = false;
    bool m_invokedStart = false;
    bool m_noUpdateLastInterval = false;
    bool m_updateTimeoutSent = false;
    bool m_connectedReadyRead = false;
    int m_pushDelay = 20;
    QBasicTimer *m_updateTimer = nullptr; // the timer used in startUpdates()
    QTimer *m_requestTimer = nullptr; // the timer used in requestUpdate()

protected:
    void readAvailableData();
    bool openSourceDevice();
    bool initialize();
    void prepareSourceDevice();
    bool emitUpdated(Update &update);
    void timerEvent(QTimerEvent *event) override;
};

QNmeaSatelliteInfoSourcePrivate::QNmeaSatelliteInfoSourcePrivate(QNmeaSatelliteInfoSource *parent)
:   m_source(parent)
{
}

void QNmeaSatelliteInfoSourcePrivate::notifyNewUpdate()
{
    if (m_pendingUpdate.isValid() && m_pendingUpdate.isFresh()) {
        if (m_requestTimer && m_requestTimer->isActive()) { // User called requestUpdate()
            m_requestTimer->stop();
            emitUpdated(m_pendingUpdate);
        } else if (m_invokedStart) { // user called startUpdates()
            if (m_updateTimer && m_updateTimer->isActive()) { // update interval > 0
                // for periodic updates, only want the most recent update
                if (m_noUpdateLastInterval) {
                    // if the update was invalid when timerEvent was last called, a valid update
                    // should be sent ASAP
                    emitPendingUpdate(); // m_noUpdateLastInterval handled in there.
                }
            } else { // update interval <= 0, send anything new ASAP
                m_noUpdateLastInterval = !emitUpdated(m_pendingUpdate);
            }
        }
    }
}

QNmeaSatelliteInfoSourcePrivate::~QNmeaSatelliteInfoSourcePrivate()
{
    delete m_updateTimer;
}

void QNmeaSatelliteInfoSourcePrivate::startUpdates()
{
    if (m_invokedStart)
        return;

    m_invokedStart = true;
    m_pendingUpdate.clear();
    m_noUpdateLastInterval = false;

    bool initialized = initialize();
    if (!initialized)
        return;

    // Do not support simulation just yet
//    if (m_updateMode == QNmeaPositionInfoSource::RealTimeMode)
    {
        // skip over any buffered data - we only want the newest data.
        // Don't do this in requestUpdate. In that case bufferedData is good to have/use.
        if (m_device->bytesAvailable()) {
            if (m_device->isSequential())
                m_device->readAll();
            else
                m_device->seek(m_device->bytesAvailable());
        }
    }

    if (m_updateTimer)
        m_updateTimer->stop();

    if (m_source->updateInterval() > 0) {
        if (!m_updateTimer)
            m_updateTimer = new QBasicTimer;
        m_updateTimer->start(m_source->updateInterval(), this);
    }

    if (initialized)
        prepareSourceDevice();
}

void QNmeaSatelliteInfoSourcePrivate::stopUpdates()
{
    m_invokedStart = false;
    if (m_updateTimer)
        m_updateTimer->stop();
    m_pendingUpdate.clear();
    m_noUpdateLastInterval = false;
}

void QNmeaSatelliteInfoSourcePrivate::requestUpdate(int msec)
{
    if (m_requestTimer && m_requestTimer->isActive())
        return;

    if (msec <= 0 || msec < m_source->minimumUpdateInterval()) {
        emit m_source->requestTimeout();
        return;
    }

    if (!m_requestTimer) {
        m_requestTimer = new QTimer(this);
        connect(m_requestTimer, SIGNAL(timeout()), SLOT(updateRequestTimeout()));
    }

    bool initialized = initialize();
    if (!initialized) {
        emit m_source->requestTimeout();
        return;
    }

    m_requestTimer->start(msec);
    prepareSourceDevice();
}

void QNmeaSatelliteInfoSourcePrivate::readyRead()
{
    readAvailableData();
}

void QNmeaSatelliteInfoSourcePrivate::emitPendingUpdate()
{
    if (m_pendingUpdate.isValid() && m_pendingUpdate.isFresh()) {
        m_updateTimeoutSent = false;
        m_noUpdateLastInterval = false;
        if (!emitUpdated(m_pendingUpdate))
            m_noUpdateLastInterval = true;
//        m_pendingUpdate.clear(); // Do not clear, it will be incrementally updated
    } else { // invalid or not fresh update
        if (m_noUpdateLastInterval && !m_updateTimeoutSent) {
            m_updateTimeoutSent = true;
            emit m_source->requestTimeout();
        }
        m_noUpdateLastInterval = true;
    }
}

void QNmeaSatelliteInfoSourcePrivate::sourceDataClosed()
{
    if (m_device && m_device->bytesAvailable())
        readAvailableData();
}

void QNmeaSatelliteInfoSourcePrivate::updateRequestTimeout()
{
    m_requestTimer->stop();
    emit m_source->requestTimeout();
}

void QNmeaSatelliteInfoSourcePrivate::readAvailableData()
{
    while (m_device->canReadLine()) {
        char buf[1024];
        qint64 size = m_device->readLine(buf, sizeof(buf));
        QList<int> satInUse;
        const bool satInUseParsed = QLocationUtils::getSatInUseFromNmea(buf, size, satInUse);
        if (satInUseParsed) {
            m_pendingUpdate.setSatellitesInUse(satInUse);
#if USE_NMEA_PIMPL
            m_pendingUpdate.gsa = QByteArray(buf, size);
            if (m_pendingUpdate.m_satellitesInUse.size()) {
                for (auto &s: m_pendingUpdate.m_satellitesInUse)
                    static_cast<QGeoSatelliteInfoPrivateNmea *>(QGeoSatelliteInfoPrivate::get(s))->nmeaSentences.append(m_pendingUpdate.gsa);
                for (auto &s: m_pendingUpdate.m_satellitesInView)
                    static_cast<QGeoSatelliteInfoPrivateNmea *>(QGeoSatelliteInfoPrivate::get(s))->nmeaSentences.append(m_pendingUpdate.gsa);
            }
#endif
        } else {
            const QLocationUtils::GSVParseStatus parserStatus = QLocationUtils::getSatInfoFromNmea(buf, size, m_pendingUpdate.m_satellitesInView);
            if (parserStatus == QLocationUtils::GSVPartiallyParsed) {
                m_pendingUpdate.m_updatingGsv = true;
#if USE_NMEA_PIMPL
                m_pendingUpdate.gsv.append(QByteArray(buf, size));
#endif
            } else if (parserStatus == QLocationUtils::GSVFullyParsed) {
#if USE_NMEA_PIMPL
                m_pendingUpdate.gsv.append(QByteArray(buf, size));
                for (int i = 0; i < m_pendingUpdate.m_satellitesInView.size(); i++) {
                    const QGeoSatelliteInfo &s = m_pendingUpdate.m_satellitesInView.at(i);
                    QGeoSatelliteInfoPrivateNmea *pimpl = new QGeoSatelliteInfoPrivateNmea(*QGeoSatelliteInfoPrivate::get(s));
                    pimpl->nmeaSentences.append(m_pendingUpdate.gsa);
                    pimpl->nmeaSentences.append(m_pendingUpdate.gsv);
                    m_pendingUpdate.m_satellitesInView.replace(i, QGeoSatelliteInfo(*pimpl));
                }
                m_pendingUpdate.gsv.clear();
#endif
                m_pendingUpdate.setSatellitesInView(m_pendingUpdate.m_satellitesInView);
            }
        }
    }
    notifyNewUpdate();
}

bool QNmeaSatelliteInfoSourcePrivate::openSourceDevice()
{
    if (!m_device) {
        qWarning("QNmeaSatelliteInfoSource: no QIODevice data source, call setDevice() first");
        return false;
    }

    if (!m_device->isOpen() && !m_device->open(QIODevice::ReadOnly)) {
        qWarning("QNmeaSatelliteInfoSource: cannot open QIODevice data source");
        return false;
    }

    connect(m_device, SIGNAL(aboutToClose()), SLOT(sourceDataClosed()));
    connect(m_device, SIGNAL(readChannelFinished()), SLOT(sourceDataClosed()));
    connect(m_device, SIGNAL(destroyed()), SLOT(sourceDataClosed()));

    return true;
}

bool QNmeaSatelliteInfoSourcePrivate::initialize()
{
    if (!openSourceDevice())
        return false;

    return true;
}

void QNmeaSatelliteInfoSourcePrivate::prepareSourceDevice()
{
    if (!m_connectedReadyRead) {
        connect(m_device, SIGNAL(readyRead()), SLOT(readyRead()));
        m_connectedReadyRead = true;
    }
}

bool QNmeaSatelliteInfoSourcePrivate::emitUpdated(QNmeaSatelliteInfoSourcePrivate::Update &update)
{
    bool emitted = false;
    if (!update.isFresh())
        return emitted;

    update.consume();
    const bool inUseUpdated = update.m_satellitesInUse != m_lastUpdate.m_satellitesInUse;
    const bool inViewUpdated = update.m_satellitesInView != m_lastUpdate.m_satellitesInView;


    m_lastUpdate = update;
    if (update.m_validInUse && inUseUpdated) {
        emit m_source->satellitesInUseUpdated(update.m_satellitesInUse);
        emitted = true;
    }
    if (update.m_validInView && inViewUpdated) {
        emit m_source->satellitesInViewUpdated(update.m_satellitesInView);
        emitted = true;
    }
    return emitted;
}

void QNmeaSatelliteInfoSourcePrivate::timerEvent(QTimerEvent * /*event*/)
{
    emitPendingUpdate();
}


// currently supports only realtime
QNmeaSatelliteInfoSource::QNmeaSatelliteInfoSource(QObject *parent)
:   QGeoSatelliteInfoSource(*new QNmeaSatelliteInfoSourcePrivate(this), parent)
{
    d = static_cast<QNmeaSatelliteInfoSourcePrivate *>(QGeoSatelliteInfoSourcePrivate::get(*this));
}

QNmeaSatelliteInfoSource::~QNmeaSatelliteInfoSource()
{
    // d deleted in superclass destructor
}

void QNmeaSatelliteInfoSource::setDevice(QIODevice *device)
{
    if (device != d->m_device) {
        if (!d->m_device)
            d->m_device = device;
        else
            qWarning("QNmeaPositionInfoSource: source device has already been set");
    }
}

QIODevice *QNmeaSatelliteInfoSource::device() const
{
    return d->m_device;
}

void QNmeaSatelliteInfoSource::setUpdateInterval(int msec)
{
    int interval = msec;
    if (interval != 0)
        interval = qMax(msec, minimumUpdateInterval());
    QGeoSatelliteInfoSource::setUpdateInterval(interval);
    if (d->m_invokedStart) {
        d->stopUpdates();
        d->startUpdates();
    }
}

int QNmeaSatelliteInfoSource::minimumUpdateInterval() const
{
    return 2; // Some chips are capable of over 100 updates per seconds.
}

QGeoSatelliteInfoSource::Error QNmeaSatelliteInfoSource::error() const
{
    return d->m_satelliteError;
}

void QNmeaSatelliteInfoSource::startUpdates()
{
    d->startUpdates();
}

void QNmeaSatelliteInfoSource::stopUpdates()
{
    d->stopUpdates();
}

void QNmeaSatelliteInfoSource::requestUpdate(int msec)
{
    d->requestUpdate(msec == 0 ? 60000 * 5 : msec); // 5min default timeout
}

void QNmeaSatelliteInfoSource::setError(QGeoSatelliteInfoSource::Error satelliteError)
{
    d->m_satelliteError = satelliteError;
    emit QGeoSatelliteInfoSource::error(satelliteError);
}


//QT_END_NAMESPACE

#include "qnmeasatelliteinfosource.moc"
