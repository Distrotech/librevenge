if WITH_LIBWPD_STREAM
target_wpd2raw = wpd2raw
else
target_wpd2raw =
endif

bin_PROGRAMS = $(target_wpd2raw)

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I../../lib/

wpd2raw_LDFLAGS = -L../../lib/
wpd2raw_DEPENDENCIES = @WPD2RAW_WIN32_RESOURCE@
wpd2raw_LDADD = -lwpd-0.8 -lwpd-stream-0.8 $(LIBWPD_LIBS) @WPD2RAW_WIN32_RESOURCE@

wpd2raw_SOURCES = \
	RawListener.cpp	 \
	RawListener.h	 \
	main.cpp

if OS_WIN32

@WPD2RAW_WIN32_RESOURCE@ : wpd2raw.rc
	$(top_srcdir)/build/win32/lt-compile-resource wpd2raw.rc @WPD2RAW_WIN32_RESOURCE@
endif

# Include the wpd2raw_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(wpd2raw_SOURCES)	\
	wpd2raw.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	wpd2raw.rc	 
 
