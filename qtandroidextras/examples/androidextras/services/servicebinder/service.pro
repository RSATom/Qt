TEMPLATE = lib
TARGET = service
CONFIG += dll
QT += core androidextras

HEADERS += androidbinder.h

SOURCES += \
    service_main.cpp \
    androidbinder.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/services/servicebinder
INSTALLS += target
