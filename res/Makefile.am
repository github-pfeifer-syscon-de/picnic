
PKGCONFIG=pkgconf
GLIB_COMPILE_RESOURCES=$(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

PICNIC_RESOURCE_XML=picnic.gresource.xml
PICNIC_RESOURCES=$(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=. --generate-dependencies $(PICNIC_RESOURCE_XML))
PICNIC_RESOURCE_C=../src/resources.c


all:$(PICNIC_RESOURCE_C)

$(PICNIC_RESOURCE_C): $(PICNIC_RESOURCE_XML) $(PICNIC_RESOURCES)
	$(GLIB_COMPILE_RESOURCES) --target=$(PICNIC_RESOURCE_C) --sourcedir=. --generate-source $(PICNIC_RESOURCE_XML)



# The desktop files
desktopdir=$(datadir)/applications
desktop_in_files=picnic.desktop.in
desktop_DATA=$(desktop_in_files:.desktop.in=.desktop)


# application icon
appicondir=$(datadir)/icons/hicolor/64x64/apps
appicon_DATA=picnic.png
appdir_icon=$(appicondir)/$(appicon_DATA)

EXTRA_DIST=$(PICNIC_RESOURCES) $(PICNIC_RESOURCE_XML) $(desktop_in_files) $(appicon_DATA)

DISTCLEANFILES=$(desktop_DATA)

%.desktop: %.desktop.in
	@regex_cmd@ -e "s,\@APP_ICON\@,$(appdir_icon),g" $? > $@