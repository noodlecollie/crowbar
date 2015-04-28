TEMPLATE = subdirs

SUBDIRS += \
    application \
    tools \
    modules

application.depends += \
    modules

tools.depends += \
    modules
