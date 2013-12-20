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

# Don't actually need this yet because RegexUtil does not actually export anything.
# If we do export things in the future, uncomment (and correct!) this bit.
#win32 {
#    # Windows build
#    # Should change this for release mode!
#    LIBS    += -L../RegexUtil/debug/ -lRegexUtil
#}
#unix {
#    # Unix build - for some reason the -L/-l syntax doesn't work...
#    LIBS    += ../RegexUtil/libRegexUtil.so
#    
#    # Stick this on the linker command line to allow loading libraries from the folder the application is in.
#    #QMAKE_RPATHDIR += /media/Ext4Drive/crowbar/build-Crowbar-Desktop-Debug/IConsole  # This is obviously only for testing, remove it
#}

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
    inc/commandsenderinfo.h \
    inc/nregexutil.h

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
    src/commandsenderinfo.cpp \
    src/nregexutil.cpp

INCLUDEPATH += \
    inc \
    ..\RegexUtil\inc

# Commented for the same reason as above (no libraries to depend on).
#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#    mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
#    win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
#}
