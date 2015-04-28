#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T10:46:39
#
#-------------------------------------------------

QT       += core gui 3d

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    applicationsettings.cpp \
    orthographicsubwindow.cpp \
    texturetabledockwidget.cpp

HEADERS  += mainwindow.h \
    applicationsettings.h \
    orthographicsubwindow.h \
    texturetabledockwidget.h

FORMS    += mainwindow.ui \
    orthographicsubwindow.ui \
    texturetabledockwidget.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../model/release/ -lmodel
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../model/debug/ -lmodel
else:unix: LIBS += -L$$OUT_PWD/../model/ -lmodel

INCLUDEPATH += $$PWD/../model
DEPENDPATH += $$PWD/../model

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tools/release/ -ltools
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tools/debug/ -ltools
else:unix: LIBS += -L$$OUT_PWD/../tools/ -ltools

INCLUDEPATH += $$PWD/../tools
DEPENDPATH += $$PWD/../tools

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../view/release/ -lview
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../view/debug/ -lview
else:unix: LIBS += -L$$OUT_PWD/../view/ -lview

INCLUDEPATH += $$PWD/../view
DEPENDPATH += $$PWD/../view

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../serialisation/release/ -lserialisation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../serialisation/debug/ -lserialisation
else:unix:!macx: LIBS += -L$$OUT_PWD/../serialisation/ -lserialisation
macx: LIBS += -L$$OUT_PWD/../serialisation/ -lserialisation

INCLUDEPATH += $$PWD/../serialisation
DEPENDPATH += $$PWD/../serialisation

OTHER_FILES +=

RESOURCES += \
    ../resources/resource.qrc
