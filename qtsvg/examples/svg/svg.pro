TEMPLATE = subdirs

qtHaveModule(widgets): SUBDIRS += embeddedsvgviewer  svggenerator  svgviewer
SUBDIRS += embedded richtext draganddrop

qtHaveModule(opengl):!qtConfig(opengles2): SUBDIRS += opengl
