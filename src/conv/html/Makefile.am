if WITH_LIBREVENGE_STREAM
target_rvng2html = rvng2html
else
target_rvng2html =
endif

bin_PROGRAMS = $(target_rvng2html)

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

rvng2html_DEPENDENCIES = @RVNG2HTML_WIN32_RESOURCE@

if STATIC_TOOLS
rvng2html_LDADD = ../../lib/@RVNG_OBJDIR@/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a ../../lib/@RVNG_OBJDIR@/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a @RVNG2HTML_WIN32_RESOURCE@
rvng2html_LDFLAGS = -all-static
else
rvng2html_LDADD = ../../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la ../../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la @RVNG2HTML_WIN32_RESOURCE@
endif

rvng2html_SOURCES = \
	HtmlDocumentGenerator.cpp	\
	HtmlDocumentGenerator.h	\
	rvng2html.cpp

if OS_WIN32

@RVNG2HTML_WIN32_RESOURCE@ : rvng2html.rc $(rvng2html_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource rvng2html.rc @RVNG2HTML_WIN32_RESOURCE@
endif

# Include the rvng2html_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(rvng2html_SOURCES)	\
	rvng2html.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	rvng2html.rc	 

