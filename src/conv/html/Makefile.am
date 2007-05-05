if WITH_LIBWPD_STREAM
target_wpd2html = wpd2html
else
target_wpd2html =
endif

bin_PROGRAMS = $(target_wpd2html)

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I$(top_srcdir)/src/lib

wpd2html_LDFLAGS = -L../../lib
wpd2html_DEPENDENCIES = @WPD2HTML_WIN32_RESOURCE@
wpd2html_LDADD = -lwpd-0.8 -lwpd-stream-0.8 $(LIBWPD_LIBS) @WPD2HTML_WIN32_RESOURCE@

wpd2html_SOURCES = \
	HtmlListenerImpl.cpp	\
	HtmlListenerImpl.h	\
	wpd2html.cpp

if OS_WIN32

@WPD2HTML_WIN32_RESOURCE@ : wpd2html.rc
	$(top_srcdir)/build/win32/lt-compile-resource wpd2html.rc @WPD2HTML_WIN32_RESOURCE@
endif

# Include the wpd2html_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(wpd2html_SOURCES)	\
	wpd2html.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	wpd2html.rc	 

