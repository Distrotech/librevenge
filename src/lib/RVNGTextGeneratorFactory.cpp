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

#include <librevenge-generators/RVNGTextGeneratorFactory.h>

#include "RVNGHTMLTextGenerator.h"
#include "RVNGRawTextGenerator.h"
#include "RVNGTextTextGenerator.h"

namespace librevenge
{

RVNGTextInterface *RVNGTextGeneratorFactory::makeHTML(RVNGStringVector &/*output*/)
{
	return new RVNGHTMLTextGenerator();
}

RVNGTextInterface *RVNGTextGeneratorFactory::makeRaw(RVNGStringVector &/*output*/)
{
	// TODO: expose isInfo in API or drop it?
	return new RVNGRawTextGenerator(false);
}

RVNGTextInterface *RVNGTextGeneratorFactory::makeText(RVNGStringVector &/*output*/)
{
	// TODO: expose printCallgraphScore in API or drop it?
	return new RVNGTextTextGenerator(false);
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
