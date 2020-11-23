TEMPLATE = subdirs

android {
    qtHaveModule(quick) {
        SUBDIRS += \
            notification \
            jnimessenger \
            services \
            customactivity \
            musiclist

        EXAMPLE_FILES += \
            notification \
            jnimessenger \
            services \
            customactivity \
            musiclist
    }
}
