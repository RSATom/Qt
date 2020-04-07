TARGET = qtquicktimelineplugin
TARGETPATH = QtQuick/Timeline
IMPORT_VERSION = 1.0

QT += qml quick core-private

QT_PRIVATE += core-private gui-private qml-private quick-private

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

include(timeline.pri)

DISTFILES += \
    qmldir \
    plugins.qmltypes

SOURCES += \
    $$PWD/qtquicktimelineplugin.cpp

CONFIG += no_cxx_module

QMAKE_DOCS = $$PWD/doc/qtquicktimeline.qdocconf

load(qml_plugin)
