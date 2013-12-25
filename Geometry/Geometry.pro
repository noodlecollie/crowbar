#-------------------------------------------------
#
# Project created by QtCreator 2013-12-23T08:20:33
#
#-------------------------------------------------

#QT       -= gui
Qt        += gui    # Apparently GUI is needed for QVector3D??

TARGET = Geometry
TEMPLATE = lib

DEFINES += GEOMETRY_LIBRARY

SOURCES += \
    src/geommetahandle.cpp \
    src/vertex3d.cpp \
    src/geomutil.cpp \
    src/edge3d.cpp \
    src/polygon3d.cpp \
    src/plane3d.cpp

HEADERS += \
    inc/geommetahandle.h \
    inc/geometry_global.h \
    inc/vertex3d.h \
    inc/geomutil.h \
    inc/edge3d.h \
    inc/polygon3d.h \
    inc/plane3d.h

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
