if PLATFORM_WIN32
no_undefined = -no-undefined
else
if PLATFORM_DARWIN
no_undefined = -no-undefined
else
no_undefined =
endif
endif

if OS_WIN32
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) -avoid-version
else
version_info = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)
endif

if DEF_FILES
export_symbols_librevenge = -export-symbols librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def
export_symbols_librevenge_stream = -export-symbols librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def
else
export_symbols_librevenge =
export_symbols_librevenge_stream =
endif

if WITH_LIBREVENGE_STREAM
target_librevenge_stream = librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la
else
target_librevenge_stream =
endif

lib_LTLIBRARIES = librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la $(target_librevenge_stream)
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_includedir = $(includedir)/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@/librevenge
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_include_HEADERS = \
	$(top_srcdir)/inc/librevenge/librevenge.h \
	$(top_srcdir)/inc/librevenge/RVNGBinaryData.h \
	$(top_srcdir)/inc/librevenge/RVNGTextInterface.h \
	$(top_srcdir)/inc/librevenge/RVNGProperty.h \
	$(top_srcdir)/inc/librevenge/RVNGPropertyList.h \
	$(top_srcdir)/inc/librevenge/RVNGPropertyListVector.h \
	$(top_srcdir)/inc/librevenge/RVNGString.h

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS)

librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LIBADD  = @LIBREVENGE_WIN32_RESOURCE@
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_DEPENDENCIES = @LIBREVENGE_WIN32_RESOURCE@
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined) $(export_symbols_librevenge)
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_SOURCES = \
	librevenge_math.cpp \
	RVNGBinaryData.cpp \
	RVNGMemoryStream.cpp \
	RVNGProperty.cpp \
	RVNGPropertyList.cpp \
	RVNGPropertyListVector.cpp \
	RVNGString.cpp \
	librevenge_math.h \
	librevenge_types.h \
	RVNGMemoryStream.h

librevenge_stream_headers = \
	$(top_srcdir)/inc/librevenge-stream/librevenge-stream.h \
	$(top_srcdir)/inc/librevenge-stream/RVNGStream.h \
	$(top_srcdir)/inc/librevenge-stream/RVNGStreamImplementation.h

librevenge_stream_sources = \
	RVNGStreamImplementation.cpp \
	RVNGOLEStream.cpp \
	RVNGZipStream.cpp \
	RVNGOLEStream.h \
	RVNGZipStream.h

if WITH_LIBREVENGE_STREAM

librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_includedir = $(includedir)/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@/librevenge-stream
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_include_HEADERS = $(librevenge_stream_headers)

librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LIBADD  = $(ZLIB_LIBS) @LIBREVENGE_STREAM_WIN32_RESOURCE@
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_DEPENDENCIES = @LIBREVENGE_STREAM_WIN32_RESOURCE@
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined) $(export_symbols_librevenge_stream)
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_SOURCES = $(librevenge_stream_sources)

endif

if OS_WIN32

@LIBREVENGE_WIN32_RESOURCE@ : librevenge.rc $(librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource librevenge.rc @LIBREVENGE_WIN32_RESOURCE@

@LIBREVENGE_STREAM_WIN32_RESOURCE@ : librevenge-stream.rc $(librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource librevenge-stream.rc @LIBREVENGE_STREAM_WIN32_RESOURCE@
endif


# Include the GSFStream.* in case we build a tarball without stream
EXTRA_DIST = \
	$(librevenge_stream_sources) \
	$(librevenge_stream_headers) \
	makefile.mk \
	librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def \
	librevenge.rc.in \
	librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.def \
	librevenge-stream.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	librevenge.rc \
	librevenge-stream.rc
