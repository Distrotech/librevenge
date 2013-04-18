/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6Part.h"
#include "WP6VariableLengthGroup.h"
#include "WP6FixedLengthGroup.h"
#include "WP6SingleByteFunction.h"

// constructPart: constructs a parseable low-level representation of part of the document
// returns the part if it successfully creates the part, returns 0 if it can't
// throws an exception if there is an error
// precondition: readVal us between 0x80 and 0xFF
WP6Part *WP6Part::constructPart(WPXInputStream *input, WPXEncryption *encryption, const uint8_t readVal)
{
	WPD_DEBUG_MSG(("WordPerfect: ConstructPart\n"));

	if (readVal >= (uint8_t)0x80 && readVal <= (uint8_t)0xCF)
	{
		WPD_DEBUG_MSG(("WordPerfect: constructSingleByteFunction(input, val=0x%.2x)\n", readVal));
		return WP6SingleByteFunction::constructSingleByteFunction(input, encryption, readVal);
	}
	else if (readVal >= (uint8_t)0xD0 && readVal <= (uint8_t)0xEF)
	{
		if (!WP6VariableLengthGroup::isGroupConsistent(input, encryption, readVal))
		{
			WPD_DEBUG_MSG(("WordPerfect: Consistency Check (variable length) failed; ignoring this byte\n"));
			return 0;
		}
		WPD_DEBUG_MSG(("WordPerfect: constructVariableLengthGroup(input, val=0x%.2x)\n", readVal));
		return WP6VariableLengthGroup::constructVariableLengthGroup(input, encryption, readVal);
	}

	else if (readVal >= (uint8_t)0xF0 && readVal < (uint8_t)0xFF)
	{
		if (!WP6FixedLengthGroup::isGroupConsistent(input, encryption, readVal))
		{
			WPD_DEBUG_MSG(("WordPerfect: Consistency Check (fixed length) failed; ignoring this byte\n"));
			return 0;
		}
		WPD_DEBUG_MSG(("WordPerfect: constructFixedLengthGroup(input, val=0x%.2x)\n", readVal));
		return WP6FixedLengthGroup::constructFixedLengthGroup(input, encryption, readVal);
	}
#ifdef DEBUG
	else
		WPD_DEBUG_MSG(("WordPerfect: invalid group (val=0x%2x)\n", readVal));
#endif

	WPD_DEBUG_MSG(("WordPerfect: Returning 0 from constructPart\n"));
	return 0;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
