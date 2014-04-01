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
    src/treenode.cpp \
    inc/worldculltreenode.tcc

HEADERS +=\
    inc/octree_global.h \
    inc/itreenode.h \
    inc/treenode.h \
    inc/worldculltreenode.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    License.txt

INCLUDEPATH += \
    inc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../qt3d/lib -lQt53D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../qt3d/lib -lQt53Dd
else:unix: LIBS += -L$$OUT_PWD/../../../qt3d/src/threed/ -lQt53Dd

INCLUDEPATH += $$PWD/../../../qt3d/include
DEPENDPATH += $$PWD/../../../qt3d/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Geometry/release/ -lGeometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Geometry/debug/ -lGeometry
else:unix: LIBS += -L$$OUT_PWD/../Geometry/ -lGeometry

INCLUDEPATH += $$PWD/../Geometry/inc
DEPENDPATH += $$PWD/../Geometry/inc
