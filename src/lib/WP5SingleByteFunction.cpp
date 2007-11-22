/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP5SingleByteFunction.h"
#include "WP5Listener.h"
#include "libwpd_internal.h"

WP5SingleByteFunction * WP5SingleByteFunction::constructSingleByteFunction(WPXInputStream * /* input */, WPXEncryption * /* encryption */, uint8_t groupID)
{

	switch (groupID) 
	{
	case 0x8c: // combination hard return/soft page
	case 0x90: // deletable return at EOL
	case 0x99: // dormant hard return
		return new WP5EOLFunction();
		
	case 0x93: // invisible return in line
	case 0x94: // invisible return EOL
	case 0x95: // invisible return EOP
		return new WP5SpaceFunction();

	case 0xa0: // hard space
		return new WP5HardSpaceFunction();
		
	case 0xa9: // hard hyphen in line
	case 0xaa: // hard hyphen EOL
	case 0xab: // hard hyphen EOP
		return new WP5HyphenFunction();

	case 0xac: // soft hyphen in line
	case 0xad: // soft hyphen EOL
	case 0xae: // soft hyphen EOP
		return new WP5SoftHyphenFunction();		
	
	default:
		// should not happen
		return 0;
	}
}

void WP5EOLFunction::parse(WP5Listener *listener)
{
	listener->insertEOL();
}

void WP5SpaceFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint16_t) ' ');
}

void WP5HardSpaceFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint16_t) 0xa0);
}

void WP5HyphenFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint16_t) '-');
}

void WP5SoftHyphenFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint16_t) 0xad);
}

