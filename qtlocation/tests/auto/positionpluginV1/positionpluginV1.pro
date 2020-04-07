TARGET = qtposition_testpluginv1
QT += positioning

PLUGIN_TYPE = position
PLUGIN_CLASS_NAME = QGeoPositionInfoSourceFactoryTestV1
PLUGIN_EXTENDS = -
load(qt_plugin)

SOURCES += plugin.cpp

OTHER_FILES += \
    plugin.json
