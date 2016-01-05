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

#ifndef RVNGSTRINGITERTEST_H
#define RVNGSTRINGITERTEST_H

#include <boost/lexical_cast.hpp>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <librevenge/librevenge.h>

#include "librevenge_internal.h"

namespace test
{

using librevenge::RVNGString;

using std::string;

namespace
{

void implTestIter(const string &msg, const char *chars[], const std::size_t count)
{
	assert(count > 1);

	RVNGString str;
	for (std::size_t i = 0; i != count; ++i)
		str.append(chars[i]);

	RVNGString::Iter iter(str);

	CPPUNIT_ASSERT_MESSAGE(msg + ": " + "not at end after creation", !iter.last());
	CPPUNIT_ASSERT_EQUAL_MESSAGE(msg + ": " + "deref. at first char", string(chars[0]), string(iter()));
	CPPUNIT_ASSERT_EQUAL_MESSAGE(msg + ": " + "second deref. doesn't change result", string(chars[0]), string(iter()));
	CPPUNIT_ASSERT_MESSAGE(msg + ": " + "can advance to second char", iter.next());

	for (std::size_t i = 1; i < count - 1; ++i)
	{
		const string pos(boost::lexical_cast<string>(i));
		CPPUNIT_ASSERT_MESSAGE(msg + ": " + "not at end at char pos " + pos, !iter.last());
		CPPUNIT_ASSERT_EQUAL_MESSAGE(msg + ": " + "deref. at char pos " + pos, string(chars[i]), string(iter()));
		CPPUNIT_ASSERT_MESSAGE(msg + ": " + "can advance at char pos " + pos, iter.next());
	}

	CPPUNIT_ASSERT_MESSAGE(msg + ": " + "not at end at last char", !iter.last());
	CPPUNIT_ASSERT_EQUAL_MESSAGE(msg + ": " + "deref. at last char", string(chars[count - 1]), string(iter()));
	CPPUNIT_ASSERT_MESSAGE(msg + ": " + "can't advance anymore", !iter.next());
	CPPUNIT_ASSERT_MESSAGE(msg + ": " + "is at end", iter.last());
}

}

class RVNGStringIterTest : public CPPUNIT_NS::TestFixture
{
public:
	virtual void setUp();
	virtual void tearDown();

private:
	CPPUNIT_TEST_SUITE(RVNGStringIterTest);
	CPPUNIT_TEST(testEmpty);
	CPPUNIT_TEST(testNonempty);
	CPPUNIT_TEST(testRewind);
	CPPUNIT_TEST_SUITE_END();

private:
	void testEmpty();
	void testNonempty();
	void testRewind();
};

void RVNGStringIterTest::setUp()
{
}

void RVNGStringIterTest::tearDown()
{
}

void RVNGStringIterTest::testEmpty()
{
	const RVNGString str;
	RVNGString::Iter iter(str);

	CPPUNIT_ASSERT_MESSAGE("can't advance on empty string", !iter.next());
	CPPUNIT_ASSERT_MESSAGE("deref. returns a string", iter());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("deref. returns an empty string", string(), string(iter()));
	CPPUNIT_ASSERT_MESSAGE("check end", iter.last());
}

void RVNGStringIterTest::testNonempty()
{
	const char *ascii[] = {"a", "b", "c"};
	implTestIter("ASCII text", ascii, RVNG_NUM_ELEMENTS(ascii));
	const char *cyrilic[] = {"\xd0\xb0", "\xd0\xb1", "\xd0\xb2"};
	implTestIter("Cyrilic text", cyrilic, RVNG_NUM_ELEMENTS(cyrilic));
	const char *mixed[] = {"a", "\xd0\xb1", "c", "\xd0\xb3"};
	implTestIter("Mixed text", mixed, RVNG_NUM_ELEMENTS(mixed));
}

void RVNGStringIterTest::testRewind()
{
	const RVNGString str;
	RVNGString::Iter iter(str);

	CPPUNIT_ASSERT_MESSAGE("at end", iter.last());
	iter.rewind();
	CPPUNIT_ASSERT_MESSAGE("not at end", !iter.last());
	CPPUNIT_ASSERT_EQUAL_MESSAGE("deref. is ignored", reinterpret_cast<const char *>(0), iter());
	CPPUNIT_ASSERT_MESSAGE("nothing to step forward at", !iter.next());
	CPPUNIT_ASSERT_MESSAGE("at end again", iter.last());
	CPPUNIT_ASSERT_MESSAGE("deref. returns a string", iter());
}

CPPUNIT_TEST_SUITE_REGISTRATION(RVNGStringIterTest);

}

#endif // RVNGSTRINGITERTEST_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
