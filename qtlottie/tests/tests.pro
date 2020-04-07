TEMPLATE = subdirs

!qtHaveModule(bodymovin): \
    return()

SUBDIRS += \
    auto \
    manual
