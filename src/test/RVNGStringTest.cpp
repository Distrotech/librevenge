/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGSTRINGTEST_H
#define RVNGSTRINGTEST_H

#include <algorithm>
#include <cstring>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <librevenge/librevenge.h>

#include "librevenge_internal.h"

namespace test
{

using librevenge::RVNGString;

using std::equal;
using std::size_t;
using std::strlen;

namespace
{

void implTestEscapeXML(const char *const input, const char *const expected)
{
	const size_t len = strlen(expected);

	{
		// appending a C string
		RVNGString str;
		str.appendEscapedXML(input);
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), str.size());
		CPPUNIT_ASSERT_EQUAL(int(len), str.len());
		CPPUNIT_ASSERT(equal(expected, expected + len, str.cstr()));
	}

	{
		// appending a RVNGString
		RVNGString str;
		str.appendEscapedXML(RVNGString(input));
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), str.size());
		CPPUNIT_ASSERT_EQUAL(int(len), str.len());
		CPPUNIT_ASSERT(equal(expected, expected + len, str.cstr()));
	}

	{
		// creating from a C string
		RVNGString str(RVNGString::escapeXML(input));
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), str.size());
		CPPUNIT_ASSERT_EQUAL(int(len), str.len());
		CPPUNIT_ASSERT(equal(expected, expected + len, str.cstr()));
	}

	{
		// creating from a RVNGString
		RVNGString str(RVNGString::escapeXML(RVNGString(input)));
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), str.size());
		CPPUNIT_ASSERT_EQUAL(int(len), str.len());
		CPPUNIT_ASSERT(equal(expected, expected + len, str.cstr()));
	}
}

}

class RVNGStringTest : public CPPUNIT_NS::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();

private:
	CPPUNIT_TEST_SUITE(RVNGStringTest);
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testAppend);
	CPPUNIT_TEST(testAppendEscapedXML);
	CPPUNIT_TEST(testClear);
	CPPUNIT_TEST_SUITE_END();

private:
	void testConstruction();
	void testAppend();
	void testAppendEscapedXML();
	void testClear();
};

void RVNGStringTest::setUp()
{
}

void RVNGStringTest::tearDown()
{
}

void RVNGStringTest::testConstruction()
{
	// default ctor creates empty string
	RVNGString empty;
	CPPUNIT_ASSERT_EQUAL(0ul, empty.size());
	CPPUNIT_ASSERT_EQUAL(0, empty.len());

	// construction from a C string
	const char input[] = "hello world";
	const size_t len = RVNG_NUM_ELEMENTS(input) - 1;
	RVNGString str(input);
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), str.size());
	CPPUNIT_ASSERT_EQUAL(int(len), str.len());
	CPPUNIT_ASSERT(equal(input, input + len, str.cstr()));

	// copy construction
	RVNGString copy(str);
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), copy.size());
	CPPUNIT_ASSERT_EQUAL(int(len), copy.len());
	CPPUNIT_ASSERT(equal(input, input + len, copy.cstr()));

	// assignment
	RVNGString assign;
	assign = str;
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), assign.size());
	CPPUNIT_ASSERT_EQUAL(int(len), assign.len());
	CPPUNIT_ASSERT(equal(input, input + len, assign.cstr()));

	// assignment from C string
	RVNGString assign2;
	assign2 = input;
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), assign.size());
	CPPUNIT_ASSERT_EQUAL(int(len), assign.len());
	CPPUNIT_ASSERT(equal(input, input + len, assign.cstr()));
}

void RVNGStringTest::testAppend()
{
	const char input[] = "hello world";
	const size_t len = RVNG_NUM_ELEMENTS(input) - 1;
	RVNGString str;

	// appending a character
	str.append('h');
	CPPUNIT_ASSERT_EQUAL(1ul, str.size());
	CPPUNIT_ASSERT_EQUAL(1, str.len());
	CPPUNIT_ASSERT_EQUAL('h', str.cstr()[0]);

	RVNGString str2;
	// appending a RVNGString
	str2.append(str);
	CPPUNIT_ASSERT_EQUAL(1ul, str2.size());
	CPPUNIT_ASSERT_EQUAL(1, str2.len());
	CPPUNIT_ASSERT_EQUAL('h', str2.cstr()[0]);

	// appending a C string
	str2.append(input + 1);
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), str2.size());
	CPPUNIT_ASSERT_EQUAL(int(len), str2.len());
	CPPUNIT_ASSERT(equal(input, input + len, str2.cstr()));
}

void RVNGStringTest::testAppendEscapedXML()
{
	implTestEscapeXML("hello world", "hello world");
	implTestEscapeXML("<greetings who=\"world\" kind=\'&hello;\'>", "&lt;greetings who=&quot;world&quot; kind=&apos;&amp;hello;&apos;&gt;");
}

void RVNGStringTest::testClear()
{
	RVNGString str;
	// clearing empty str does nothing
	str.clear();
	CPPUNIT_ASSERT_EQUAL(0ul, str.size());
	CPPUNIT_ASSERT_EQUAL(0, str.len());

	str.append('a');
	str.clear();
	CPPUNIT_ASSERT_EQUAL(0ul, str.size());
	CPPUNIT_ASSERT_EQUAL(0, str.len());
}

CPPUNIT_TEST_SUITE_REGISTRATION(RVNGStringTest);

}

#endif // RVNGSTRINGTEST_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
