TEMPLATE = app
QT       += remoteobjects core testlib
QT       -= gui

TARGET = restart_client
DESTDIR = ./
CONFIG   += c++11
CONFIG   -= app_bundle

REPC_REPLICA = ../subclass.rep

SOURCES += main.cpp \

HEADERS += \

INCLUDEPATH += $$PWD
