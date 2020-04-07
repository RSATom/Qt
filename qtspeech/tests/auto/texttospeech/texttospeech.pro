CONFIG += testcase
TARGET = tst_qtexttospeech
QT = testlib core texttospeech-private
SOURCES += tst_qtexttospeech.cpp

qtConfig(speechd): QMAKE_USE += speechd
