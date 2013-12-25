QT  += core gui opengl qt3d
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = 3dtest
TEMPLATE = app
CONFIG += qt3d_deploy_pkg

SOURCES += \
    teapotview.cpp \
    main.cpp

HEADERS += \
    teapotview.h
