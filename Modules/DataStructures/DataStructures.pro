#-------------------------------------------------
#
# Project created by QtCreator 2014-04-04T17:14:08
#
#-------------------------------------------------

#QT       -= gui
QT        += gui 3d    # Apparently GUI is needed for QVector3D??

TARGET = DataStructures
TEMPLATE = lib

DEFINES += DATASTRUCTURES_LIBRARY

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../IRenderSystem/release/ -lIRenderSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../IRenderSystem/debug/ -lIRenderSystem
else:unix: LIBS += -L$$OUT_PWD/../IRenderSystem/ -lIRenderSystem

INCLUDEPATH += $$PWD/../IRenderSystem/inc
DEPENDPATH += $$PWD/../IRenderSystem/inc

HEADERS += \
    inc/datastructures_global.h \
    inc/itreenode.h \
    inc/treenode.h \
    inc/worldculltreenode.h \
    inc/keyvaluesnode.h

SOURCES += \
    inc/worldculltreenode.tcc \
    src/treenode.cpp \
    src/keyvaluesnode.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Geometry/release/ -lGeometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Geometry/debug/ -lGeometry
else:unix: LIBS += -L$$OUT_PWD/../Geometry/ -lGeometry

INCLUDEPATH += $$PWD/../Geometry/inc
DEPENDPATH += $$PWD/../Geometry/inc
