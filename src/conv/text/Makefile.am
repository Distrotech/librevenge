EXTRA_DIST =			\
	wpd2text.rc.in

# These may be in the builddir too
BUILD_EXTRA_DIST = 		\
	wpd2text.rc	 
 
bin_PROGRAMS = wpd2text

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I../../lib/

wpd2text_LDFLAGS = -L../../lib/
wpd2text_DEPENDENCIES = @WPD2TEXT_WIN32_RESOURCE@ 
wpd2text_LDADD = -lwpd-1 $(LIBWPD_LIBS) @WPD2TEXT_WIN32_RESOURCE@ 

wpd2text_SOURCES =			\
	TextListenerImpl.cpp	 	\
	TextListenerImpl.h	 	\
	main.cpp

if OS_WIN32

@WPD2TEXT_WIN32_RESOURCE@ : wpd2text.rc
	$(top_srcdir)/build/win32/lt-compile-resource wpd2text.rc @WPD2TEXT_WIN32_RESOURCE@
endif
