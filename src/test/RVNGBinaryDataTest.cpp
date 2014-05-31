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
#include <cstring>
#include <string>

#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>

#include "librevenge_internal.h"

#include "RVNGBinaryDataTest.h"

using namespace librevenge;

using std::equal;
using std::strlen;

namespace test
{

namespace
{

bool equalBinaryData(const RVNGBinaryData &binaryData, const char *const str)
{
	const unsigned len = strlen(str);
	if (binaryData.size() != len)
		return false;

	if (0 == len) // there is nothing to compare here
		return true;

	return equal(str, str + len, reinterpret_cast<const char *>(binaryData.getDataBuffer()));
}

void implTestBase64(const char *const plain, const char *const base64)
{
	const std::string msg(std::string("plain text: '") + plain + '\'');
	// construct (decode)
	{
		const RVNGBinaryData data(base64);
		CPPUNIT_ASSERT_MESSAGE(msg, equalBinaryData(data, plain));
	}
	{
		const RVNGString base64Str(base64);
		const RVNGBinaryData data(base64Str);
		CPPUNIT_ASSERT_MESSAGE(msg, equalBinaryData(data, plain));
	}

	// append (decode)
	{
		RVNGBinaryData data;
		data.appendBase64Data(base64);
		CPPUNIT_ASSERT_MESSAGE(msg, equalBinaryData(data, plain));
	}
	{
		RVNGBinaryData data;
		const RVNGString base64Str(base64);
		data.appendBase64Data(base64Str);
		CPPUNIT_ASSERT_MESSAGE(msg, equalBinaryData(data, plain));
	}

	// encode
	{
		const RVNGBinaryData data(reinterpret_cast<const unsigned char *>(plain), strlen(plain));
		CPPUNIT_ASSERT_EQUAL_MESSAGE(msg, 0, std::strcmp(base64, data.getBase64Data().cstr()));
	}
}

}

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
	CPPUNIT_ASSERT(empty.empty());

	// construction from a buffer
	const unsigned char input[] = "hello world";
	const size_t len = RVNG_NUM_ELEMENTS(input);
	RVNGBinaryData data(input, len);
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), data.size());
	CPPUNIT_ASSERT(!data.empty());
	CPPUNIT_ASSERT(equal(input, input + len, data.getDataBuffer()));

	{
		// copy construction
		RVNGBinaryData copy(data);
		CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), copy.size());
		CPPUNIT_ASSERT(equal(input, input + len, copy.getDataBuffer()));
	}
	// COW: destruction of a copy does not touch the original
	// this should be tested under valgrind
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), data.size());
	CPPUNIT_ASSERT(equal(input, input + len, data.getDataBuffer()));

	// assignment
	RVNGBinaryData assign;
	CPPUNIT_ASSERT_EQUAL(0ul, assign.size());
	CPPUNIT_ASSERT(assign.empty());
	assign = data;
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), assign.size());
	CPPUNIT_ASSERT(!assign.empty());
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
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(len), data2.size());
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
	// NOTE: these patterns come from http://www.ietf.org/rfc/rfc4648.txt
	implTestBase64("", "");
	implTestBase64("f", "Zg==");
	implTestBase64("fo", "Zm8=");
	implTestBase64("foo", "Zm9v");
	implTestBase64("foob", "Zm9vYg==");
	implTestBase64("fooba", "Zm9vYmE=");
	implTestBase64("foobar", "Zm9vYmFy");

	// more test patterns
	implTestBase64("e=mc^2", "ZT1tY14y");
}

void RVNGBinaryDataTest::testStream()
{
	RVNGBinaryData data((const unsigned char *)("\1\2\3\4\0\5\6\7"), 8);
	RVNGInputStream *input = const_cast<RVNGInputStream *>(data.getDataStream());
	unsigned long numBytesRead = 0;

	CPPUNIT_ASSERT_EQUAL(false, input->isStructured());
	CPPUNIT_ASSERT_EQUAL((RVNGInputStream *) NULL, input->getSubStreamByName("foo"));

	// test read()
	input->seek(0, RVNG_SEEK_SET);
	CPPUNIT_ASSERT(NULL == input->read(0, numBytesRead));
	CPPUNIT_ASSERT_EQUAL((unsigned long) 0, numBytesRead);
	CPPUNIT_ASSERT_EQUAL((long) 0 , input->tell());
	CPPUNIT_ASSERT(NULL != input->read(1, numBytesRead));
	CPPUNIT_ASSERT_EQUAL((long) 1 , input->tell());

	input->seek(0, RVNG_SEEK_SET);
	CPPUNIT_ASSERT(NULL != input->read(50, numBytesRead));
	CPPUNIT_ASSERT_EQUAL((long) 8 , input->tell());

	// test seek(), tell(), isEnd()
	input->seek(1, RVNG_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL((long) 1 , input->tell());

	input->seek(0, RVNG_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL((long) 0 , input->tell());

	input->seek(8, RVNG_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL((long) 8 , input->tell());

	input->seek(-1, RVNG_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL((long) 0, input->tell());

	input->seek(8, RVNG_SEEK_SET);
	CPPUNIT_ASSERT_EQUAL(true, input->isEnd());

	input->seek(10000, RVNG_SEEK_SET);
	CPPUNIT_ASSERT(10000 != input->tell());
	CPPUNIT_ASSERT(input->isEnd());
}

CPPUNIT_TEST_SUITE_REGISTRATION(RVNGBinaryDataTest);

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
