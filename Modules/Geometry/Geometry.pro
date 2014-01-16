#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T08:20:33
#
#-------------------------------------------------

#QT       -= gui
QT        += gui 3d    # Apparently GUI is needed for QVector3D??

TARGET = Geometry
TEMPLATE = lib

DEFINES += GEOMETRY_LIBRARY

SOURCES += \
    src/geommetahandle.cpp \
    src/geomutil.cpp \
    src/brushvertex.cpp \
    src/renderbox.cpp

HEADERS += \
    inc/geommetahandle.h \
    inc/geometry_global.h \
    inc/geomutil.h \
    inc/ibasictransform.h \
    inc/ibrushops.h \
    inc/brushvertex.h \
    inc/renderbox.h \
    inc/igeomdataexport.h

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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../qt3d/lib -lQt53D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../qt3d/lib -lQt53Dd
else:unix: LIBS += -L$$OUT_PWD/../../../qt3d/src/threed/ -lQt53Dd

INCLUDEPATH += $$PWD/../../../qt3d/include
DEPENDPATH += $$PWD/../../../qt3d/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../IRenderSystem/release/ -lIRenderSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../IRenderSystem/debug/ -lIRenderSystem
else:unix: LIBS += -L$$OUT_PWD/../IRenderSystem/ -lIRenderSystem

INCLUDEPATH += $$PWD/../IRenderSystem/inc
DEPENDPATH += $$PWD/../IRenderSystem/inc
