## Process this file with automake to produce Makefile.in

bin_PROGRAMS = picnic

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)."\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GTHREAD_CFLAGS) \
	$(GENERICGLM_CFLAGS) \
	$(GENERICIMG_CFLAGS) \
	$(EPOXY_CFLAGS) \
	$(OPENSSL_CFLAGS)

picnic_LDFLAGS = 

picnic_CFLAGS = $(WARN_CFLAGS)
picnic_CXXFLAGS = $(WARN_CXXFLAGS)

if RELEASE
picnic_CXXFLAGS += $(OPTIM_CXXFLAGS)
else
picnic_CXXFLAGS += $(INSTUMENT_CXXFLAGS)
picnic_LDFLAGS += $(INSTUMENT_LINKFLAGS)
endif


picnic_SOURCES = \
	resources.c \
	Worker.cpp \
	Worker.hpp \
	DbusWorker.cpp \
	DbusWorker.hpp \
	ThreadWorker.cpp \
	ThreadWorker.hpp \
	PicnicApp.cpp \
	PicnicApp.hpp \
	PicnicWindow.cpp \
	PicnicWindow.hpp \
	PicnicView.hpp \
	PicnicView.cpp \
	Pict.cpp \
	Pict.hpp \
	Tile.cpp \
	Tile.hpp \
	Layout.cpp \
	Layout.hpp \
	DirNode.cpp \
	DirNode.hpp \
	ImageIconList.cpp \
	ImageIconList.hpp \
	FileSupport.cpp \
	FileSupport.hpp \
	FileTree.cpp \
	FileTree.hpp \
	FileTreeView.cpp \
	FileTreeView.hpp \
	RectLayout.cpp \
	RectLayout.hpp \
	RollLayout.cpp \
	RollLayout.hpp \
	FileFinder.cpp \
	FileFinder.hpp \
	CubeLayout.cpp \
	CubeLayout.hpp

picnic_LDADD = ../gen/libthumbnail1.a \
	$(GENERICGLM_LIBS) \
	$(GENERICIMG_LIBS) \
	$(GTHREAD_LIBS) \
	$(EPOXY_LIBS) \
	$(OPENSSL_LIBS)


# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(pkgdatadir)

