TEMPLATE = lib
TARGET = service
CONFIG += dll
QT += core androidextras remoteobjects

SOURCES += service_main.cpp

include(../common/common_ro.pri)

REPC_SOURCE += ../common/qtandroidservice.rep

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/services/serviceremoteobjects
INSTALLS += target
