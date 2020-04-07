TARGET = qtposition_testplugin
QT += positioning-private

PLUGIN_TYPE = position
PLUGIN_CLASS_NAME = QGeoPositionInfoSourceFactoryTest
PLUGIN_EXTENDS = -
load(qt_plugin)

SOURCES += plugin.cpp

OTHER_FILES += \
    plugin.json
