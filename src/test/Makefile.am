target_test = test

noinst_PROGRAMS = $(target_test)

AM_CXXFLAGS = -I$(top_srcdir)/src/lib/ $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS)

test_LDFLAGS = -L../lib `cppunit-config --libs` 
test_LDADD = \
	../lib/libwpd-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.la \
	../lib/libwpd-stream-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@.la \
	$(LIBWPD_LIBS) 

test_SOURCES = \
	test.cpp

.PHONY:		check

check: test
	./test
