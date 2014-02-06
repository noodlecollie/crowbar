# ========== Crowbar ==========
# A modular, extensible level editor for the Source engine.
# Author: [X6] Herbius, inception around May 2013
#
# Todo:     Add signals for ConVars pre/post change.

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    Modules \
    Tests \
    app
