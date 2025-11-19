dnl Process this file with autoconf to produce a configure script.

AC_CONFIG_MACRO_DIR([m4])

AC_INIT([picnic],[0.1],[picnic@pfeifer-syscon.de])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11] foreign)

AM_SILENT_RULES([yes])

AX_CHECK_ENABLE_DEBUG([info], [DEBUG], [RELEASE], [IS-RELEASE])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([20], [noext], [optional])

LT_INIT

PKG_CHECK_MODULES([EPOXY], [epoxy])

PKG_CHECK_MODULES(GTHREAD, [gthread-2.0])

PKG_CHECK_MODULES([GENERICGLM], [genericglm >= 0.3.1])

PKG_CHECK_MODULES([GENERICIMG], [genericimg >= 0.3.3])

dnl dont repeat dependencies
PKG_CHECK_MODULES([GLIBMM], [glibmm-2.4 giomm-2.4])
PKG_CHECK_MODULES([DBUS], [dbus-1 dbus-glib-1])
dnl PKG_CHECK_MODULES([GTKMM], [gtkmm-3.0])

PKG_CHECK_MODULES([OPENSSL], [openssl])

AC_ARG_WITH([gles], AS_HELP_STRING([--with-gles], [use openGL ES 3 (Raspi) otherweise fullgl]))

if test "x$with_gles" = "xyes" ; then
    PKG_CHECK_MODULES(EPOXY,[glesv2], [AC_DEFINE([USE_GLES], [1], [Using openGL ES 3])])

else
    PKG_CHECK_MODULES(EPOXY,[epoxy])
fi

AC_CONFIG_FILES([
Makefile
res/Makefile
gen/Makefile
src/Makefile
test/Makefile
])
AC_OUTPUT
