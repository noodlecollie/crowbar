QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crowbar
TEMPLATE = app

INCLUDEPATH += "..\glew-1.9.0\include"

SOURCES += \
    main.cpp \
    mainwin.cpp \
    pluginmanager.cpp \
    logwindow.cpp \
    commandlineparser.cpp \
    globals.cpp

HEADERS += \
    mainwin.h \
    pluginmanager.h \
    plugin.h \
    logwindow.h \
    commandlineparser.h \
    globals.h

# LIBS is from the main build directory (where debug/release folders are located)
#LIBS           = -Llib -lcbp_docmanager

#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#   mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
#   win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
#}

# NOTE: These paths assume your build directory is in the same directory as the root Crowbar folder (within which is Crowbar.pro).
# For VS at least, the folder hierarchy seems to be <build directory>/app/ to get to the Makefile.
LIBS += -L..\..\Crowbar\glew-1.9.0\lib -lglew32

# For Windows:
RC_FILE = crowbar.rc

# For OS/X:
ICON = crowbar.icns
