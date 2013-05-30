QT  += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Crowbar
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwin.cpp

HEADERS += \
    mainwin.h \
    interfaces/ICrowbarExtension.h

# For Windows:
RC_FILE = crowbar.rc

# For OS/X:
ICON = crowbar.icns
