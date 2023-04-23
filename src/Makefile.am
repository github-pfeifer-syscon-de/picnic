## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)."\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GTKMM_CFLAGS) \
	$(GLIBMM_CFLAGS) \
	$(GTHREAD_CFLAGS) \
	$(GENERICGLM_CFLAGS) \
	$(EPOXY_CFLAGS) \
	$(OPENSSL_CFLAGS) \
	$(LIBEXIF_CFLAGS)


AM_CFLAGS= \
	-Wall

AM_CXXFLAGS =\
	-Wall

bin_PROGRAMS = picnic

picnic_SOURCES = \
	resources.c \
	thumbnail1_common.h \
	thumbnail1_common.cpp \
	thumbnail1_proxy.h \
	thumbnail1_proxy.cpp \
	thumbnail1_stub.h \
	thumbnail1_stub.cpp \
	Worker.cpp \
	Worker.hpp \
	DbusWorker.cpp \
	DbusWorker.hpp \
	ThreadWorker.cpp \
	ThreadWorker.hpp \
	ConcurrentCollections.cpp \
	ConcurrentCollections.hpp \
	PicnicApp.cpp \
	PicnicApp.hpp \
	PicnicWindow.cpp \
	PicnicView.hpp \
	Pict.cpp \
	Pict.hpp \
	PicnicView.cpp \
	PicnicWindow.hpp \
	Tile.cpp \
	Tile.hpp \
	Layout.cpp \
	Layout.hpp \
	ImageArea.cpp \
	ImageArea.hpp \
	ImageView.cpp \
	ImageView.hpp \
	ImageOptions.cpp \
	ImageOptions.hpp \
	ImageOptionDialog.cpp \
	ImageOptionDialog.hpp \
	BinModel.cpp \
	BinModel.hpp \
	BinView.cpp \
	BinView.hpp \
	ApplicationSupport.cpp \
	ApplicationSupport.hpp \
	DisplayImage.cpp \
	DisplayImage.hpp \
	ExifReader.cpp \
	ExifReader.hpp \
	ImageIconList.cpp \
	ImageIconList.hpp \
	FileTree.cpp \
	FileTree.hpp \
	FileTreeView.cpp \
	FileTreeView.hpp \
	FileSupport.cpp \
	FileSupport.hpp \
	PicnicListWin.cpp \
	PicnicListWin.hpp \
	DirNode.cpp \
	DirNode.hpp \
	ImageList.cpp \
	ImageList.hpp \
	RectLayout.cpp \
	RectLayout.hpp \
	RollLayout.cpp \
	RollLayout.hpp \
	FileFinder.cpp \
	FileFinder.hpp \
	CubeLayout.cpp \
	CubeLayout.hpp



picnic_LDFLAGS =

picnic_LDADD = $(GENERICGLM_LIBS) \
	$(GLIBMM_LIBS) \
	$(GTKMM_LIBS) \
	$(GTHREAD_LIBS) \
	$(EPOXY_LIBS) \
	$(OPENSSL_LIBS) \
	$(LIBEXIF_LIBS)


# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(pkgdatadir)

