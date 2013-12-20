#-------------------------------------------------
#
# Project created by QtCreator 2013-12-20T20:43:20
#
#-------------------------------------------------

QT       -= gui

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

TEMPLATE = lib

DEFINES += REGEXUTIL_LIBRARY

SOURCES += \
    src/regexutil.cpp

HEADERS += \
    inc/regexutil_global.h \
    inc/regexutil.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

INCLUDEPATH += \
    inc
