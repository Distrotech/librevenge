target_test = test

check_PROGRAMS = $(target_test)

AM_CXXFLAGS = -I$(top_srcdir)/inc -I$(top_srcdir)/src/lib $(DEBUG_CXXFLAGS)

test_LDFLAGS = -L../lib `cppunit-config --libs` 
test_LDADD = \
	../lib/libwpd-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.la \
	../lib/libwpd-stream-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.la \
	$(LIBWPD_LIBS) 

test_SOURCES = \
	test.cpp

TESTS = $(target_test)
