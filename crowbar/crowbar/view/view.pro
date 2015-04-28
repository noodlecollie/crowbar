#-------------------------------------------------
#
# Project created by QtCreator 2014-10-09T10:39:39
#
#-------------------------------------------------

QT += 3d

TARGET = view
TEMPLATE = lib

# Needed for render table entries to be in a separate folder, otherwise I need to put "../" in front of everything.
# Funnily enough Mac works fine without...
INCLUDEPATH += $$PWD

DEFINES += VIEW_LIBRARY

SOURCES += \
    maporiginmarker.cpp \
    mapview.cpp \
    glanglecamera.cpp \
    usercontrolledcamera.cpp \
    view_util.cpp \
    mapviewgrid.cpp \
    mapview3d.cpp \
    mapview2d.cpp \
    mapdocumentview.cpp \
    rendertableentries/rendertableentry.cpp \
    rendertableentries/renderentrymapbrush.cpp \
    rendertableentries/renderentryfactory.cpp \
    effects/selectionboxeffect.cpp \
    rendertableentries/renderentrymaplightpositional.cpp \
    rendertableentries/renderentrytoolbasicsquare.cpp \
    rendertableentries/renderentrytoolwireframebox.cpp \
    vieweffects.cpp \
    effects/colourblendedtextureeffect.cpp \
    effects/effectglobals.cpp \
    maptreewidget.cpp \
    maptreewidgetdialog.cpp

HEADERS +=\
        view_global.h \
    maporiginmarker.h \
    mapview.h \
    irender.h \
    glanglecamera.h \
    usercontrolledcamera.h \
    view_util.h \
    mapviewgrid.h \
    mapview3d.h \
    mapview2d.h \
    mapdocumentview.h \
    rendertableentries/rendertableentry.h \
    rendertableentries/renderentrymapbrush.h \
    rendertableentries/renderentryfactory.h \
    effects/selectionboxeffect.h \
    rendertableentries/renderentrymaplightpositional.h \
    rendertableentries/renderentrytoolbasicsquare.h \
    rendertableentries/renderentrytoolwireframebox.h \
    vieweffects.h \
    effects/colourblendedtextureeffect.h \
    effects/effectglobals.h \
    maptreewidget.h \
    maptreewidgetdialog.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

FORMS += \
    maptreewidgetdialog.ui
