# ========== Crowbar ==========
# A modular, extensible level editor for the Source engine.
# Author: [X6] Herbius, inception around May 2013
#
# Current modules:
# app           - Main application executable.
# IConsole      - Console library. Defines developer console components, console commands and variables.
# RegexUtil     - Utility regular expression strings put into their own library for portability.
# CommandStore  - Library of static ConCommand callbacks that can be referenced.
#
# Todo:     Add namespace to all IConsole entries.
#           Add signals for ConVars pre/post change.
#           Automate the build (by building libraries in order) if possible.

TEMPLATE = subdirs

SUBDIRS += \
    app \
    Tests \
    Modules 
