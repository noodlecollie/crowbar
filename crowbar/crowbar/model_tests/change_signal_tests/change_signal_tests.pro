#-------------------------------------------------
#
# Project created by QtCreator 2014-11-03T14:29:38
#
#-------------------------------------------------

QT       += testlib

QT       += gui 3d

TARGET = tst_change_signal_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_change_signal_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../../model/ -lmodel
macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model
