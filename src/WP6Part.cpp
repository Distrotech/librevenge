/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
#include "WP6Part.h"
#include "WP6Header.h"
#include "WP6VariableLengthGroup.h"
#include "WP6FixedLengthGroup.h"

WP6Part::WP6Part(FILE * stream)
	: WPXPart(stream)
{
}

WP6Part * WP6Part::constructPart(FILE * stream, WPXParser * parser)
{
	guint8 val;
	while (ftell(stream) < (long)((WP6Header *)parser->getHeader())->m_iDocumentSize)
	{
		WPD_CHECK_FILE_READ_ERROR(fread(&val, sizeof(guint8), 1, stream), 1);
		guint32 readVal = val; // convert to a 32 bit int, otherwise gcc3.2 will start whining about comparisions always being true
		
		if (readVal >= 0x00 && readVal <= 0x20)
		{
			// Default Extended International Characters
		}
		else if (readVal >= 0x21 && readVal <= 0x7F)
		{
			// normal ASCII characters
			parser->getLLListener()->insertCharacter( (guint32)readVal );
		}
		else if (readVal >= 0x80 && readVal <= 0xCF)
		{
			// Single-Byte Functions
		}
		else if (readVal >= 0xD0 && readVal <= 0xEF)
		{
			// TODO: recognize which variable length group we need to instantiate
			return new WP6VariableLengthGroup(stream);
		}      
		else if (readVal >= 0xF0 && readVal <= 0xFF)
		{
			// TODO: recognize which fixed length group we need to instantiate
			return WP6FixedLengthGroup::constructFixedLengthGroup(stream, val);
		}
	}
	
	return NULL;

	/*
	// flush any remaining text 
	WPD_CHECK_INTERNAL_ERROR(wp6_flush_text(wordperfect_state, wordperfect_parse_struct));
	;*/
}
