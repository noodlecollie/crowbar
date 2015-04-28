#-------------------------------------------------
#
# Project created by QtCreator 2014-11-06T10:47:02
#
#-------------------------------------------------

QT       += core gui 3d

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crowbar
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mapdocumentcollection.cpp \
    applicationdata.cpp \
    brushlistdialogue.cpp \
    transformdialogue.cpp

HEADERS  += mainwindow.h \
    mapdocumentcollection.h \
    applicationdata.h \
    brushlistdialogue.h \
    transformdialogue.h

FORMS    += mainwindow.ui \
    brushlistdialogue.ui \
    transformdialogue.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../view/release/ -lview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../view/debug/ -lview
else:unix: LIBS += -L$$OUT_PWD/../view/ -lview

INCLUDEPATH += $$PWD/../view
DEPENDPATH += $$PWD/../view

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tools/release/ -ltools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tools/debug/ -ltools
else:unix: LIBS += -L$$OUT_PWD/../tools/ -ltools

INCLUDEPATH += $$PWD/../tools
DEPENDPATH += $$PWD/../tools
