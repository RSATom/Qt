TEMPLATE = app
QT = core gui widgets axcontainer
CONFIG += console c++11

SOURCES += main.cpp
include(../shared/shared.pri)

DIAGLIB = ../../../../qtbase/tests/manual/diaglib
exists($$DIAGLIB):include($$DIAGLIB/diaglib.pri)
