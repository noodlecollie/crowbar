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
    consolewindow.cpp \
    console_util.cpp

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
    # Should change this for release mode!
    LIBS    += -L../IConsole/debug/ -lIConsole
}
unix {
    # Unix build - for some reason the -L/-l syntax doesn't work...
    LIBS    += ../IConsole/libIConsole.so
    
    # Stick this on the linker command line to allow loading libraries from the folder the application is in.
    #QMAKE_RPATHDIR += /media/Ext4Drive/crowbar/build-Crowbar-Desktop-Debug/IConsole  # This is obviously only for testing, remove it
}

# Extra includes for libraries.
INCLUDEPATH += ../IConsole/inc

CONF_OUT=release
if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
    mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
    win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
    CONF_OUT=debug
}

# NOTE: These paths assume your build directory is in the same directory as the root Crowbar folder (within which is Crowbar.pro).
# For VS at least, the folder hierarchy seems to be <build directory>/app/ to get to the Makefile.

# For Windows:
RC_FILE = crowbar.rc

# For OS/X:
ICON = crowbar.icns

# Variable to hold path to output folder
ORIG_FOLDER=$$quote($$PWD/..)
#CONFIG(debug):CONF_OUT=debug
#CONFIG(release):CONF_OUT=release

# Replace slashes on Windows.
win32:ORIG_FOLDER ~= s,/,\\,g

# Set up string to exec post-build.
win32:BATCH=$$quote($$PWD\build_post_windows.bat)
unix:BATCH=$$quote($$PWD/build_post_unix.sh)

# Set exec string.
# Batch file expects arg 1 to be the source folder to copy from, and the second to be the configuration we're building with (debug/release).
QMAKE_POST_LINK += $$BATCH $$ORIG_FOLDER $$CONF_OUT $$escape_expand(\\n\\t)
