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

#include <algorithm>

#include <librevenge/librevenge.h>

#include "librevenge_internal.h"

#include "RVNGBinaryDataTest.h"

using librevenge::RVNGBinaryData;

using std::equal;

namespace test
{

void RVNGBinaryDataTest::setUp()
{
}

void RVNGBinaryDataTest::tearDown()
{
}

void RVNGBinaryDataTest::testConstruction()
{
	// default ctor creates empty data
	RVNGBinaryData empty;
	CPPUNIT_ASSERT_EQUAL(0ul, empty.size());

	// construction from a buffer
	const unsigned char input[] = "hello world";
	const size_t len = RVNG_NUM_ELEMENTS(input);
	RVNGBinaryData data(input, len);
	CPPUNIT_ASSERT_EQUAL(len, data.size());
	CPPUNIT_ASSERT(equal(input, input + len, data.getDataBuffer()));

	{
		// copy construction
		RVNGBinaryData copy(data);
		CPPUNIT_ASSERT_EQUAL(len, copy.size());
		CPPUNIT_ASSERT(equal(input, input + len, copy.getDataBuffer()));
	}
	// COW: destruction of a copy does not touch the original
	// this should be tested under valgrind
	CPPUNIT_ASSERT_EQUAL(len, data.size());
	CPPUNIT_ASSERT(equal(input, input + len, data.getDataBuffer()));

	// assignment
	RVNGBinaryData assign;
	CPPUNIT_ASSERT_EQUAL(0ul, assign.size());
	assign = data;
	CPPUNIT_ASSERT_EQUAL(len, assign.size());
	CPPUNIT_ASSERT(equal(input, input + len, assign.getDataBuffer()));

}

void RVNGBinaryDataTest::testAppend()
{
	const unsigned char input[] = "hello world";
	const size_t len = RVNG_NUM_ELEMENTS(input);
	RVNGBinaryData data;

	// appending a character
	data.append('h');
	CPPUNIT_ASSERT_EQUAL(1ul, data.size());
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned char>('h'), data.getDataBuffer()[0]);

	RVNGBinaryData data2;
	// appending other RVNGBinaryData
	data2.append(data);
	CPPUNIT_ASSERT_EQUAL(1ul, data2.size());
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned char>('h'), data2.getDataBuffer()[0]);
	// the original is unchanged
	CPPUNIT_ASSERT_EQUAL(1ul, data.size());
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned char>('h'), data.getDataBuffer()[0]);

	data2.append(input + 1, len - 1);
	CPPUNIT_ASSERT_EQUAL(len, data2.size());
	CPPUNIT_ASSERT(equal(input, input + len, data2.getDataBuffer()));
	// again, the original is unchanged
	CPPUNIT_ASSERT_EQUAL(1ul, data.size());
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned char>('h'), data.getDataBuffer()[0]);
}

void RVNGBinaryDataTest::testClear()
{
	RVNGBinaryData data;
	// clearing empty data does nothing
	data.clear();
	CPPUNIT_ASSERT_EQUAL(0ul, data.size());

	data.append('a');
	RVNGBinaryData copy(data);
	// clearing empties the content
	data.clear();
	CPPUNIT_ASSERT_EQUAL(0ul, data.size());
	// but the copy remains unchanged
	CPPUNIT_ASSERT_EQUAL(1ul, copy.size());
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned char>('a'), copy.getDataBuffer()[0]);
}

void RVNGBinaryDataTest::testBase64()
{
	// TODO: implement me
}

void RVNGBinaryDataTest::testStream()
{
	// TODO: implement me
}

CPPUNIT_TEST_SUITE_REGISTRATION(RVNGBinaryDataTest);

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
