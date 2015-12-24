target_test = test

check_PROGRAMS = $(target_test) testfileinfo

AM_CXXFLAGS = -I$(top_srcdir)/inc -I$(top_srcdir)/src/lib $(CPPUNIT_CFLAGS) $(DEBUG_CXXFLAGS)

test_CPPFLAGS = \
	-DRVNG_DATA_DIR=\"$(top_srcdir)/src/test/data\" \
	-DRVNG_DIRECTORY_STREAM_TEST_DIR=\"$(top_srcdir)/inc/librevenge-stream\"
test_LDADD = \
	$(top_builddir)/src/lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	$(top_builddir)/src/lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	$(CPPUNIT_LIBS) \
	$(LIBREVENGE_LIBS)

test_SOURCES = \
	RVNGBinaryDataTest.cpp \
	RVNGDirectoryStreamTest.cpp \
	RVNGOLEStreamTest.cpp \
	RVNGStreamTest.cpp \
	RVNGStringTest.cpp \
	test.cpp
	

testfileinfo_LDADD = \
	$(top_builddir)/src/lib/librevenge-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	$(top_builddir)/src/lib/librevenge-stream-@RVNG_MAJOR_VERSION@.@RVNG_MINOR_VERSION@.la \
	$(LIBREVENGE_LIBS)

testfileinfo_SOURCES = \
	RVNGFileInfo.cpp
	
EXTRA_DIST = \
	data/test.ole

TESTS = $(target_test)
