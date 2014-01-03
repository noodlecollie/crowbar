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

QT  += core gui opengl
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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../Modules/IConsole/release/ -lIConsole
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../Modules/IConsole/debug/ -lIConsole
else:unix: LIBS += -L$$OUT_PWD/../../../Modules/IConsole/ -lIConsole

INCLUDEPATH += $$PWD/../../Modules/IConsole/inc
DEPENDPATH += $$PWD/../../Modules/IConsole/inc