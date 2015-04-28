#-------------------------------------------------
#
# Project created by QtCreator 2015-01-06T13:46:26
#
#-------------------------------------------------

QT       += testlib 3d

TARGET = tst_new_component_tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_new_component_tests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../model/debug/ -lmodel
else:unix:!macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel
macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model
