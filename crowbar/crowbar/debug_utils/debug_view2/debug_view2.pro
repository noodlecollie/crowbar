#-------------------------------------------------
#
# Project created by QtCreator 2014-11-20T10:51:17
#
#-------------------------------------------------

QT       += core gui 3d

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = debug_view2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../view/release/ -lview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../view/debug/ -lview
else:unix:!macx: LIBS += -L$$OUT_PWD/../../view/ -lview
macx: LIBS += -L$$OUT_PWD/../../view/ -lview

INCLUDEPATH += $$PWD/../../view
DEPENDPATH += $$PWD/../../view

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../model/debug/ -lmodel
else:unix:!macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel
macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../tools/release/ -ltools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../tools/debug/ -ltools
else:unix:!macx: LIBS += -L$$OUT_PWD/../../tools/ -ltools
macx: LIBS += -L$$OUT_PWD/../../tools/ -ltools

INCLUDEPATH += $$PWD/../../tools
DEPENDPATH += $$PWD/../../tools
