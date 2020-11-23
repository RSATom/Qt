TEMPLATE = app
TARGET = tst_qmlintegration
CONFIG += qmltestcase

SOURCES += \
    $$PWD/tst_integration.cpp

OTHER_FILES += \
    $$PWD/data/*.qml

TESTDATA += \
    $$PWD/data/tst_*
