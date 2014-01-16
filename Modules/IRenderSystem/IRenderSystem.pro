#-------------------------------------------------
#
# Project created by QtCreator 2014-01-05T20:46:07
#
#-------------------------------------------------

QT       += opengl 3d

QT       -= gui

TARGET = IRenderSystem
TEMPLATE = lib

DEFINES += IRENDERSYSTEM_LIBRARY

SOURCES += \
    src/dummy.cpp

HEADERS += \
    inc/irendersystem_global.h \
    inc/irenderable.h \
    inc/dummy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += \
    inc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53Dd
else:unix: LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53D

INCLUDEPATH += $$PWD/../../../qt3d/include
DEPENDPATH += $$PWD/../../../qt3d/include
