TARGETPATH = QtNfc

QT = core qml nfc

# Input
HEADERS += \
    qdeclarativenearfield_p.h \
    qdeclarativendeffilter_p.h \
    qdeclarativendeftextrecord_p.h \
    qdeclarativendefurirecord_p.h \
    qdeclarativendefmimerecord_p.h

SOURCES += plugin.cpp \
    qdeclarativenearfield.cpp \
    qdeclarativendeffilter.cpp \
    qdeclarativendeftextrecord.cpp \
    qdeclarativendefurirecord.cpp \
    qdeclarativendefmimerecord.cpp

load(qml_plugin)
