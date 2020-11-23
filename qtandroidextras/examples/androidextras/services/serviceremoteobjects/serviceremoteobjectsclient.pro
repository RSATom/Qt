QT += qml quick androidextras remoteobjects

CONFIG += c++11

SOURCES += main.cpp

include(../common/common.pri)

REPC_REPLICA += ../common/qtandroidservice.rep

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/services/serviceremoteobjects
INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
    android/AndroidManifest.xml \
    android/src/org/qtproject/example/qtandroidservice/QtAndroidService.java
