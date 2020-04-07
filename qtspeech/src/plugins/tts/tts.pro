TEMPLATE = subdirs

QT_FOR_CONFIG += texttospeech-private
unix {
    qtConfig(speechd) {
        SUBDIRS += speechdispatcher
    }
}

windows:!winrt: SUBDIRS += sapi
winrt: SUBDIRS += winrt

osx: SUBDIRS += osx
uikit: SUBDIRS += ios

android: SUBDIRS += android

qtConfig(flite) {
    qtHaveModule(multimedia): SUBDIRS += flite
}
