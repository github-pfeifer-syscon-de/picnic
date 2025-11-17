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

PICNIC_INTF_XML=$(PICNIC_INTF_PREFIX).xml

libthumbnail1_a_CFLAGS= -Wall \
    -Wconversion

libthumbnail1_a_CXXFLAGS = -Wall \
    -Wconversion

BUILT_SOURCES = \
	$(PICNIC_INTF_XML)

libthumbnail1_a_SOURCES = \
	$(PICNIC_INTF_PREFIX)_common.h \
	$(PICNIC_INTF_PREFIX)_common.cpp \
	$(PICNIC_INTF_PREFIX)_proxy.h \
	$(PICNIC_INTF_PREFIX)_proxy.cpp \
	$(PICNIC_INTF_PREFIX)_stub.h \
	$(PICNIC_INTF_PREFIX)_stub.cpp

# since these dependencies force a rebuild every time, commented them out
#thumbnail1.xml:
#    $(shell gdbus introspect -e -d org.freedesktop.thumbnails.Thumbnailer1 -o /org/freedesktop/thumbnails/Thumbnailer1 -x > $(PICNIC_INTF_XML))
#
#thumbnail1_common.cpp: thumbnail1.xml
#    $(shell gdbus-codegen-glibmm3 --generate-cpp-code=$(PICNIC_INTF_PREFIX) $(PICNIC_INTF_XML))



