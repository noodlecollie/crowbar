#-------------------------------------------------
#
# Project created by QtCreator 2013-12-20T23:29:52
#
#-------------------------------------------------

QT       -= gui

CONFIG(debug) {
    win32 {
        TARGET = CommandStored
    }
    mac {
        TARGET = CommandStore_debug
    }
} else {
    TARGET = CommandStore
}

TEMPLATE = lib

DEFINES += COMMANDSTORE_LIBRARY

SOURCES += \
    src/commandstore.cpp

HEADERS += \
    inc/commandstore_global.h \
    inc/commandstore.h

# LIBS is from the main build directory (where debug/release folders are located)
win32 {
    # Windows build
    # Should change this for release mode!
    LIBS    += -L../IConsole/debug/ -lIConsole
}
unix {
    # Unix build - for some reason the -L/-l syntax doesn't work...
    LIBS    += ../IConsole/libIConsole.so
}

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
    mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
    win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

INCLUDEPATH += \
    inc \
    ..\IConsole\inc
