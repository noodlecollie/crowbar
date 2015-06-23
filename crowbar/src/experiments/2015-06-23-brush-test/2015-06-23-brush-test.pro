#-------------------------------------------------
#
# Project created by QtCreator 2015-06-23T07:16:43
#
#-------------------------------------------------

QT       += core gui 3dcore 3drenderer 3dinput

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2015-06-23-brush-test
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=

FORMS    +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../modules/model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../modules/model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../../modules/model/ -lmodel

INCLUDEPATH += $$PWD/../../modules/model
DEPENDPATH += $$PWD/../../modules/model
