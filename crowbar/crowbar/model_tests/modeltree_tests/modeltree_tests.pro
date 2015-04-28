#-------------------------------------------------
#
# Project created by QtCreator 2015-02-10T15:26:32
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_tst_modeltree
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_tst_modeltree.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model
