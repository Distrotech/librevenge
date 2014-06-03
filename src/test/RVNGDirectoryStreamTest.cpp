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

#include <sys/stat.h>
#include <string>
#include <vector>

#include <boost/scoped_ptr.hpp>

#include <librevenge-stream/librevenge-stream.h>

#include "RVNGDirectoryStreamTest.h"

#if !defined RVNG_DIRECTORY_STREAM_TEST_DIR
#error RVNG_DIRECTORY_STREAM_TEST_DIR not defined, cannot test
#endif

using boost::scoped_ptr;

using librevenge::RVNGDirectoryStream;
using librevenge::RVNGInputStream;

namespace test
{

namespace
{

static const char TEST_DIR[] = RVNG_DIRECTORY_STREAM_TEST_DIR;
static const char TEST_FILENAME[] = "RVNGDirectoryStream.h";
static const char TEST_NONEXISTENT[] = "foobar";

#ifdef DEBUG

static bool isReg(const char *const path)
{
	struct stat statBuf;
	if (stat(path, &statBuf))
		return false;
	if (S_ISREG(statBuf.st_mode))
		return true;
#ifndef _WIN32
	else if (S_ISLNK(statBuf.st_mode))
	{
		if (lstat(path, &statBuf))
			return false;
		if (S_ISREG(statBuf.st_mode))
			return true;
	}
#endif
	return false;
}

static bool isDir(const char *const path)
{
	struct stat statBuf;
	if (stat(path, &statBuf))
		return false;
	if (S_ISDIR(statBuf.st_mode))
		return true;
#ifndef _WIN32
	else if (S_ISLNK(statBuf.st_mode))
	{
		if (lstat(path, &statBuf))
			return false;
		if (S_ISDIR(statBuf.st_mode))
			return true;
	}
#endif
	return false;
}

static bool exists(const char *const path)
{
	struct stat statBuf;
	if (stat(path, &statBuf))
		return false;
	return true;
}

#endif

}

RVNGDirectoryStreamTest::RVNGDirectoryStreamTest()
	: m_dir(TEST_DIR)
	, m_file(TEST_DIR)
	, m_nonexistent(TEST_DIR)
{
	m_file.append("/");
	m_file.append(TEST_FILENAME);
	m_nonexistent.append("/");
	m_nonexistent.append(TEST_NONEXISTENT); // dir
	m_nonexistent.append("/");
	m_nonexistent.append(TEST_NONEXISTENT); // file
	// sanity check
	assert(isDir(m_dir.c_str()));
	assert(isReg(m_file.c_str()));
	assert(!exists(m_nonexistent.c_str()));
}

void RVNGDirectoryStreamTest::setUp()
{
}

void RVNGDirectoryStreamTest::tearDown()
{
}

void RVNGDirectoryStreamTest::testConstruction()
{
	const scoped_ptr<RVNGDirectoryStream> dir(RVNGDirectoryStream::createForParent(m_file.c_str()));
	CPPUNIT_ASSERT(bool(dir));
	CPPUNIT_ASSERT(dir->isStructured());

	// this should work for dirs too
	const scoped_ptr<RVNGDirectoryStream> dir2(RVNGDirectoryStream::createForParent(m_dir.c_str()));
	CPPUNIT_ASSERT(bool(dir2));
	CPPUNIT_ASSERT(dir2->isStructured());

	// for nonexistent dirs nothing is created
	const scoped_ptr<RVNGDirectoryStream> nondir(RVNGDirectoryStream::createForParent(m_nonexistent.c_str()));
	CPPUNIT_ASSERT(!nondir);

	// even if try harder, just an empty shell is created
	RVNGDirectoryStream nondir2(m_nonexistent.c_str());
	CPPUNIT_ASSERT(!nondir2.isStructured());
}

void RVNGDirectoryStreamTest::testDetection()
{
	CPPUNIT_ASSERT(RVNGDirectoryStream::isDirectory(m_dir.c_str()));
	CPPUNIT_ASSERT(!RVNGDirectoryStream::isDirectory(m_file.c_str()));
	CPPUNIT_ASSERT(!RVNGDirectoryStream::isDirectory(m_nonexistent.c_str()));
}

void RVNGDirectoryStreamTest::testDataOperations()
{
	// data operations do not make sense on a directory -> just dummy
	// impls.
	RVNGDirectoryStream dir(m_dir.c_str());

	CPPUNIT_ASSERT(dir.isEnd());
	CPPUNIT_ASSERT_EQUAL(0L, dir.tell());
	CPPUNIT_ASSERT_EQUAL(-1, dir.seek(0, librevenge::RVNG_SEEK_CUR));

	unsigned long numBytesRead = 0;
	CPPUNIT_ASSERT(0 == dir.read(1, numBytesRead));
	CPPUNIT_ASSERT_EQUAL(0UL, numBytesRead);
}

void RVNGDirectoryStreamTest::testStructuredOperations()
{
	{
		RVNGDirectoryStream dir(m_dir.c_str());

		CPPUNIT_ASSERT(dir.isStructured());
		scoped_ptr<RVNGInputStream> substream(dir.getSubStreamByName(TEST_FILENAME));
		CPPUNIT_ASSERT(substream.get());

		// TODO: test for other operations when they are implemented =)
	}

	{
		scoped_ptr<RVNGDirectoryStream> dir(RVNGDirectoryStream::createForParent(m_file.c_str()));

		CPPUNIT_ASSERT(dir->isStructured());
		scoped_ptr<RVNGInputStream> substream(dir->getSubStreamByName(TEST_FILENAME));
		CPPUNIT_ASSERT(substream.get());

		// TODO: test for other operations when they are implemented =)
	}
}

CPPUNIT_TEST_SUITE_REGISTRATION(RVNGDirectoryStreamTest);

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
