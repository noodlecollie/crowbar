#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T14:27:47
#
#-------------------------------------------------

#QT       -= gui
QT      += 3d

TARGET = Octree
TEMPLATE = lib

DEFINES += OCTREE_LIBRARY

SOURCES += \
    inc/octree.tcc \
    src/dummy.cpp \
    inc/worldculltree.tcc

HEADERS +=\
    inc/array.h \
    inc/array2d.h \
    inc/numtraits.h \
    inc/octree.h \
    inc/point3d.h \
    inc/shareddata.h \
    inc/tinyvector.h \
    inc/octree_global.h \
    inc/dummy.h \
    inc/worldculltree.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    License.txt

INCLUDEPATH += \
    inc \
    $$PWD/../Geometry/inc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../qt3d/lib -lQt53D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../qt3d/lib -lQt53Dd
else:unix: LIBS += -L$$OUT_PWD/../../../qt3d/src/threed/ -lQt53Dd

INCLUDEPATH += $$PWD/../../../qt3d/include
DEPENDPATH += $$PWD/../../../qt3d/include
