QT += quick androidextras

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    musiclist.h

SOURCES += \
        main.cpp \
        musiclist.cpp

RESOURCES += qml.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/musiclist
INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DISTFILES += \
    android/AndroidManifest.xml \
    android/src/org/qtproject/example/musiclist/MusicList.java
