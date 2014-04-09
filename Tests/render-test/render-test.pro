#-------------------------------------------------
#
# Project created by QtCreator 2014-03-20T12:46:18
#
#-------------------------------------------------

QT  += core gui opengl 3d
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = render-test
TEMPLATE = app

SOURCES += \
    main.cpp \
    customview.cpp \
    customview2.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53Dd
else:unix: LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53D

INCLUDEPATH += $$PWD/../../../qt3d/include
DEPENDPATH += $$PWD/../../../qt3d/include

HEADERS += \
    customview.h \
    customview2.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Modules/Geometry/release/ -lGeometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Modules/Geometry/debug/ -lGeometry
else:unix: LIBS += -L$$OUT_PWD/../../Modules/Geometry/ -lGeometry

INCLUDEPATH += $$PWD/../../Modules/Geometry/inc
DEPENDPATH += $$PWD/../../Modules/Geometry/inc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Modules/IRenderSystem/release/ -lIRenderSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Modules/IRenderSystem/debug/ -lIRenderSystem
else:unix: LIBS += -L$$OUT_PWD/../../Modules/IRenderSystem/ -lIRenderSystem

INCLUDEPATH += $$PWD/../../Modules/IRenderSystem/inc
DEPENDPATH += $$PWD/../../Modules/IRenderSystem/inc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Modules/DataStructures/release/ -lDataStructures
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Modules/DataStructures/debug/ -lDataStructures
else:unix: LIBS += -L$$OUT_PWD/../../Modules/DataStructures/ -lDataStructures

INCLUDEPATH += $$PWD/../../Modules/DataStructures/inc
DEPENDPATH += $$PWD/../../Modules/DataStructures/inc
