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

#include "WP6TabGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

// use the BELL code to represent a TAB for now
// TODO: is this the right way a TAB should be defined or should we use a WPX_CHARACTER_TAB define?
#define UCS_TAB 0x0009 

WP6TabGroup::WP6TabGroup(FILE *stream) :
	WP6VariableLengthGroup()
{
	_read(stream);
}

void WP6TabGroup::_readContents(FILE *stream)
{
}

void WP6TabGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Tab group\n"));
	
	guint8 tabDefinition = getSubGroup();
	
	// TODO: soft type (tab, align, centerm and so forth), (uses tab set definition type)
	if (tabDefinition & 0x01) { }
	// TODO: dot leader 
	if (tabDefinition & 0x02) { }
	// TODO: generic search (only used in search code, ignore bits 0 and 1 when comparing)
	if (tabDefinition & 0x04) { }    
	
	switch ((tabDefinition & 0xF8) >> 3)
	{
		case  0: // 00000b = back tab
		case  1: // 00001b = table tab
		case  2: // 00010b = left tab
		case  4: // 00100b = bar tab
		case  6: // 00110b = left indent
		case  7: // 00111b = left/right indent
		case  8: // 01000b = center on margins
		case  9: // 01001b = center on current position
		case 10: // 01010b = center tab
		case 16: // 10000b = flush right
		case 18: // 10010b = right tab
		case 26: // 11010b = decimal tab
			// TODO: fix stupid default implementation of adding just a TAB char without looking what it actually should be
			llListener->insertCharacter(UCS_TAB);		
			break;
		default: // something else shouldn't be possible according to the documentation
			break;
	}
}
