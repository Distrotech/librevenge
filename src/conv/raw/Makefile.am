EXTRA_DIST =			\
	wpd2raw.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = 		\
	wpd2raw.rc	 
 
bin_PROGRAMS = wpd2raw

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I../../lib/

wpd2raw_LDFLAGS = -L../../lib/
wpd2raw_DEPENDENCIES = @WPD2RAW_WIN32_RESOURCE@
wpd2raw_LDADD = -lwpd-1 $(LIBWPD_LIBS) @WPD2RAW_WIN32_RESOURCE@

wpd2raw_SOURCES =			\
	RawListener.cpp	 	\
	RawListener.h	 	\
	main.cpp

if OS_WIN32

@WPD2RAW_WIN32_RESOURCE@ : wpd2raw.rc
	$(top_srcdir)/build/win32/lt-compile-resource wpd2raw.rc @WPD2RAW_WIN32_RESOURCE@
endif
