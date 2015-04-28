TARGET = model
TEMPLATE = lib
QT += core gui
CONFIG += dll
DEFINES += MODEL_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

include(model.pri)

HEADERS += \
    model_global.h
