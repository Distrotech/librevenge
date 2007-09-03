target_test = test

noinst_PROGRAMS = $(target_test)

AM_CXXFLAGS = $(LIBWPD_CXXFLAGS) $(DEBUG_CXXFLAGS) -I$(top_srcdir)/src/lib/

test_LDFLAGS = -L../lib/ `cppunit-config --libs` 
test_LDADD = \
	-lwpd-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@ \
	-lwpd-stream-@WPD_MAJOR_VERSION@.@WPD_MINOR_VERSION@ \
	$(LIBWPD_LIBS) 

test_SOURCES = \
	test.cpp

.PHONY:		check

check: test
	./test
