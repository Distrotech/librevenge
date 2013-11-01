if WITH_LIBREVENGE_STREAM
target_rvng2text = rvng2text
else
target_rvng2text =
endif

bin_PROGRAMS = $(target_rvng2text)

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

rvng2text_DEPENDENCIES = @RVNG2TEXT_WIN32_RESOURCE@ 

if STATIC_TOOLS
rvng2text_LDADD = ../../lib/@RVNG_OBJDIR@/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a ../../lib/@RVNG_OBJDIR@/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.a @RVNG2TEXT_WIN32_RESOURCE@
rvng2text_LDFLAGS = -all-static
else
rvng2text_LDADD = ../../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la ../../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la @RVNG2TEXT_WIN32_RESOURCE@
endif
 
rvng2text_SOURCES = \
	TextDocumentGenerator.cpp \
	TextDocumentGenerator.h \
	rvng2text.cpp

if OS_WIN32

@RVNG2TEXT_WIN32_RESOURCE@ : rvng2text.rc $(rvng2text_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource rvng2text.rc @RVNG2TEXT_WIN32_RESOURCE@
endif

# Include the rvng2text_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(rvng2text_SOURCES) \
	rvng2text.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	rvng2text.rc	 
 
