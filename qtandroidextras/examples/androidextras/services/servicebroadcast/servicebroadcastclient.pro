QT += qml quick androidextras

CONFIG += c++11

SOURCES += main.cpp

include(../common/common_broadcast.pri)
include(../common/common.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/services/servicebroadcast
INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
    android/src/org/qtproject/example/qtandroidservice/QtAndroidService.java \
    android/src/org/qtproject/example/qtandroidservice/ActivityUtils.java \
    android/AndroidManifest.xml
