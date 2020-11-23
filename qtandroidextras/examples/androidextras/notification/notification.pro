QT += quick androidextras

SOURCES += \
    main.cpp \
    notificationclient.cpp

RESOURCES += \
    main.qrc

HEADERS += \
    notificationclient.h

target.path = $$[QT_INSTALL_EXAMPLES]/androidextras/notification
INSTALLS += target

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
OTHER_FILES += \
    android/src/org/qtproject/example/notification/NotificationClient.java \
    android/AndroidManifest.xml
