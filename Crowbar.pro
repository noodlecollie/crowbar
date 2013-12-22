# ========== Crowbar ==========
# A modular, extensible level editor for the Source engine.
# Author: [X6] Herbius, inception around May 2013
#
# Current modules:
# app           - Main application executable.
# IConsole      - Console library. Defines developer console components, console commands and variables.
# RegexUtil     - Utility regular expression strings put into their own library for portability.
#
# Todo: Clean up the app.pro file to automate everything that's needed (eg. referencing correct libraries when in debug/release mode, etc.
#       Automate the build (by building libraries in order) if possible.

TEMPLATE = subdirs

SUBDIRS += \
    app \
    IConsole \
    unit-test \
    RegexUtil
