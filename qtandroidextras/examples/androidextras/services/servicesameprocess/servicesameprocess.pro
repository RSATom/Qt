QT += qml quick androidextras

CONFIG += c++11

HEADERS += \
    qtandroidservice.h

SOURCES += \
    main.cpp \
    qtandroidservice.cpp

include(../common/common.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/services/servicesameprocess
INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
    android/src/org/qtproject/example/qtandroidservice/QtAndroidService.java \
    android/AndroidManifest.xml
