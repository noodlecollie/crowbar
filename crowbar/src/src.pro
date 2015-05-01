TEMPLATE = subdirs

SUBDIRS += \
    application \
    tools \
    modules \
    experiments

application.depends += \
    modules

tools.depends += \
    modules

experiments.depends += \
    modules
