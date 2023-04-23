dnl Process this file with autoconf to produce a configure script.

AC_CONFIG_MACRO_DIR([m4])

AC_INIT([picnic],[0.1],[picnic@pfeifer-syscon.de])

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11])

AM_SILENT_RULES([yes])

AX_CHECK_ENABLE_DEBUG([info], [DEBUG], [RELEASE], [IS-RELEASE])
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11(noext, mandatory)

LT_INIT

PKG_CHECK_MODULES(EPOXY, [epoxy])

PKG_CHECK_MODULES(GTHREAD, [gthread-2.0])

PKG_CHECK_MODULES(GENERICGLM,genericglm-0.21)

PKG_CHECK_MODULES(GLIBMM, [glibmm-2.4])

PKG_CHECK_MODULES(GTKMM, [gtkmm-3.0])

PKG_CHECK_MODULES(OPENSSL, [openssl])

PKG_CHECK_MODULES(LIBEXIF, [libexif])

AX_CHECK_SED()

AC_CONFIG_FILES([
Makefile
src/Makefile
res/Makefile
])
AC_OUTPUT
