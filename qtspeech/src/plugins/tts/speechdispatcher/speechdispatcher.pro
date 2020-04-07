TARGET = qtexttospeech_speechd
PLUGIN_TYPE = texttospeech
PLUGIN_CLASS_NAME = QTextToSpeechPluginSpeechd

load(qt_plugin)

QT = core texttospeech
QT_FOR_CONFIG += texttospeech-private

HEADERS += \
    qtexttospeech_speechd.h \
    qtexttospeech_speechd_plugin.h \

SOURCES += \
    qtexttospeech_speechd.cpp \
    qtexttospeech_speechd_plugin.cpp \

OTHER_FILES += \
    speechd_plugin.json

qtConfig(speechd): QMAKE_USE_PRIVATE += speechd
