if WITH_LIBREVENGE_STREAM
target_wpd2text = wpd2text
else
target_wpd2text =
endif

bin_PROGRAMS = $(target_wpd2text)

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

wpd2text_DEPENDENCIES = @RVNG2TEXT_WIN32_RESOURCE@ 

if STATIC_TOOLS
wpd2text_LDADD = ../../lib/@RVNG_OBJDIR@/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a ../../lib/@RVNG_OBJDIR@/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a @RVNG2TEXT_WIN32_RESOURCE@
wpd2text_LDFLAGS = -all-static
else
wpd2text_LDADD = ../../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la ../../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la @RVNG2TEXT_WIN32_RESOURCE@
endif
 
wpd2text_SOURCES = \
	TextDocumentGenerator.cpp \
	TextDocumentGenerator.h \
	wpd2text.cpp

if OS_WIN32

@RVNG2TEXT_WIN32_RESOURCE@ : wpd2text.rc $(wpd2text_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource wpd2text.rc @RVNG2TEXT_WIN32_RESOURCE@
endif

# Include the wpd2text_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(wpd2text_SOURCES) \
	wpd2text.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	wpd2text.rc	 
 
