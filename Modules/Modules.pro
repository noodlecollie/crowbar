TEMPLATE = subdirs

# Allows us to compile in the correct order!
CONFIG += ordered

SUBDIRS += \
    RegexUtil \
    IConsole \
    CommandStore \
    IRenderSystem \
    Geometry \
    Octree
