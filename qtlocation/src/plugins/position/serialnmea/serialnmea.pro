TARGET = qtposition_serialnmea

QT = core-private positioning-private serialport

HEADERS += \
    qgeopositioninfosourcefactory_serialnmea.h qnmeasatelliteinfosource_p.h qiopipe_p.h

SOURCES += \
    qgeopositioninfosourcefactory_serialnmea.cpp qnmeasatelliteinfosource.cpp qiopipe.cpp

OTHER_FILES += \
    plugin.json

PLUGIN_TYPE = position
PLUGIN_CLASS_NAME = QGeoPositionInfoSourceFactorySerialNmea
load(qt_plugin)
