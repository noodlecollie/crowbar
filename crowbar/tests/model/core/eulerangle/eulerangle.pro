#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T09:55:26
#
#-------------------------------------------------

QT       += testlib gui

TARGET = tst_tst_eulerangle
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_tst_eulerangle.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../../src/modules/model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../../src/modules/model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../../../../src/modules/model/ -lmodel

INCLUDEPATH += $$PWD/../../../../src/modules/model
DEPENDPATH += $$PWD/../../../../src/modules/model
