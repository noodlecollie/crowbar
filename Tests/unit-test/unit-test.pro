#defineTest(copyToResourceDir) {
#    files = $$1
#
#    for(FILE, files) {
#        
#    # TODO: Create these folders if they don't exist.
#    CONFIG(debug):DDIR = $$OUT_PWD/debug/resource/
#    CONFIG(release):DDIR = $$OUT_PWD/release/resource
#
#        # Replace slashes in paths with backslashes for Windows
#        win32:FILE ~= s,/,\\,g
#        win32:DDIR ~= s,/,\\,g
#
#        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
#    }
#
#    export(QMAKE_POST_LINK)
#}

# This doesn't work yet.
#defineTest(createDir) {
#    directory = $$1
#
#    win32:directory ~= s,/,\\,g
#
#    temp = 0
#    exists($$directory) {
#        temp = "does exist"
#    } else {
#        temp = "doesn't exist"
#    }
#    message($$directory $$temp)
#    
#    !exists($$directory) {
#        QMAKE_POST_LINK += mkdir $$quote($$directory) $$escape_expand(\\n\\t)
#    }
#}

QT  += core gui opengl 3d
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = unit-test
TEMPLATE = app

#if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
#   mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
#   win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
#}

#if(debug):createDir($$OUT_PWD/debug/resource/)
#if(release):createDir($$OUT_PWD/release/resource/)

SOURCES += \
    main.cpp

#copyToResourceDir($$PWD/../crowbar/resource/manifest.qss $$PWD/../crowbar/resource/command_symbol.svg $$PWD/../crowbar/resource/variable_symbol.svg)

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53D
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53Dd
else:unix: LIBS += -L$$PWD/../../../qt3d/lib/ -lQt53D

INCLUDEPATH += $$PWD/../../../qt3d/include
DEPENDPATH += $$PWD/../../../qt3d/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Modules/IRenderSystem/release/ -lIRenderSystem
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Modules/IRenderSystem/debug/ -lIRenderSystem
else:unix: LIBS += -L$$OUT_PWD/../../Modules/IRenderSystem/ -lIRenderSystem

INCLUDEPATH += $$PWD/../../Modules/IRenderSystem/inc
DEPENDPATH += $$PWD/../../Modules/IRenderSystem/inc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Modules/Geometry/release/ -lGeometry
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Modules/Geometry/debug/ -lGeometry
else:unix: LIBS += -L$$OUT_PWD/../../Modules/Geometry/ -lGeometry

INCLUDEPATH += $$PWD/../../Modules/Geometry/inc
DEPENDPATH += $$PWD/../../Modules/Geometry/inc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Modules/DataStructures/release/ -lDataStructures
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Modules/DataStructures/debug/ -lDataStructures
else:unix: LIBS += -L$$OUT_PWD/../../Modules/DataStructures/ -lDataStructures

INCLUDEPATH += $$PWD/../../Modules/DataStructures/inc
DEPENDPATH += $$PWD/../../Modules/DataStructures/inc
