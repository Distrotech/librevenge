target_test = test

check_PROGRAMS = $(target_test) testfileinfo

AM_CXXFLAGS = -I$(top_srcdir)/inc -I$(top_srcdir)/src/lib $(CPPUNIT_CFLAGS) $(DEBUG_CXXFLAGS)

test_CPPFLAGS = -DRVNG_DIRECTORY_STREAM_TEST_DIR=\"$(top_srcdir)/inc/librevenge-stream\"
test_LDFLAGS = -L../lib
test_LDADD = \
	../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	$(CPPUNIT_LIBS) \
	$(LIBREVENGE_LIBS)

test_SOURCES = \
	RVNGBinaryDataTest.cpp \
	RVNGBinaryDataTest.h \
	RVNGDirectoryStreamTest.cpp \
	RVNGDirectoryStreamTest.h \
	RVNGStreamTest.cpp \
	RVNGStreamTest.h \
	RVNGStringTest.cpp \
	test.cpp
	

testfileinfo_LDFLAGS = -L../lib
testfileinfo_LDADD = \
	../lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	../lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	$(LIBREVENGE_LIBS)

testfileinfo_SOURCES = \
	RVNGFileInfo.cpp
	


TESTS = $(target_test)
