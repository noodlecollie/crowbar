#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T14:27:47
#
#-------------------------------------------------

#QT       -= gui

TARGET = Octree
TEMPLATE = lib

DEFINES += OCTREE_LIBRARY

SOURCES += \
    inc/octree.tcc \
    src/dummy.cpp

HEADERS +=\
    inc/array.h \
    inc/array2d.h \
    inc/numtraits.h \
    inc/octree.h \
    inc/point3d.h \
    inc/shareddata.h \
    inc/tinyvector.h \
    inc/octree_global.h \
    inc/dummy.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    License.txt

INCLUDEPATH += \
    inc
