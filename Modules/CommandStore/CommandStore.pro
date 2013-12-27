#-------------------------------------------------
#
# Project created by QtCreator 2013-12-22T13:01:23
#
#-------------------------------------------------

QT       -= gui

#CONFIG(debug) {
#    win32 {
#        TARGET = CommandStored
#    }
#    mac {
#        TARGET = CommandStore_debug
#    }
#} else {
#    TARGET = CommandStore
#}

TARGET = CommandStore

TEMPLATE = lib

DEFINES += COMMANDSTORE_LIBRARY

SOURCES += \
    src/commandstore.cpp \
    src/commandmanager.cpp

HEADERS += \
    inc/commandstore_global.h \
    inc/commandstore.h \
    inc/commandstore_manager.h

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
    ../IConsole/Inc

#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#    mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
#    win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
#}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../IConsole/release/ -lIConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../IConsole/debug/ -lIConsole
else:unix: LIBS += -L$$OUT_PWD/../IConsole/ -lIConsole

INCLUDEPATH += $$PWD/../IConsole/inc
DEPENDPATH += $$PWD/../IConsole/inc
