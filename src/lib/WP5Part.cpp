/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXPart.h"
#include "WP5Part.h"
#include "WP5Header.h"
#include "WP5VariableLengthGroup.h"
#include "WP5FixedLengthGroup.h"
#include "WP5SingleByteFunction.h"
#include "libwpd_internal.h"

// constructPart: constructs a parseable low-level representation of part of the document
// returns the part if it successfully creates the part, returns NULL if it can't
// throws an exception if there is an error
// precondition: readVal is between 0xC0 and 0xFF
WP5Part * WP5Part::constructPart(WPXInputStream *input, uint8_t readVal)
{	
	WPD_DEBUG_MSG(("WordPerfect: ConstructPart\n"));

	if (readVal >= (uint8_t)0x80 && readVal <= (uint8_t)0xBF)
	{
		// single-byte function
	
		WPD_DEBUG_MSG(("WordPerfect: constructSingleByteFunction(input, val)\n"));
		return WP5SingleByteFunction::constructSingleByteFunction(input, readVal);
	}      
	else if (readVal >= (uint8_t)0xC0 && readVal <= (uint8_t)0xCF)
	{
		// fixed length multi-byte function
	
		WPD_DEBUG_MSG(("WordPerfect: constructFixedLengthGroup(input, val)\n"));
		return WP5FixedLengthGroup::constructFixedLengthGroup(input, readVal);
	}      
	else if (readVal >= (uint8_t)0xD0)
	{
		// variable length multi-byte function
	
		WPD_DEBUG_MSG(("WordPerfect: constructVariableLengthGroup(input, val)\n"));
		return WP5VariableLengthGroup::constructVariableLengthGroup(input, readVal);
	}

	WPD_DEBUG_MSG(("WordPerfect: Returning NULL from constructPart\n"));
	return NULL;
}
