TARGET = qttexttospeech_flite
QT = core multimedia texttospeech
QT_FOR_CONFIG += texttospeech-private

PLUGIN_TYPE = texttospeech
PLUGIN_CLASS_NAME = QTextToSpeechEngineFlite
load(qt_plugin)

include(../common/common.pri)

HEADERS += \
    qtexttospeech_flite.h \
    qtexttospeech_flite_plugin.h \
    qtexttospeech_flite_processor.h

SOURCES += \
    qtexttospeech_flite.cpp \
    qtexttospeech_flite_plugin.cpp \
    qtexttospeech_flite_processor.cpp

OTHER_FILES += \
    flite_plugin.json

QMAKE_USE_PRIVATE += flite
qtConfig(flite_alsa): QMAKE_USE_PRIVATE += flite_alsa
