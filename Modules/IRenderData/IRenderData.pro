#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T23:37:35
#
#-------------------------------------------------

QT       -= gui
Qt       += 3d opengl

TARGET = IRenderData
TEMPLATE = lib

DEFINES += IRENDERDATA_LIBRARY

SOURCES += \
    src/irenderdata.cpp

HEADERS += \
    inc/irenderdata_global.h \
    inc/irenderdata.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../qt3d/lib -lQt53D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../qt3d/lib -lQt53Dd
else:unix: LIBS += -L$$OUT_PWD/../../qt3d/src/threed/ -lQt53Dd

INCLUDEPATH += $$PWD/../../qt3d/include
DEPENDPATH += $$PWD/../../qt3d/include
