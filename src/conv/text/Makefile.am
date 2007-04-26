if WITH_LIBWPD_STREAM
target_wpd2text = wpd2text
else
target_wpd2text =
endif

bin_PROGRAMS = $(target_wpd2text)

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I$(top_srcdir)/src/lib

wpd2text_LDFLAGS = -L../../lib
wpd2text_DEPENDENCIES = @WPD2TEXT_WIN32_RESOURCE@ 
wpd2text_LDADD = -lwpd-0.8 -lwpd-stream-0.8 $(LIBWPD_LIBS) @WPD2TEXT_WIN32_RESOURCE@ 
wpd2text_SOURCES = \
	TextListenerImpl.cpp \
	TextListenerImpl.h \
	main.cpp

if OS_WIN32

@WPD2TEXT_WIN32_RESOURCE@ : wpd2text.rc
	$(top_srcdir)/build/win32/lt-compile-resource wpd2text.rc @WPD2TEXT_WIN32_RESOURCE@
endif

# Include the wpd2text_SOURCES in case we build a tarball without stream
EXTRA_DIST = \
	$(wpd2text_SOURCES)	\
	wpd2text.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = \
	wpd2text.rc	 
 
