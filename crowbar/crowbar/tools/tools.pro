#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T12:41:20
#
#-------------------------------------------------

QT       += gui 3d

TARGET = tools
TEMPLATE = lib

DEFINES += TOOLS_LIBRARY

SOURCES += \
    mapbrushbuilder.cpp \
    basetool.cpp \
    toolselect.cpp \
    toolselectdockwidget.cpp \
    toolsetitem.cpp \
    toolcreatebrush.cpp \
    toolcreatebrushdockwidget.cpp \
    tooleditfacedialog.cpp \
    tooleditface.cpp \
    tooleditfacedockwidget.cpp \
    vertexlistoperator.cpp \
    brushvertexoperator.cpp \
    hierarchyeditdialogue.cpp \
    tooledithierarchy.cpp \
    toolhierarchyeditdockwidget.cpp

HEADERS +=\
        tools_global.h \
    mapbrushbuilder.h \
    basetool.h \
    toolselect.h \
    toolselectdockwidget.h \
    toolsetitem.h \
    toolcreatebrush.h \
    toolcreatebrushdockwidget.h \
    tooleditfacedialog.h \
    tooleditface.h \
    tooleditfacedockwidget.h \
    vertexlistoperator.h \
    brushvertexoperator.h \
    hierarchyeditdialogue.h \
    tooledithierarchy.h \
    toolhierarchyeditdockwidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel
macx: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../view/release/ -lview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../view/debug/ -lview
else:unix:!macx: LIBS += -L$$OUT_PWD/../view/ -lview
macx: LIBS += -L$$OUT_PWD/../view/ -lview

INCLUDEPATH += $$PWD/../view
DEPENDPATH += $$PWD/../view

FORMS += \
    toolselectdockwidget.ui \
    toolcreatebrushdockwidget.ui \
    tooleditfacedialog.ui \
    tooleditfacedockwidget.ui \
    hierarchyeditdialogue.ui \
    toolhierarchyeditdockwidget.ui
