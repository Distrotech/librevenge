target_test = test

check_PROGRAMS = $(target_test)

AM_CXXFLAGS = -I$(top_srcdir)/inc -I$(top_srcdir)/src/lib $(DEBUG_CXXFLAGS)

test_LDFLAGS = -L../lib `cppunit-config --libs` 
test_LDADD = \
	../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	$(LIBREVENGE_LIBS) 

test_SOURCES = \
	RVNGBinaryDataTest.cpp \
	RVNGBinaryDataTest.h \
	RVNGStreamTest.cpp \
	RVNGStreamTest.h \
	test.cpp

TESTS = $(target_test)
