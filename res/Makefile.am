
PKGCONFIG=pkgconf
GLIB_COMPILE_RESOURCES=$(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

PICNIC_RESOURCE_XML=$(srcdir)/picnic.gresource.xml
PICNIC_RESOURCES=$(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --generate-dependencies $(PICNIC_RESOURCE_XML))
PICNIC_RESOURCE_C=$(top_srcdir)/src/resources.c


all:$(PICNIC_RESOURCE_C)

$(PICNIC_RESOURCE_C): $(PICNIC_RESOURCE_XML) $(PICNIC_RESOURCES)
	$(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(dir $<) --generate-source $<



# The desktop files
desktopdir=$(datadir)/applications
desktop_DATA=picnic.desktop


# application icon
icon_basename=picnic
pixmapdir=$(datadir)/icons/hicolor/64x64/apps
pixmap_DATA=$(icon_basename).png
appicondir=$(datadir)/icons/hicolor/scalable/apps
appicon_DATA=$(icon_basename).svg

EXTRA_DIST=$(PICNIC_RESOURCES) $(PICNIC_RESOURCE_XML) $(desktop_DATA) $(pixmap_DATA) $(appicon_DATA)

