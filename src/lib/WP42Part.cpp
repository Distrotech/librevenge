/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003-2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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
#include "WP42Part.h"
#include "WP42FileStructure.h"
#include "WP42VariableLengthGroup.h"
#include "libwpd_internal.h"

// constructPart: constructs a parseable low-level representation of part of the document
// returns the part if it successfully creates the part, returns NULL if it can't
// throws an exception if there is an error
// precondition: readVal us between 0xC0 and 0xFF
// TODO: check the precondition :D
WP42Part * WP42Part::constructPart(WPXInputStream *input, uint8_t readVal)
{	
	WPD_DEBUG_MSG(("WordPerfect: ConstructPart(readVal: 0x0%x)\n", readVal));
		
	// TODO: ASSERT readVal
	
	if (WP42_FUNCTION_GROUP_SIZE[readVal-0xC0] == -1)
	{
		WPD_DEBUG_MSG(("WordPerfect: constructVariableLengthGroup(input, val)\n"));
		return WP42VariableLengthGroup::constructVariableLengthGroup(input, readVal);
	}
	else
	{
		// fixed length function group
		
		// for now, we just skip over the group
		input->seek(WP42_FUNCTION_GROUP_SIZE[readVal-0xC0]-1, WPX_SEEK_CUR);
	}

	WPD_DEBUG_MSG(("WordPerfect: Returning NULL from constructPart\n"));
	return NULL;
}
