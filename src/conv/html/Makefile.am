EXTRA_DIST =			\
	wpd2html.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = 		\
	wpd2html.rc	 

bin_PROGRAMS = wpd2html

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I../../lib/

wpd2html_LDFLAGS = -L../../lib/
wpd2html_DEPENDENCIES = @WPD2HTML_WIN32_RESOURCE@
wpd2html_LDADD = -lwpd-1 $(LIBWPD_LIBS) @WPD2HTML_WIN32_RESOURCE@

wpd2html_SOURCES =			\
	HtmlListenerImpl.cpp	 	\
	HtmlListenerImpl.h	 	\
	main.cpp

if OS_WIN32

@WPD2HTML_WIN32_RESOURCE@ : wpd2html.rc
	$(top_srcdir)/build/win32/lt-compile-resource wpd2html.rc @WPD2HTML_WIN32_RESOURCE@
endif
