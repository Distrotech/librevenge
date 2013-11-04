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

#ifndef RVNGTEXTGENERATORFACTORY_H
#define RVNGTEXTGENERATORFACTORY_H

#include <librevenge/RVNGStringVector.h>
#include <librevenge/RVNGTextInterface.h>

namespace librevenge
{

/** A factory for text generators.

  The generators produced by this factory are dynamically allocated. It
  is responsibility of the user to delete them after use.
  */
class RVNGTextGeneratorFactory
{
public:
	// TODO: The generators should put their output into a StringVector.
	// That means passing a RVNGStringVector argument to every one of them.
	// Adapt.

	static RVNGTextInterface *makeHTML(RVNGStringVector &output);
	static RVNGTextInterface *makeRaw(RVNGStringVector &output);
	static RVNGTextInterface *makeText(RVNGStringVector &output);
};

}

#endif // RVNGTEXTGENERATORFACTORY_H

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
