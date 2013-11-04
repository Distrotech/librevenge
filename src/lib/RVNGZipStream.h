/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2012 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGZIPSTREAM_H
#define RVNGZIPSTREAM_H

#include <string>
#include <vector>

namespace librevenge
{

class RVNGInputStream;

class RVNGZipStream
{
public:
	static bool isZipFile(RVNGInputStream *input);

	static std::vector<std::string> getSubStreamNamesList(RVNGInputStream *input);
	static RVNGInputStream *getSubstream(RVNGInputStream *input, const char *name);
};

}

#endif // RVNGZIPSTREAM_H
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
