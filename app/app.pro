QT  += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crowbar
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwin.cpp \
    commandlineparser.cpp \
    globals.cpp \
    openglwidget.cpp \
    octree/octree.tcc \
    mapdoc.cpp \
    face.cpp \
    plane.cpp \
    solid.cpp \
    indexpool.cpp \
    edge.cpp \
    consolewindow.cpp

HEADERS += \
    mainwin.h \
    commandlineparser.h \
    globals.h \
    openglwidget.h \
    octree/tinyvector.h \
    octree/shareddata.h \
    octree/point3d.h \
    octree/octree.h \
    octree/numtraits.h \
    octree/array2d.h \
    octree/array.h \
    mapdoc.h \
    vertex.h \
    edge.h \
    face.h \
    plane.h \
    matlib.h \
    ivertex3drenderspec.h \
    solid.h \
    indexpool.h \
    consolewindow.h

# LIBS is from the main build directory (where debug/release folders are located)
win32 {
    # Windows build
    LIBS    += -L../IConsole/debug/ -lIConsole
}
unix {
    # Unix build - for some reason the -L/-l syntax doesn't work...
    LIBS    += ../IConsole/libIConsole.so
}

# Extra includes for libraries.
INCLUDEPATH += ../IConsole/inc

if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
   mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
   win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
}

# NOTE: These paths assume your build directory is in the same directory as the root Crowbar folder (within which is Crowbar.pro).
# For VS at least, the folder hierarchy seems to be <build directory>/app/ to get to the Makefile.

# For Windows:
RC_FILE = crowbar.rc

# For OS/X:
ICON = crowbar.icns
