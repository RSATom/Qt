TEMPLATE=app
CONFIG += QMAKE_LFLAGS_CONSOLE
QT += widgets axcontainer testlib

SOURCES += main.cpp
include(../shared/shared.pri)

# Assume Web Browser type library is available in all windows installations
TYPELIBS = $$(SystemRoot)\\system32\\ieframe.dll

!exists($$TYPELIBS) {
    message("Web Browser type library for test not found!")
}
