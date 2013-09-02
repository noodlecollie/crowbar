#-------------------------------------------------
#
# Project created by QtCreator 2013-08-27T07:31:53
#
#-------------------------------------------------

QT       += widgets

TARGET = IConsole
TEMPLATE = lib

DEFINES += ICONSOLE_LIBRARY

SOURCES += \
    consolewindow.cpp \
    completionlist.cpp

HEADERS +=\
        iconsole_global.h \
    consolewindow.h \
    completionlist.h \
    interfaces/iconsolewindow.h \
    interfaces/icompletionwidget.h \
    interfaces/icommandmanager.h \
    interfaces/icommandbox.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
