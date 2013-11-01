if WITH_LIBREVENGE_STREAM
target_rvng2raw = rvng2raw
else
target_rvng2raw =
endif

bin_PROGRAMS = $(target_rvng2raw)

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

rvng2raw_DEPENDENCIES = @RVNG2RAW_WIN32_RESOURCE@

if STATIC_TOOLS
rvng2raw_LDADD = ../../lib/@RVNG_OBJDIR@/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a ../../lib/@RVNG_OBJDIR@/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a @RVNG2RAW_WIN32_RESOURCE@
rvng2raw_LDFLAGS = -all-static
else
rvng2raw_LDADD = ../../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la ../../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la @RVNG2RAW_WIN32_RESOURCE@
endif

rvng2raw_SOURCES = \
	RawDocumentGenerator.cpp	 \
	RawDocumentGenerator.h	 \
	rvng2raw.cpp

if OS_WIN32

@RVNG2RAW_WIN32_RESOURCE@ : rvng2raw.rc $(rvng2raw_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource rvng2raw.rc @RVNG2RAW_WIN32_RESOURCE@
endif

# Include the rvng2raw_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(rvng2raw_SOURCES)	\
	rvng2raw.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	rvng2raw.rc	 
 
