#-------------------------------------------------
#
# Project created by QtCreator 2014-04-03T20:25:40
#
#-------------------------------------------------

QT       -= gui

TARGET = GlobalUtil
TEMPLATE = lib

DEFINES += GLOBALUTIL_LIBRARY

SOURCES += \
    src/globals.cpp

HEADERS +=\
    inc/globals.h \
    inc/globalutil_global.h \
    inc/interface.h \
    inc/classutil.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += inc
