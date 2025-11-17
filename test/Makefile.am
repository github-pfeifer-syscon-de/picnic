
TESTS = bus_test

check_PROGRAMS = bus_test

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)."\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GLIBMM_CFLAGS) \
	$(DBUS_CFLAGS) \
	-I../src

AM_CFLAGS= \
	-Wall -Wpedantic

AM_CXXFLAGS = \
	-Wall -Wpedantic


bus_test_LDADD =  \
	$(GLIBMM_LIBS) \
	$(DBUS_LIBS)

bus_test_SOURCES = \
    bus_test.cpp
