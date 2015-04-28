#-------------------------------------------------
#
# Project created by QtCreator 2015-04-28T17:06:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = application
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../modules/model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../modules/model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../modules/model/ -lmodel

INCLUDEPATH += $$PWD/../modules/model
DEPENDPATH += $$PWD/../modules/model
