/* libwpd
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "WP5FormatGroup.h"
#include "WPXListener.h"
#include "libwpd_internal.h"

WP5FormatGroup::WP5FormatGroup(WPXInputStream *input) :	
	WP5VariableLengthGroup()
{
	_read(input);
}

void WP5FormatGroup::_readContents(WPXInputStream *input)
{
	switch(getSubGroup())
	{
		case WP5_FORMAT_GROUP_END_OF_PAGE:
			// wow, lots of stuff to parse here, but I'll just ignore it :-)
			// most (all?) page breaks are just a single function byte 0x0C anyway
			break;
		default:
			break;
	}	
}

void WP5FormatGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Format group\n"));

	switch(getSubGroup())
	{
		case WP5_FORMAT_GROUP_END_OF_PAGE:
			listener->insertBreak(WPX_PAGE_BREAK);
			break;		
		default:
			break;		
	}
}
