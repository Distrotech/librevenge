if WITH_LIBREVENGE_STREAM
target_wpd2html = wpd2html
else
target_wpd2html =
endif

bin_PROGRAMS = $(target_wpd2html)

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

wpd2html_DEPENDENCIES = @RVNG2HTML_WIN32_RESOURCE@

if STATIC_TOOLS
wpd2html_LDADD = ../../lib/@RVNG_OBJDIR@/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a ../../lib/@RVNG_OBJDIR@/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a @RVNG2HTML_WIN32_RESOURCE@
wpd2html_LDFLAGS = -all-static
else
wpd2html_LDADD = ../../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la ../../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la @RVNG2HTML_WIN32_RESOURCE@
endif

wpd2html_SOURCES = \
	HtmlDocumentGenerator.cpp	\
	HtmlDocumentGenerator.h	\
	wpd2html.cpp

if OS_WIN32

@RVNG2HTML_WIN32_RESOURCE@ : wpd2html.rc $(wpd2html_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource wpd2html.rc @RVNG2HTML_WIN32_RESOURCE@
endif

# Include the wpd2html_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(wpd2html_SOURCES)	\
	wpd2html.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	wpd2html.rc	 

