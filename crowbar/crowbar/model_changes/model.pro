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
    util.cpp \
    hierarchynode3d.cpp \
    mapdocument.cpp \
    mapbrush.cpp \
    mapvertex.cpp \
    mapedge.cpp \
    handleobjectlist.cpp \
    mapface.cpp \
    mapcomponent.cpp

HEADERS +=\
        model_global.h \
    eulerangle.h \
    util.h \
    hierarchynode3d.h \
    mapdocument.h \
    mapbrush.h \
    imaphandle.h \
    handleobjectlist.h \
    imaptitle.h \
    mapvertex.h \
    mapedge.h \
    mapface.h \
    mapcomponent.h \
    iselectable.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
