/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP5Part.h"
#include "WP5VariableLengthGroup.h"
#include "WP5FixedLengthGroup.h"
#include "WP5SingleByteFunction.h"
#include "WP5Listener.h"

// constructPart: constructs a parseable low-level representation of part of the document
// returns the part if it successfully creates the part, returns 0 if it can't
// throws an exception if there is an error
// precondition: readVal is between 0xC0 and 0xFF
WP5Part * WP5Part::constructPart(WPXInputStream *input, WPXEncryption *encryption, const uint8_t readVal)
{	
	WPD_DEBUG_MSG(("WordPerfect: ConstructPart\n"));

	if (readVal >= (uint8_t)0x80 && readVal <= (uint8_t)0xBF)
	{
		// single-byte function
	
		WPD_DEBUG_MSG(("WordPerfect: constructSingleByteFunction(input, val)\n"));
		return WP5SingleByteFunction::constructSingleByteFunction(input, encryption, readVal);
	}      
	else if (readVal >= (uint8_t)0xC0 && readVal <= (uint8_t)0xCF)
	{
		// fixed length multi-byte function
	
		if (!WP5FixedLengthGroup::isGroupConsistent(input, encryption, readVal))
		{
			WPD_DEBUG_MSG(("WordPerfect: Consistency Check (fixed length) failed; ignoring this byte\n"));
			return 0;
		}
		WPD_DEBUG_MSG(("WordPerfect: constructFixedLengthGroup(input, val)\n"));
		return WP5FixedLengthGroup::constructFixedLengthGroup(input, encryption, readVal);
	}      
	else if (readVal >= (uint8_t)0xD0)
	{
		// variable length multi-byte function
	
		/* check whether the function is consistent with the variable length
		 * function definition. If not, just skip the function byte and try next position.
		 * The documentation speaks about variable length functions from 0xD0 to 0xFF, but
		 * only 0xD0 to 0xDF, 0xE1 and 0xFE are documented. We saw in some documents that
		 * the 0xE8 function was a single byte undocumented function (or corruption???) */
		if (!WP5VariableLengthGroup::isGroupConsistent(input, encryption, readVal))
		{
			WPD_DEBUG_MSG(("WordPerfect: Consistency Check (variable length) failed; ignoring this byte\n"));
			return 0;
		}
		WPD_DEBUG_MSG(("WordPerfect: constructVariableLengthGroup(input, val)\n"));
		return WP5VariableLengthGroup::constructVariableLengthGroup(input, encryption, readVal);
	}


	WPD_DEBUG_MSG(("WordPerfect: Returning 0 from constructPart\n"));
	return 0;
}
