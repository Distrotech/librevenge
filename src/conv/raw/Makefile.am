if WITH_LIBREVENGE_STREAM
target_wpd2raw = wpd2raw
else
target_wpd2raw =
endif

bin_PROGRAMS = $(target_wpd2raw)

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

wpd2raw_DEPENDENCIES = @RVNG2RAW_WIN32_RESOURCE@

if STATIC_TOOLS
wpd2raw_LDADD = ../../lib/@RVNG_OBJDIR@/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a ../../lib/@RVNG_OBJDIR@/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a @RVNG2RAW_WIN32_RESOURCE@
wpd2raw_LDFLAGS = -all-static
else
wpd2raw_LDADD = ../../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la ../../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la @RVNG2RAW_WIN32_RESOURCE@
endif

wpd2raw_SOURCES = \
	RawDocumentGenerator.cpp	 \
	RawDocumentGenerator.h	 \
	wpd2raw.cpp

if OS_WIN32

@RVNG2RAW_WIN32_RESOURCE@ : wpd2raw.rc $(wpd2raw_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource wpd2raw.rc @RVNG2RAW_WIN32_RESOURCE@
endif

# Include the wpd2raw_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(wpd2raw_SOURCES)	\
	wpd2raw.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	wpd2raw.rc	 
 
