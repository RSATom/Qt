TEMPLATE = subdirs

SUBDIRS += \
    servicesameprocess \
    servicebinder \
    servicebroadcast \
    servicebroadcastsamelib

qtHaveModule(remoteobjects) {
    SUBDIRS += \
        serviceremoteobjects \
        serviceremoteobjectssamelib
}
