CXX_MODULE = qml
TARGET = qtremoteobjects
TARGETPATH = QtRemoteObjects
IMPORT_VERSION = 5.$$QT_MINOR_VERSION

QT += qml remoteobjects

SOURCES = \
    $$PWD/plugin.cpp \

HEADERS = \

load(qml_plugin)
