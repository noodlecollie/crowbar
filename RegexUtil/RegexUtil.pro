#-------------------------------------------------
#
# Project created by QtCreator 2013-12-20T08:00:00
#
#-------------------------------------------------

QT       -= gui

TARGET = RegexUtil
TEMPLATE = lib

DEFINES += REGEXUTIL_LIBRARY

SOURCES += \
    regexutil.cpp

HEADERS +=\
        regexutil_global.h \
    regexutil.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

CONFIG(debug) {
    win32 {
        TARGET = RegexUtild
    }
    mac {
        TARGET = RegexUtil_debug
    }
} else {
    TARGET = RegexUtil
}
