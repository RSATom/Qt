CXX_MODULE = qtlottie
TARGET  = lottieqtplugin
TARGETPATH = Qt/labs/lottieqt
IMPORT_VERSION = 1.0

QT += qml quick gui-private bodymovin-private

QMAKE_DOCS = $$PWD/doc/qtlottieanimation.qdocconf

# Input
SOURCES += \
    lottieanimation.cpp \
    lottie_plugin.cpp \
    rasterrenderer/lottierasterrenderer.cpp \
    rasterrenderer/batchrenderer.cpp

HEADERS += \
    lottieanimation.h \
    lottie_plugin.h \
    rasterrenderer/lottierasterrenderer.h \
    rasterrenderer/batchrenderer.h

OTHER_FILES += \
    qmldir

load(qml_plugin)
