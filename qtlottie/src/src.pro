TEMPLATE = subdirs

!qtHaveModule(gui): \
    return()

CONFIG += ordered
SUBDIRS += \
    bodymovin \
    imports
