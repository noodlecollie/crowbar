#-------------------------------------------------
#
# Project created by QtCreator 2014-01-05T20:46:07
#
#-------------------------------------------------

QT       += opengl

QT       -= gui

TARGET = IRenderSystem
TEMPLATE = lib

DEFINES += IRENDERSYSTEM_LIBRARY

SOURCES += \
    dummy.cpp

HEADERS += \
    inc/irendersystem_global.h \
    dummy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
