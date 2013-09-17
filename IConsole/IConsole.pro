#-------------------------------------------------
#
# Project created by QtCreator 2013-08-27T07:31:53
#
#-------------------------------------------------

QT       += widgets

TARGET = IConsole
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
    inc/wr_listedconsolecommand.h

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
    src/convar.cpp

INCLUDEPATH += \
    inc
