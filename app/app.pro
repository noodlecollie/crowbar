QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crowbar
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwin.cpp \
    pluginmanager.cpp

HEADERS += \
    mainwin.h \
    pluginmanager.h \
    plugin.h

# LIBS is from the main build directory (where debug/release folders are located)
#LIBS           = -Llib -lcbp_docmanager

#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#   mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
#   win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
#}

# For Windows:
RC_FILE = crowbar.rc

# For OS/X:
ICON = crowbar.icns
