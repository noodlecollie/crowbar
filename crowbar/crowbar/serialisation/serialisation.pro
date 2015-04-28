#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T08:59:14
#
#-------------------------------------------------

QT       += gui 3d

TARGET = serialisation
TEMPLATE = lib

DEFINES += SERIALISATION_LIBRARY

SOURCES += \
    keyvaluestoken.cpp \
    keyvaluesparser.cpp \
    documentserialiser.cpp

HEADERS +=\
        serialisation_global.h \
    keyvaluestoken.h \
    keyvaluesparser.h \
    documentserialiser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix:!macx: LIBS += -L$$OUT_PWD/../model/ -lmodel
macx: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model
