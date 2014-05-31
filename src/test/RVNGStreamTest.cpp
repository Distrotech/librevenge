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

#include <boost/scoped_ptr.hpp>

#include <stdio.h>
#include <unistd.h>

#include <librevenge/librevenge.h>

#include "librevenge_internal.h"
#include "RVNGMemoryStream.h"

#include "RVNGStreamTest.h"

using boost::scoped_ptr;

using namespace librevenge;

#define TMP_FILENAME "librevenge-unittest.tmp"

namespace test
{

void RVNGStreamTest::setUp()
{
	FILE *f = fopen(TMP_FILENAME, "w");
	fprintf(f, "%c%c%c%c",1,2,3,4);
	fprintf(f, "%c%c%c%c",0,5,6,7);
	fclose(f);
}

void RVNGStreamTest::tearDown()
{
	unlink(TMP_FILENAME);
}

void RVNGStreamTest::testFileStream()
{
	scoped_ptr<RVNGInputStream> input(new RVNGFileStream(TMP_FILENAME));
	unsigned long numBytesRead = 0;

	CPPUNIT_ASSERT_EQUAL(false, input->isStructured());
	CPPUNIT_ASSERT_EQUAL((RVNGInputStream *) NULL, input->getSubStreamByName("foo"));

	// test read()
	input->seek(0, RVNG_SEEK_SET);
	CPPUNIT_ASSERT(NULL == input->read(0, numBytesRead));
	CPPUNIT_ASSERT_EQUAL((unsigned long)0, numBytesRead);
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

void RVNGStreamTest::testStringStream()
{
	scoped_ptr<RVNGInputStream> input(new RVNGStringStream((const unsigned char *)"\1\2\3\4\0\5\6\7", 8));
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

CPPUNIT_TEST_SUITE_REGISTRATION(RVNGStreamTest);

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
