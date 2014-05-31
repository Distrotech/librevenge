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

#ifndef RVNGSTREAMTATEST_H
#define RVNGSTREAMTATEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace test
{

class RVNGStreamTest : public CPPUNIT_NS::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();

private:
	CPPUNIT_TEST_SUITE(RVNGStreamTest);
	CPPUNIT_TEST(testFileStream);
	CPPUNIT_TEST(testStringStream);
	CPPUNIT_TEST_SUITE_END();

private:
	void testFileStream();
	void testMemoryInputStream();
	void testStringStream();
};

}

#endif // RVNGSTREAMTATEST_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
