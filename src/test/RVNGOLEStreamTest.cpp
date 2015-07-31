/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Andrew Ziem
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <sstream>
#include <string>

#include <boost/scoped_ptr.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <librevenge/librevenge.h>

#include "librevenge_internal.h"

#if !defined RVNG_DATA_DIR
#error RVNG_DATA_DIR not defined, cannot test
#endif

namespace test
{

using boost::scoped_ptr;

using librevenge::RVNGInputStream;
using librevenge::RVNGFileStream;

using std::string;

namespace
{

const string makeMessage(const unsigned n, const char *const message)
{
	std::ostringstream out;
	out << "substream " << n << ' ' << message;
	return out.str();
}

void assertSubStream(const unsigned n, const char *const message, bool actual)
{
	CPPUNIT_ASSERT_MESSAGE(makeMessage(n, message), actual);
}

template<typename T>
void assertSubStream(const unsigned n, const char *const message, const T &expected, const T &actual)
{
	CPPUNIT_ASSERT_EQUAL_MESSAGE(makeMessage(n, message), expected, actual);
}

const string getContent(RVNGInputStream *const input)
{
	input->seek(0, librevenge::RVNG_SEEK_END);
	const long end = input->tell();
	input->seek(0, librevenge::RVNG_SEEK_SET);

	unsigned long readBytes = 0;
	const unsigned char *const bytes = input->read(static_cast<unsigned long>(end), readBytes);
	CPPUNIT_ASSERT_EQUAL(end, long(readBytes));

	return string(reinterpret_cast<const char *>(bytes), size_t(readBytes));
}

}

class RVNGOLEStreamTest : public CPPUNIT_NS::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();

private:
	CPPUNIT_TEST_SUITE(RVNGOLEStreamTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();

private:
	void test();
};

void RVNGOLEStreamTest::setUp()
{
}

void RVNGOLEStreamTest::tearDown()
{
}

void RVNGOLEStreamTest::test()
{
	struct SubStream
	{
		const char *name;
		const char *content;
		unsigned size;
		bool dir;
	};

	const SubStream subStreams[] =
	{
		{"/", "", 11, true},
		{"a", "a\n", 0, false},
		{"b", "bb\n", 0, false},
		{"c/", "", 7, true},
		{"c/h/", "", 3, true},
		{"c/h/i/", "", 2, true},
		{"c/h/i/j", "jjjj\n", 0, false},
		{"c/g", "gg\n", 0, false},
		{"c/d/", "", 2, true},
		{"c/d/e", "eee\n", 0, false},
		{"f", "fff\n", 0, false},
	};

	RVNGFileStream input(RVNG_DATA_DIR "/test.ole");

	CPPUNIT_ASSERT_MESSAGE("file is structured", input.isStructured());
	CPPUNIT_ASSERT_EQUAL(RVNG_NUM_ELEMENTS(subStreams), size_t(input.subStreamCount()));

	for (unsigned i = 0; i != RVNG_NUM_ELEMENTS(subStreams); ++i)
	{
		assertSubStream(i, "exists", input.existsSubStream(subStreams[i].name));
		assertSubStream(i, "has matching name", string(subStreams[i].name), string(input.subStreamName(i)));

		const scoped_ptr<RVNGInputStream> streamById(input.getSubStreamById(i));
		const scoped_ptr<RVNGInputStream> streamByName(input.getSubStreamByName(subStreams[i].name));

		assertSubStream(i, "can be retrieved by id", bool(streamById));
		assertSubStream(i, "can be retrieved by name", bool(streamByName));
		assertSubStream(i, "get by id is the right type", subStreams[i].dir, streamById->isStructured());
		assertSubStream(i, "get by name is the right type", subStreams[i].dir, streamByName->isStructured());

		if (subStreams[i].dir)
		{
			assertSubStream(i, "get by id has the right number of substreams", subStreams[i].size, streamById->subStreamCount());
			assertSubStream(i, "get by name has the right number of substreams", subStreams[i].size, streamByName->subStreamCount());
		}
		else
		{
			assertSubStream(i, "get by id has the right content", string(subStreams[i].content), getContent(streamById.get()));
			assertSubStream(i, "get by name has the right content", string(subStreams[i].content), getContent(streamByName.get()));
		}
	}
}

CPPUNIT_TEST_SUITE_REGISTRATION(RVNGOLEStreamTest);

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
