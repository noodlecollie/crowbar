#-------------------------------------------------
#
# Project created by QtCreator 2014-10-16T11:07:16
#
#-------------------------------------------------

QT       += core gui 3d

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = debug_view
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cameracontrollerv2.cpp \
    openglcameraanimation.cpp \
    openglwidgetv2.cpp \
    qangle.cpp \
    qglcamera2.cpp \
    brushaddwidget.cpp

HEADERS  += mainwindow.h \
    cameracontrollerv2.h \
    icamera3d.h \
    openglcameraanimation.h \
    openglwidgetv2.h \
    qangle.h \
    qglcamera2.h \
    brushaddwidget.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../../model/ -lmodel
macx: LIBS += -L$$OUT_PWD/../../model/ -lmodel

INCLUDEPATH += $$PWD/../../model
DEPENDPATH += $$PWD/../../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../view/release/ -lview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../view/debug/ -lview
else:unix: LIBS += -L$$OUT_PWD/../../view/ -lview
macx: LIBS += -L$$OUT_PWD/../../view/ -lview

INCLUDEPATH += $$PWD/../../view
DEPENDPATH += $$PWD/../../view

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../tools/release/ -ltools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../tools/debug/ -ltools
else:unix: LIBS += -L$$OUT_PWD/../../tools/ -ltools
macx: LIBS += -L$$OUT_PWD/../../tools/ -ltools

INCLUDEPATH += $$PWD/../../tools
DEPENDPATH += $$PWD/../../tools

FORMS += \
    brushaddwidget.ui
