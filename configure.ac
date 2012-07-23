# Process this file with autoconf to create configure.

AC_PREREQ([2.65])

# ====================
# Version informations
# ====================
m4_define([libwpd_version_major],[0])
m4_define([libwpd_version_minor],[9])
m4_define([libwpd_version_micro],[4])
m4_define([libwpd_version],[libwpd_version_major.libwpd_version_minor.libwpd_version_micro])

# =============
# Automake init
# =============
AC_INIT([libwpd],[libwpd_version])
AC_CONFIG_MACRO_DIR([m4])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE([1.11 foreign dist-xz dist-bzip2])
AM_SILENT_RULES([yes])
AC_LANG([C++])

# ===========================
# Find required base packages
# ===========================
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL
AC_LIBTOOL_WIN32_DLL
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# =================================
# Libtool/Version Makefile settings
# =================================
AC_SUBST(WPD_MAJOR_VERSION, [libwpd_version_major])
AC_SUBST(WPD_MINOR_VERSION, [libwpd_version_minor])
AC_SUBST(WPD_MICRO_VERSION, [libwpd_version_micro])
AC_SUBST(WPD_VERSION, [libwpd_version])
# AC_SUBST(LT_RELEASE, [libwpd_version_major.libwpd_version_minor])
LT_CURRENT=`expr 100 '*' libwpd_version_major + libwpd_version_minor`
# For 1.0.0 comment the first line and uncomment the second
LT_AGE=0
# LT_AGE=libwpd_version_minor
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION, [libwpd_version_micro])
AC_SUBST(LT_AGE)

# ==========================
# Platform check for windows
# ==========================
AC_MSG_CHECKING([for native Win32])
AS_CASE([$host],
	[*-*-mingw*], [
		native_win32=yes
		LIBWPD_WIN32_RESOURCE=libwpd-win32res.lo
		LIBWPD_STREAM_WIN32_RESOURCE=libwpd-stream-win32res.lo
		WPD2HTML_WIN32_RESOURCE=wpd2html-win32res.lo
		WPD2TEXT_WIN32_RESOURCE=wpd2text-win32res.lo
		WPD2RAW_WIN32_RESOURCE=wpd2raw-win32res.lo
	], [
		native_win32=no
		LIBWPD_WIN32_RESOURCE=
		LIBWPD_STREAM_WIN32_RESOURCE=
		WPD2HTML_WIN32_RESOURCE=
		WPD2TEXT_WIN32_RESOURCE=
		WPD2RAW_WIN32_RESOURCE=
	]
)
AC_MSG_RESULT([$native_win32])
AM_CONDITIONAL(OS_WIN32, [test "x$native_win32" = "xyes"])
AC_SUBST(LIBWPD_WIN32_RESOURCE)
AC_SUBST(LIBWPD_STREAM_WIN32_RESOURCE)
AC_SUBST(WPD2HTML_WIN32_RESOURCE)
AC_SUBST(WPD2TEXT_WIN32_RESOURCE)
AC_SUBST(WPD2RAW_WIN32_RESOURCE)

AC_MSG_CHECKING([for Win32 platform in general])
AS_CASE([$host],
	[*-*-mingw*|*-*-cygwin*], [platform_win32=yes],
	[platform_win32=no]
)
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL([PLATFORM_WIN32], [test "x$platform_win32" = "xyes"])

AC_ARG_ENABLE([def-files],
	[AS_HELP_STRING([--enable-def-files], [Enable use of def files to define exported symbols on win32])],
	[enable_def_files="$enableval"],
	[enable_def_files=no]
)
AM_CONDITIONAL(DEF_FILES, [test "x$enable_def_files" = "xyes" -a "x$native_win32" = "xyes"])

# =========================
# Platform check for darwin
# =========================
AC_MSG_CHECKING([for Darwin (Mac OS X) platform])
AS_CASE([$host],
	[*-*-darwin*], [platform_darwin=yes],
	[platform_darwin=no]
)
AC_MSG_RESULT([$platform_darwin])
AM_CONDITIONAL(PLATFORM_DARWIN, [test "x$platform_darwin" = "xyes"])

# ================
# Check for cflags
# ================
AC_ARG_ENABLE([werror],
	[AS_HELP_STRING([--disable-werror], [Treat all warnings as errors, usefull for development])],
	[enable_werror="$enableval"],
	[enable_werror=yes]
)
AS_IF([test x"$enable_werror" != "xno"], [
	CFLAGS="$CFLAGS -Werror"
	CXXFLAGS="$CXXFLAGS -Werror"
])
# Courtesy of Glib: Ensure MSVC-compatible struct packing convention
# is used when compiling for Win32 with gcc.
AS_IF([test "x$native_win32" = "xyes"], [
	AC_CHECK_TOOL(WINDRES, windres)
	AS_IF([test x"$GCC" = xyes], [
		AC_MSG_CHECKING([how to get MSVC-compatible struct packing])
		AS_IF([test -z "$ac_cv_prog_CC"], [
			our_gcc="$CC"
		], [
			our_gcc="$ac_cv_prog_CC"
		])
		AS_IF([$our_gcc -v --help 2>/dev/null | grep ms-bitfields >/dev/null], [
			msnative_struct="-mms-bitfields"
			CFLAGS="$CFLAGS $msnative_struct"
			CXXFLAGS="$CXXFLAGS $msnative_struct"
			AC_MSG_RESULT([${msnative_struct}])
		], [
			AC_MSG_RESULT([no way])
			AC_MSG_WARN([produced libraries might be incompatible with MSVC-compiled code])
		])
	])
	CFLAGS="$CFLAGS -Wall -Wextra -pedantic"
	CXXFLAGS="$CXXFLAGS -Wall -Wextra -Wshadow -pedantic"
], [
	AS_IF([test x"$GCC" = xyes], [
		# Be tough with warnings and produce less careless code
		CFLAGS="$CFLAGS -Wall -Wextra -pedantic"
		CXXFLAGS="$CXXFLAGS -Wall -Wextra -Wshadow -pedantic -Weffc++"
	])
])
AC_SUBST(DEBUG_CXXFLAGS)

# ============
# Debug switch
# ============
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug], [Turn on debugging])],
	[enable_debug="$enableval"],
	[enable_debug=no]
)
AS_IF([test "x$enable_debug" = "xyes"], [
	DEBUG_CXXFLAGS="-DDEBUG -g"
], [
	DEBUG_CXXFLAGS="-DNDEBUG"
])
AC_SUBST(DEBUG_CXXFLAGS)

# ============
# Static tools
# ============
AC_ARG_ENABLE([static-tools],
	[AS_HELP_STRING([--enable-static-tools], [Link tools (binaries) statically])],
	[enable_static_tools="$enableval"],
	[enable_static_tools=no]
)
AS_IF([test "x$enable_static_tools" = "xyes"], [
	enable_static="yes"
], [
	AC_DISABLE_STATIC
])
AM_CONDITIONAL(STATIC_TOOLS, [test "x$enable_static_tools" = "xyes"])

# =============
# Documentation
# =============
AC_ARG_WITH(docs,
	[AS_HELP_STRING([--without-docs], [Do not build documentation])],
	[with_docs="$withval"],
	[AS_IF([test "x$native_win32" = "xyes"], [with_docs=no], [with_docs=yes])]
)
AS_IF([test "x$with_docs" != "xno"], [
	AC_PATH_PROG(DOXYGEN, [doxygen])
	AS_IF([test -z "$DOXYGEN"], [
		AC_MSG_WARN([*** Could not find doxygen in your PATH.])
		AC_MSG_WARN([*** The documentation will not be built.])
		build_docs=no
	], [build_docs=yes])
], [build_docs=no])
AM_CONDITIONAL([WITH_LIBWPD_DOCS], [test "x$build_docs" != "xno"])


# =========
# Streaming
# =========
AC_ARG_WITH([stream],
	[AS_HELP_STRING([--without-stream], [Do not build the input stream implementation])],
	[with_stream="$withval"],
	[with_stream=yes]
)
AM_CONDITIONAL(WITH_LIBWPD_STREAM, [test "x$with_stream" != "xno"])

# =========
# Find zlib
# =========
if test "x$with_stream" != "xno"; then
PKG_CHECK_MODULES([ZLIB],[zlib],[],[
    AC_CHECK_HEADER(zlib.h, [ZLIB_CFLAGS=],
        [AC_MSG_ERROR(zlib.h not found. install zlib)], [])
    AC_CHECK_LIB(z, inflate, [ ZLIB_LIBS=-lz ],
        [AC_MSG_ERROR(zlib not found or functional)], [])
])
else
ZLIB_CFLAGS=
ZLIB_LIBS=
fi
AC_SUBST(ZLIB_CFLAGS)
AC_SUBST(ZLIB_LIBS)


# =====================
# Prepare all .in files
# =====================
AC_CONFIG_FILES([
Makefile
src/Makefile
src/conv/Makefile
src/conv/html/Makefile
src/conv/html/wpd2html.rc
src/conv/raw/Makefile
src/conv/raw/wpd2raw.rc
src/conv/text/Makefile
src/conv/text/wpd2text.rc
src/lib/Makefile
src/lib/libwpd.h
src/lib/libwpd.rc
src/lib/libwpd-stream.h
src/lib/libwpd-stream.rc
src/test/Makefile
docs/Makefile
docs/doxygen/Makefile
build/Makefile
build/win32/Makefile
libwpd-0.9.pc
libwpd-stream-0.9.pc
libwpd.spec
libwpd-zip
])
AC_OUTPUT

# ==============================================
# Display final informations about configuration
# ==============================================
AC_MSG_NOTICE([
==============================================================================
Build configuration:
	debug:           ${enable_debug}
	docs:            ${build_docs}
	stream:          ${with_stream}
	static-tools:    ${enable_static_tools}
	werror:          ${enable_werror}
==============================================================================
])
