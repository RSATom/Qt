QT += quick androidextras

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

HEADERS += \
    activityhandler.h

SOURCES += \
        activityhandler.cpp \
        main.cpp

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/customactivity
INSTALLS += target

android {
    DISTFILES += \
        android/AndroidManifest.xml \
        android/src/org/qtproject/example/activityhandler/CustomActivity.java \
        android/res/layout/second_activity.xml \
        android/res/values/strings.xml

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
