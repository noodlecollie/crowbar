TEMPLATE = subdirs

SUBDIRS += \
    application \
    modules \
    experiments

application.depends += \
    modules

experiments.depends += \
    modules
