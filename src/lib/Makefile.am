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

lib_LTLIBRARIES = \
	librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la

librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_includedir = $(includedir)/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@/librevenge
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_include_HEADERS = \
	$(top_srcdir)/inc/librevenge/librevenge.h \
	$(top_srcdir)/inc/librevenge/RVNGBinaryData.h \
	$(top_srcdir)/inc/librevenge/RVNGDrawingInterface.h \
	$(top_srcdir)/inc/librevenge/RVNGPresentationInterface.h \
	$(top_srcdir)/inc/librevenge/RVNGProperty.h \
	$(top_srcdir)/inc/librevenge/RVNGPropertyList.h \
	$(top_srcdir)/inc/librevenge/RVNGPropertyListVector.h \
	$(top_srcdir)/inc/librevenge/RVNGString.h \
	$(top_srcdir)/inc/librevenge/RVNGStringVector.h \
	$(top_srcdir)/inc/librevenge/RVNGTextInterface.h

AM_CXXFLAGS = -I$(top_srcdir)/inc $(DEBUG_CXXFLAGS) $(ZLIB_CFLAGS)

librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LIBADD  = @LIBREVENGE_WIN32_RESOURCE@
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_DEPENDENCIES = @LIBREVENGE_WIN32_RESOURCE@
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined)
librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_SOURCES = \
	RVNGBinaryData.cpp \
	RVNGMemoryStream.cpp \
	RVNGProperty.cpp \
	RVNGPropertyList.cpp \
	RVNGPropertyListVector.cpp \
	RVNGString.cpp \
	RVNGStringVector.cpp \
	librevenge_internal.h \
	RVNGMemoryStream.h

librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_includedir = $(includedir)/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@/librevenge-stream
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_include_HEADERS = \
	$(top_srcdir)/inc/librevenge-stream/librevenge-stream.h \
	$(top_srcdir)/inc/librevenge-stream/RVNGStream.h \
	$(top_srcdir)/inc/librevenge-stream/RVNGStreamImplementation.h

librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LIBADD  = $(ZLIB_LIBS) @LIBREVENGE_STREAM_WIN32_RESOURCE@
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_DEPENDENCIES = @LIBREVENGE_STREAM_WIN32_RESOURCE@
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_LDFLAGS = $(version_info) -export-dynamic $(no_undefined)
librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_SOURCES = \
	RVNGStreamImplementation.cpp \
	RVNGOLEStream.cpp \
	RVNGZipStream.cpp \
	RVNGOLEStream.h \
	RVNGZipStream.h

if OS_WIN32

@LIBREVENGE_WIN32_RESOURCE@ : librevenge.rc $(librevenge_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource librevenge.rc @LIBREVENGE_WIN32_RESOURCE@

@LIBREVENGE_STREAM_WIN32_RESOURCE@ : librevenge-stream.rc $(librevenge_stream_@RVNG_MAJOR_VERSION@_@RVNG_MINOR_VERSION@_la_OBJECTS)
	chmod +x $(top_srcdir)/build/win32/*compile-resource && \
	WINDRES=@WINDRES@ $(top_srcdir)/build/win32/lt-compile-resource librevenge-stream.rc @LIBREVENGE_STREAM_WIN32_RESOURCE@
endif


EXTRA_DIST = \
	librevenge.rc.in \
	librevenge-stream.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	librevenge.rc \
	librevenge-stream.rc
