/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002, 2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002, 2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */
#include "librevenge_internal.h"
#include <ctype.h>
#include <locale.h>
#include <string>


RVNGString doubleToString(const double value)
{
	RVNGString tempString;
	if (value < 0.0001 && value > -0.0001)
		tempString.sprintf("0.0000");
	else
		tempString.sprintf("%.4f", value);
#ifndef __ANDROID__
	std::string decimalPoint(localeconv()->decimal_point);
#else
	std::string decimalPoint(".");
#endif
	if ((decimalPoint.size() == 0) || (decimalPoint == "."))
		return tempString;
	std::string stringValue(tempString.cstr());
	if (!stringValue.empty())
	{
		std::string::size_type pos;
		while ((pos = stringValue.find(decimalPoint)) != std::string::npos)
			stringValue.replace(pos,decimalPoint.size(),".");
	}
	return RVNGString(stringValue.c_str());
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
