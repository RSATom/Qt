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

#include "qgeopositioninfosourcefactory_serialnmea.h"
#include <QtPositioning/qnmeapositioninfosource.h>
#include <QtSerialPort/qserialport.h>
#include <QtSerialPort/qserialportinfo.h>
#include <QtCore/qloggingcategory.h>
#include <QSet>
#include "qiopipe_p.h"
#include <QSharedPointer>
#include "qnmeasatelliteinfosource_p.h"

Q_LOGGING_CATEGORY(lcSerial, "qt.positioning.serialnmea")

class IODeviceContainer
{
public:
    IODeviceContainer() {}
    IODeviceContainer(IODeviceContainer const&) = delete;
    void operator=(IODeviceContainer const&)  = delete;

    QSharedPointer<QIOPipe> serial(const QString &portName)
    {
        if (m_serialPorts.contains(portName)) {
            m_serialPorts[portName].refs++;
            QIOPipe *endPipe = new QIOPipe(m_serialPorts[portName].proxy);
            m_serialPorts[portName].proxy->addChildPipe(endPipe);
            return QSharedPointer<QIOPipe>(endPipe);
        }
        IODevice device;
        QSerialPort *port = new QSerialPort(portName);
        port->setBaudRate(4800);
        qCDebug(lcSerial) << "Opening serial port" << portName;
        if (!port->open(QIODevice::ReadOnly)) {
            qWarning("serialnmea: Failed to open %s", qPrintable(portName));
            delete port;
            return {};
        }
        qCDebug(lcSerial) << "Opened successfully";
        device.device = port;
        device.refs = 1;
        device.proxy = new QIOPipe(port, QIOPipe::ProxyPipe);
        m_serialPorts[portName] = device;
        QIOPipe *endPipe = new QIOPipe(device.proxy);
        device.proxy->addChildPipe(endPipe);
        return QSharedPointer<QIOPipe>(endPipe);
    }

    void releaseSerial(const QString &portName, QSharedPointer<QIOPipe> &pipe) {
        if (!m_serialPorts.contains(portName))
            return;

        pipe.clear(); // make sure to release the pipe returned by getSerial, or else, if there are still refs, data will be leaked through it
        IODevice &device = m_serialPorts[portName];
        if (device.refs > 1) {
            device.refs--;
            return;
        }

        IODevice taken = m_serialPorts.take(portName);
        taken.device->deleteLater();
    }

private:

    struct IODevice {
        QIODevice *device = nullptr;
        QIOPipe *proxy = nullptr;  // adding client pipes as children of proxy allows to dynamically add clients to one device.
        unsigned int refs = 1;
    };

    QMap<QString, IODevice> m_serialPorts;
};

Q_GLOBAL_STATIC(IODeviceContainer, deviceContainer)


class NmeaSource : public QNmeaPositionInfoSource
{
public:
    explicit NmeaSource(QObject *parent, const QVariantMap &parameters);
    ~NmeaSource() override;
    bool isValid() const { return !m_port.isNull(); }

private:
    QSharedPointer<QIOPipe> m_port;
    QString m_portName;
};

NmeaSource::NmeaSource(QObject *parent, const QVariantMap &parameters)
    : QNmeaPositionInfoSource(RealTimeMode, parent)
{
    QByteArray requestedPort;
    if (parameters.contains(QStringLiteral("serialnmea.serial_port")))
        requestedPort = parameters.value(QStringLiteral("serialnmea.serial_port")).toString().toLatin1();
    else
        requestedPort = qgetenv("QT_NMEA_SERIAL_PORT");
    QString portName;
    if (requestedPort.isEmpty()) {
        const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
        qCDebug(lcSerial) << "Found" << ports.count() << "serial ports";
        if (ports.isEmpty()) {
            qWarning("serialnmea: No serial ports found");
            return;
        }

        // Try to find a well-known device.
        QSet<int> supportedDevices;
        supportedDevices << 0x67b; // GlobalSat (BU-353S4 and probably others)
        supportedDevices << 0xe8d; // Qstarz MTK II
        for (const QSerialPortInfo& port : ports) {
            if (port.hasVendorIdentifier() && supportedDevices.contains(port.vendorIdentifier())) {
                portName = port.portName();
                break;
            }
        }

        if (portName.isEmpty()) {
            qWarning("serialnmea: No known GPS device found. Specify the COM port via QT_NMEA_SERIAL_PORT.");
            return;
        }
        m_portName = portName;
    } else {
        m_portName = QString::fromUtf8(requestedPort);
    }

    m_port = deviceContainer->serial(m_portName);
    if (!m_port)
        return;

    setDevice(m_port.data());
}

NmeaSource::~NmeaSource()
{
    deviceContainer->releaseSerial(m_portName, m_port);
}



class NmeaSatelliteSource : public QNmeaSatelliteInfoSource
{
public:
    NmeaSatelliteSource(QObject *parent, const QVariantMap &parameters)
        : QNmeaSatelliteInfoSource(parent)
    {
        QByteArray requestedPort;
        if (parameters.contains(QStringLiteral("serialnmea.serial_port")))
            requestedPort = parameters.value(QStringLiteral("serialnmea.serial_port")).toString().toLatin1();
        else
            requestedPort = qgetenv("QT_NMEA_SERIAL_PORT");
        QString portName;
        if (requestedPort.isEmpty()) {
            const QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
            qCDebug(lcSerial) << "Found" << ports.count() << "serial ports";
            if (ports.isEmpty()) {
                qWarning("serialnmea: No serial ports found");
                return;
            }

            // Try to find a well-known device.
            QSet<int> supportedDevices;
            supportedDevices << 0x67b; // GlobalSat (BU-353S4 and probably others)
            supportedDevices << 0xe8d; // Qstarz MTK II
            foreach (const QSerialPortInfo& port, ports) {
                if (port.hasVendorIdentifier() && supportedDevices.contains(port.vendorIdentifier())) {
                    portName = port.portName();
                    break;
                }
            }

            if (portName.isEmpty()) {
                qWarning("serialnmea: No known GPS device found. Specify the COM port via QT_NMEA_SERIAL_PORT.");
                return;
            }
            m_portName = portName;
        } else {
            m_portName = QString::fromUtf8(requestedPort);
        }

        m_port = deviceContainer->serial(m_portName);
        if (!m_port)
            return;

        setDevice(m_port.data());
    }

    ~NmeaSatelliteSource()
    {
        deviceContainer->releaseSerial(m_portName, m_port);
    }

    bool isValid() const { return !m_port.isNull(); }

private:
    QSharedPointer<QIOPipe> m_port;
    QString m_portName;
};

QGeoPositionInfoSource *QGeoPositionInfoSourceFactorySerialNmea::positionInfoSource(QObject *parent)
{
    return positionInfoSourceWithParameters(parent, QVariantMap());
}

QGeoSatelliteInfoSource *QGeoPositionInfoSourceFactorySerialNmea::satelliteInfoSource(QObject *parent)
{
    return satelliteInfoSourceWithParameters(parent, QVariantMap());
}

QGeoAreaMonitorSource *QGeoPositionInfoSourceFactorySerialNmea::areaMonitor(QObject *parent)
{
    return areaMonitorWithParameters(parent, QVariantMap());
}

QGeoPositionInfoSource *QGeoPositionInfoSourceFactorySerialNmea::positionInfoSourceWithParameters(QObject *parent, const QVariantMap &parameters)
{
    QScopedPointer<NmeaSource> src(new NmeaSource(parent, parameters));
    return src->isValid() ? src.take() : nullptr;
}

QGeoSatelliteInfoSource *QGeoPositionInfoSourceFactorySerialNmea::satelliteInfoSourceWithParameters(QObject *parent, const QVariantMap &parameters)
{
    QScopedPointer<NmeaSatelliteSource> src(new NmeaSatelliteSource(parent, parameters));
    return src->isValid() ? src.take() : nullptr;
}

QGeoAreaMonitorSource *QGeoPositionInfoSourceFactorySerialNmea::areaMonitorWithParameters(QObject *parent, const QVariantMap &parameters)
{
    Q_UNUSED(parent);
    Q_UNUSED(parameters)
    return nullptr;
}
