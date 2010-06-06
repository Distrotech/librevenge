/* libwps
 * Copyright (C) 2006 Andrew Ziem
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>

#include "libwpd_internal.h"
#include "WPXStreamImplementation.h"
#include "WPXMemoryStream.h"


#define TMP_FILENAME "libwpd-unittest.tmp"

class Test : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(Test);
  CPPUNIT_TEST(testStream);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp(void);
  void tearDown(void);

protected:
  void testStream(void);

};

void Test::setUp(void)
{
	FILE *f = fopen(TMP_FILENAME, "w");
	fprintf(f, "%c%c%c%c",1,2,3,4);
	fprintf(f, "%c%c%c%c",0,5,6,7);
	fclose(f);
}

void Test::tearDown(void)
{
	unlink(TMP_FILENAME);
}

void Test::testStream(void)
{ 
	WPXInputStream* input = 0;
	unsigned long numBytesRead;
	unsigned long u32;

	/********************** 
	 * Test WPXFileStream *
	 **********************/
	input = new WPXFileStream(TMP_FILENAME);

	CPPUNIT_ASSERT_EQUAL ( false, input->isOLEStream() );
	CPPUNIT_ASSERT_EQUAL ( (WPXInputStream*) NULL, input->getDocumentOLEStream("foo") );

	// test read()
	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT ( NULL == input->read(0, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (unsigned long)0, numBytesRead );
	CPPUNIT_ASSERT_EQUAL ( (long) 0 , input->tell() );
	CPPUNIT_ASSERT ( NULL != input->read(1, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (long) 1 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT ( NULL != input->read(50, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (long) 8 , input->tell() );

	// test readU*()
	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 1 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 2 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 3 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 4 , readU8(input, 0) );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL( (uint16_t) 0x0201 , readU16(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint16_t) 0x0403 , readU16(input, 0) );

	input->seek(0, WPX_SEEK_SET);
	u32 = readU32(input, 0);
	CPPUNIT_ASSERT_EQUAL( (unsigned long) 0x04030201 , u32 );
	u32 = readU32(input, 0);
	CPPUNIT_ASSERT_EQUAL( (unsigned long) 0x07060500 , u32 );

	// test seek(), tell(), atEOS()
	input->seek(1, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 1 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 0 , input->tell() );

	input->seek(8, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 8 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	for (int i = 0; i < 8; i++)
		readU8(input, 0);
	CPPUNIT_ASSERT_EQUAL ( true, input->atEOS() );

	CPPUNIT_ASSERT_THROW ( readU8(input, 0), FileException );

	input->seek(-1, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 0, input->tell() );

	input->seek(8, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( true, input->atEOS() );
	
	input->seek(10000, WPX_SEEK_SET);
	CPPUNIT_ASSERT( 10000 != input->tell() );
	CPPUNIT_ASSERT( input->atEOS() );

	delete input;

	/************************ 
	 * Test WPXStringStream *
	 ************************/
	input = new WPXStringStream((const unsigned char *)"\1\2\3\4\0\5\6\7", 8);

	CPPUNIT_ASSERT_EQUAL ( false, input->isOLEStream() );
	CPPUNIT_ASSERT_EQUAL ( (WPXInputStream*) NULL, input->getDocumentOLEStream("foo") );

	// test read()
	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT ( NULL == input->read(0, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (unsigned long) 0, numBytesRead );
	CPPUNIT_ASSERT_EQUAL ( (long) 0 , input->tell() );
	CPPUNIT_ASSERT ( NULL != input->read(1, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (long) 1 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT ( NULL != input->read(50, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (long) 8 , input->tell() );

	// test readU*()
	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 1 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 2 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 3 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 4 , readU8(input, 0) );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL( (uint16_t) 0x0201 , readU16(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint16_t) 0x0403 , readU16(input, 0) );

	input->seek(0, WPX_SEEK_SET);
	u32 = readU32(input, 0);
	CPPUNIT_ASSERT_EQUAL( (unsigned long) 0x04030201 , u32 );
	u32 = readU32(input, 0);
	CPPUNIT_ASSERT_EQUAL( (unsigned long) 0x07060500 , u32 );

	// test seek(), tell(), atEOS()
	input->seek(1, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 1 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 0 , input->tell() );

	input->seek(8, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 8 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	for (int i = 0; i < 8; i++)
		readU8(input, 0);
	CPPUNIT_ASSERT_EQUAL ( true, input->atEOS() );

	CPPUNIT_ASSERT_THROW ( readU8(input, 0), FileException );

	input->seek(-1, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 0, input->tell() );

	input->seek(8, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( true, input->atEOS() );
	
	input->seek(10000, WPX_SEEK_SET);
	CPPUNIT_ASSERT( 10000 != input->tell() );
	CPPUNIT_ASSERT( input->atEOS() );

	delete input;


	/************************ 
	 * Test WPXMemoryInputStream *
	 ************************/
	input = new WPXMemoryInputStream((uint8_t *)("\1\2\3\4\0\5\6\7"), 8);

	CPPUNIT_ASSERT_EQUAL ( false, input->isOLEStream() );
	CPPUNIT_ASSERT_EQUAL ( (WPXInputStream*) NULL, input->getDocumentOLEStream("foo") );

	// test read()
	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT ( NULL == input->read(0, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (unsigned long) 0, numBytesRead );
	CPPUNIT_ASSERT_EQUAL ( (long) 0 , input->tell() );
	CPPUNIT_ASSERT ( NULL != input->read(1, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (long) 1 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT ( NULL != input->read(50, numBytesRead)  );
	CPPUNIT_ASSERT_EQUAL ( (long) 8 , input->tell() );

	// test readU*()
	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 1 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 2 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 3 , readU8(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint8_t) 4 , readU8(input, 0) );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL( (uint16_t) 0x0201 , readU16(input, 0) );
	CPPUNIT_ASSERT_EQUAL( (uint16_t) 0x0403 , readU16(input, 0) );

	input->seek(0, WPX_SEEK_SET);
	u32 = readU32(input, 0);
	CPPUNIT_ASSERT_EQUAL( (unsigned long) 0x04030201 , u32 );
	u32 = readU32(input, 0);
	CPPUNIT_ASSERT_EQUAL( (unsigned long) 0x07060500 , u32 );

	// test seek(), tell(), atEOS()
	input->seek(1, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 1 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 0 , input->tell() );

	input->seek(8, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 8 , input->tell() );

	input->seek(0, WPX_SEEK_SET);
	for (int i = 0; i < 8; i++)
		readU8(input, 0);
	CPPUNIT_ASSERT_EQUAL ( true, input->atEOS() );

	CPPUNIT_ASSERT_THROW ( readU8(input, 0), FileException );

	input->seek(-1, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( (long) 0, input->tell() );

	input->seek(8, WPX_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL ( true, input->atEOS() );
	
	input->seek(10000, WPX_SEEK_SET);
	CPPUNIT_ASSERT( 10000 != input->tell() );
	CPPUNIT_ASSERT( input->atEOS() );

	delete input;


}

CPPUNIT_TEST_SUITE_REGISTRATION(Test);

int main( int argc, char **argv )
{
	// Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	// Add a listener that colllects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener( &result );        

	// Add a listener that print dots as test run.
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener( &progress );      

	// Add the top suite to the test runner
	CPPUNIT_NS::TestRunner runner;
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
	runner.run( controller );

	// output
	CPPUNIT_NS::CompilerOutputter outputter( &result, std::cerr );
	outputter.write();

	// return status code
	return result.wasSuccessful() ? 0 : 1;
}
