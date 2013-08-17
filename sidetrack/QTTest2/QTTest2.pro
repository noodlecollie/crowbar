QT  += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTTest2
TEMPLATE = app

SOURCES += \
    main.cpp \
    glwidget.cpp

HEADERS += \
    glwidget.h \
    cube.h

OTHER_FILES += \
    simple.vert \
    simple.frag
