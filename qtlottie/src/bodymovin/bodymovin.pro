#-------------------------------------------------
#
# Project created by QtCreator 2018-09-28T16:59:29
#
#-------------------------------------------------

TARGET = QtBodymovin
MODULE = bodymovin

CONFIG += internal_module
# As long as this is an internal module, we do not need to have cmake linkage tests.
CMAKE_MODULE_TESTS = -

QT += gui-private

DEFINES += BODYMOVIN_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bmbase.cpp \
    bmlayer.cpp \
    bmshape.cpp \
    bmshapelayer.cpp \
    bmrect.cpp \
    bmfill.cpp \
    bmgfill.cpp \
    bmgroup.cpp \
    bmstroke.cpp \
    bmbasictransform.cpp \
    bmshapetransform.cpp \
    bmellipse.cpp \
    bmround.cpp \
    bmfreeformshape.cpp \
    bmtrimpath.cpp \
    bmpathtrimmer.cpp \
    lottierenderer.cpp \
    trimpath.cpp \
    bmfilleffect.cpp \
    bmrepeater.cpp \
    bmrepeatertransform.cpp \
    beziereasing.cpp

HEADERS += \
    beziereasing_p.h \
    bmbase_p.h \
    bmbasictransform_p.h \
    bmconstants_p.h \
    bmellipse_p.h \
    bmfill_p.h \
    bmfilleffect_p.h \
    bmfreeformshape_p.h \
    bmgfill_p.h \
    bmgroup_p.h \
    bmlayer_p.h \
    bmproperty_p.h \
    bmrect_p.h \
    bmrepeater_p.h \
    bmrepeatertransform_p.h \
    bmround_p.h \
    bmshape_p.h \
    bmshapelayer_p.h \
    bmshapetransform_p.h \
    bmspatialproperty_p.h \
    bmstroke_p.h \
    bmtrimpath_p.h \
    trimpath_p.h \
    lottierenderer_p.h \
    bmpathtrimmer_p.h \
    bmglobal.h

load(qt_module)
