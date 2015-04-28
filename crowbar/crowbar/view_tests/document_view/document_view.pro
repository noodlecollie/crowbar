#-------------------------------------------------
#
# Project created by QtCreator 2015-01-08T13:15:40
#
#-------------------------------------------------

QT       += testlib 3d

TARGET = tst_document_view_test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_document_view_test.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../model/debug/ -lmodel
else:unix:!macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel
macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../view/release/ -lview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../view/debug/ -lview
else:unix:!macx: LIBS += -L$$OUT_PWD/../../view/ -lview
macx: LIBS += -L$$OUT_PWD/../../view/ -lview

INCLUDEPATH += $$PWD/../../view
DEPENDPATH += $$PWD/../../view
