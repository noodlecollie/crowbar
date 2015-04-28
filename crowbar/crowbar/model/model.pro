#-------------------------------------------------
#
# Project created by QtCreator 2014-09-23T11:01:57
#
#-------------------------------------------------

QT       += gui 3d

TARGET = model
TEMPLATE = lib

DEFINES += MODEL_LIBRARY

SOURCES += \
    eulerangle.cpp \
    model_util.cpp \
    mapsubcomponent.cpp \
    handlepool.cpp \
    mapworld.cpp \
    positionalsubcomponent.cpp \
    mapcomponentset.cpp \
    mapbrush.cpp \
    mapcomponent.cpp \
    mapdocument.cpp \
    mapedge.cpp \
    mapface.cpp \
    mapvertex.cpp \
    hierarchytransform_new.cpp \
    maplightbase.cpp \
    maplightpositional.cpp \
    modeltree.cpp \
    modeltreeitem.cpp \
    inputtranslator.cpp \
    toolcomponent.cpp \
    toolbasicsquare.cpp \
    toolwireframebox.cpp \
    facetextureinfo.cpp

HEADERS +=\
        model_global.h \
    eulerangle.h \
    changesignal.h \
    model_util.h \
    mapsubcomponent.h \
    handlepool.h \
    mapworld.h \
    positionalsubcomponent.h \
    shared_vectors.h \
    mapcomponentset.h \
    imaphandle.h \
    mapcomponent.h \
    mapdocument.h \
    mapedge.h \
    mapface.h \
    mapvertex.h \
    mapbrush.h \
    hierarchytransform_new.h \
    maplightbase.h \
    maplightpositional.h \
    modeltree.h \
    modeltreeitem.h \
    inputtranslator.h \
    toolcomponent.h \
    toolbasicsquare.h \
    toolwireframebox.h \
    facetextureinfo.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
