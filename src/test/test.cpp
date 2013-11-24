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

#include <iostream>

#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/CompilerOutputter.h>

int main()
{
	// Create the event manager and test controller
	CPPUNIT_NS::TestResult controller;

	// Add a listener that colllects test result
	CPPUNIT_NS::TestResultCollector result;
	controller.addListener(&result);

	// Add a listener that print dots as test run.
	CPPUNIT_NS::BriefTestProgressListener progress;
	controller.addListener(&progress);

	// Add the top suite to the test runner
	CPPUNIT_NS::TestRunner runner;
	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run(controller);

	// output
	CPPUNIT_NS::CompilerOutputter outputter(&result, std::cerr);
	outputter.write();

	// return status code
	return result.wasSuccessful() ? 0 : 1;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
