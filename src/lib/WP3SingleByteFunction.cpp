/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3SingleByteFunction.h"
#include "WP3LLListener.h"
#include "libwpd_internal.h"

WP3SingleByteFunction * WP3SingleByteFunction::constructSingleByteFunction(WPXInputStream *input, uint8_t groupID)
{

	switch (groupID) 
	{
	case 0x80: // condensed hard return
		return new WP3EOLFunction();

	case 0x81: // condensed hard page
		return new WP3EOPFunction();

	case 0x96: // hard hyphen in line
		return new WP3HyphenFunction();

	case 0x97: // soft hyphen in line
		return new WP3SoftHyphenFunction();		
	
	case 0xa0: // hard space
		return new WP3HardSpaceFunction();
		
	default:
		// should not happen
		return NULL;
		}
}

/*void WP3SpaceFunction::parse(WP3HLListener *listener)
{
	listener->insertCharacter((uint16_t) ' ');
}*/

void WP3HardSpaceFunction::parse(WP3HLListener *listener)
{
	listener->insertCharacter((uint16_t) 0xa0);
}

void WP3HyphenFunction::parse(WP3HLListener *listener)
{
	listener->insertCharacter((uint16_t) '-');
}

void WP3SoftHyphenFunction::parse(WP3HLListener *listener)
{
	listener->insertCharacter((uint16_t) 0xad);
}

void WP3EOLFunction::parse(WP3HLListener *listener)
{
	listener->insertEOL();
}

/* void WP3EOCFunction::parse(WP3HLListener *listener)
{
	listener->insertBreak(WPX_COLUMN_BREAK);
}*/

void WP3EOPFunction::parse(WP3HLListener *listener)
{
	listener->insertBreak(WPX_PAGE_BREAK);
}

