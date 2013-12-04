#-------------------------------------------------
#
# Project created by QtCreator 2013-08-27T07:31:53
#
#-------------------------------------------------

QT       += widgets

#TARGET = IConsole

CONFIG(debug) {
    win32 {
        TARGET = IConsoled
    }
    mac {
        TARGET = IConsole_debug
    }
} else {
    TARGET = IConsole
}

TEMPLATE = lib

DEFINES += ICONSOLE_LIBRARY

HEADERS += \
    inc/nglobalcmd.h \
    inc/iconsole_global.h \
    inc/baseconsolecommand.h \
    inc/listedconsolecommand.h \
    inc/commandmanager.h \
    inc/concommand.h \
    inc/convar.h \
    inc/wr_commandmanager.h \
    inc/wr_concommand.h \
    inc/wr_convar.h \
    inc/wr_listedconsolecommand.h \
    inc/listedcommandmanager.h \
    inc/wr_listedcommandmanager.h \
    inc/consolewidget.h \
    inc/commandinterpreter.h \
    inc/wr_commandinterpreter.h \
    inc/commandentrybox.h \
    inc/commandsuggestionlist.h \
    inc/wr_commandentrybox.h \
    inc/commandsenderinfo.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES +=

SOURCES += \  
    src/baseconsolecommand.cpp \
    src/listedconsolecommand.cpp \
    src/commandmanager.cpp \
    src/concommand.cpp \
    src/convar.cpp \
    src/listedcommandmanager.cpp \
    src/consolewidget.cpp \
    src/commandinterpreter.cpp \
    src/commandentrybox.cpp \
    src/commandsuggestionlist.cpp \
    src/commandsenderinfo.cpp

win32 {
    # Windows build
    # Should change this for release mode!
    LIBS    += -L../RegexUtil/debug/ -lRegexUtil
}
unix {
    # Unix build - for some reason the -L/-l syntax doesn't work...
    LIBS    += ../RegexUtil/libRegexUtil.so
}

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
    mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
    win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

message($$LIBS)

INCLUDEPATH += \
    inc \
    ../RegexUtil/inc
