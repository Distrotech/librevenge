/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5BoxGroup.h"
#include "WPXListener.h"
#include "libwpd_internal.h"
#include "WP5FileStructure.h"
#include "WP5PrefixData.h"
#include "WP5Listener.h"

WP5BoxGroup::WP5BoxGroup(WPXInputStream *input) :	
	WP5VariableLengthGroup()
{
	_read(input);
}

void WP5BoxGroup::_readContents(WPXInputStream * /* input */)
{
	switch(getSubGroup())
	{
		case WP5_TOP_BOX_GROUP_FIGURE:
			break;
		default:
			break;
	}	
}

void WP5BoxGroup::parse(WP5Listener * /* listener */)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Box group\n"));

	switch(getSubGroup())
	{
		case WP5_TOP_BOX_GROUP_FIGURE:
			break;
		default:
			break;		
	}
}
