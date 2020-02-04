TEMPLATE = app
CONFIG += testcase
TARGET = tst_qgeojson

SOURCES += \
    tst_qgeojson.cpp

QT += positioning testlib location
QT += location-private
