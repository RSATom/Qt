TEMPLATE = lib
TARGET = service
CONFIG += dll
QT += core androidextras

SOURCES += \
    service_main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/services/servicebroadcast
INSTALLS += target
