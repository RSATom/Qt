TEMPLATE = subdirs

qtHaveModule(location) {

    SUBDIRS += geotestplugin    # several subtargets depend on this

    #Place unit tests
    SUBDIRS += qplace \
           qplaceattribute \
           qplacecategory \
           qplacecontactdetail \
           qplacecontentrequest \
           qplacedetailsreply \
           qplaceeditorial \
           qplacematchreply \
           qplacematchrequest \
           qplaceimage \
           qplaceratings \
           qplaceresult \
           qproposedsearchresult \
           qplacereply \
           qplacereview \
           qplacesearchrequest \
           qplacesupplier \
           qplacesearchresult \
           qplacesearchreply \
           qplacesearchsuggestionreply \
           qplaceuser

    !android: {
        SUBDIRS += \
           qplacemanager \
           qplacemanager_nokia \
           qplacemanager_unsupported \
           placesplugin_unsupported

        qplacemanager.depends = geotestplugin
    }

    #misc tests
    SUBDIRS +=   doublevectors
    !android: SUBDIRS += cmake qmlinterface # looks for .qmls locally

    #Map and Navigation tests
    SUBDIRS += qgeocodingmanagerplugins \
           qgeocameracapabilities\
           qgeocameradata \
           qgeocodereply \
           qgeomaneuver \
           qgeotiledmapscene \
           qgeoroute \
           qgeoroutereply \
           qgeorouterequest \
           qgeoroutesegment \
           qgeoroutingmanagerplugins \
           qgeotilespec \
           qgeoroutexmlparser \
           maptype \
           qgeocameratiles

    # These use plugins
    !android: {
        SUBDIRS += qgeoserviceprovider \
                         qgeoroutingmanager \
                         nokia_services \
                         qgeocodingmanager \
                         qgeotiledmap

        qgeoserviceprovider.depends = geotestplugin
        qgeotiledmap.depends = geotestplugin
    }
    qtHaveModule(quick):!android {
        SUBDIRS += declarative_geoshape \
                   declarative_core
        declarative_core.depends = geotestplugin

        !mac: {
            SUBDIRS += declarative_ui
            declarative_ui.depends = geotestplugin
        }
    }
}


SUBDIRS += \
           qgeoaddress \
           qgeoshape \
           qgeorectangle \
           qgeocircle \
           qgeopath \
           qgeopolygon \
           qgeocoordinate \
           qgeolocation \
           qgeopositioninfo \
           qgeosatelliteinfo \
           qgeojson

!android: SUBDIRS += \
            positionplugin \
            positionpluginV1 \
            positionplugintest \
            qgeoareamonitor \
            qgeopositioninfosource \
            qgeosatelliteinfosource \
            qnmeapositioninfosource
