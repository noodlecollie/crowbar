#-------------------------------------------------
#
# Project created by QtCreator 2015-06-16T07:24:11
#
#-------------------------------------------------

QT       += core gui qml

TARGET = 2015-06-16-qml
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

DISTFILES += \
    schema.txt

HEADERS +=

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../modules/model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../modules/model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../../modules/model/ -lmodel

INCLUDEPATH += $$PWD/../../modules/model \
               $$PWD/../../modules/model/core \
               $$PWD/../../modules/model/map/core \
               $$PWD/../../modules/model/util
DEPENDPATH += $$PWD/../../modules/model \
              $$PWD/../../modules/model/core \
              $$PWD/../../modules/model/map/core
              $$PWD/../../modules/model/util
