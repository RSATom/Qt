QT += quick androidextras

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    jnimessenger.cpp \
    main.cpp

HEADERS += \
    jnimessenger.h

RESOURCES += qml.qrc

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
DISTFILES += \
    android/src/org/qtproject/example/jnimessenger/JniMessenger.java \
    android/AndroidManifest.xml


target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/jnimessenger
INSTALLS += target
