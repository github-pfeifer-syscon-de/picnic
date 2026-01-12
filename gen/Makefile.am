## Process this file with automake to produce Makefile.in
# this is provided as reference (the generated files are included)
#   see readme for the prerequisites

noinst_LIBRARIES = libthumbnail1.a

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)."\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GTHREAD_CFLAGS) \
	$(GENERICIMG_CFLAGS)

PICNIC_INTF_PREFIX=thumbnail1
PICNIC_INTF_XML_BASE=$(PICNIC_INTF_PREFIX).xml
PICNIC_INTF_XML_PATH=$(srcdir)/$(PICNIC_INTF_XML_BASE)
DBUS_INTF_NAME=org.freedesktop.thumbnails.Thumbnailer1
DBUS_INTF_PATH=/org/freedesktop/thumbnails/Thumbnailer1

libthumbnail1_a_CXXFLAGS = -Wall \
    -Wconversion

libthumbnail1_a_SOURCES = \
	$(PICNIC_INTF_PREFIX)_common.cpp \
	$(PICNIC_INTF_PREFIX)_common.h \
	$(PICNIC_INTF_PREFIX)_proxy.cpp \
	$(PICNIC_INTF_PREFIX)_proxy.h \
	$(PICNIC_INTF_PREFIX)_stub.cpp \
	$(PICNIC_INTF_PREFIX)_stub.h

# as this part is "normal" make syntax needs to be properly tabbed!
#   gdbus may come with the glib package (or maybe some developer part?)
$(PICNIC_INTF_XML_PATH):
	$(shell gdbus introspect -e -d $(DBUS_INTF_NAME) -o $(DBUS_INTF_PATH) -x > $(PICNIC_INTF_XML_PATH))

# howto get gdbus-codegen-glibmm3 see README.md
$(PICNIC_INTF_PREFIX)_common.cpp: $(PICNIC_INTF_XML_PATH)
	$(shell cd $(srcdir); gdbus-codegen-glibmm3 --generate-cpp-code=$(PICNIC_INTF_PREFIX) $(PICNIC_INTF_XML_BASE))
