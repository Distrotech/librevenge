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

#include "WP6SingleByteFunction.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6SingleByteFunction * WP6SingleByteFunction::constructSingleByteFunction(GsfInput *input, guint8 groupID)
{
	switch (groupID) 
		{
		case WP6_TOP_SOFT_EOL:
		case WP6_TOP_SOFT_SPACE:
			return new WP6SpaceFunction();
			
		case WP6_TOP_HARD_HYPHEN:
			return new WP6HyphenFunction();

		case WP6_TOP_HARD_EOL:
		case WP6_TOP_DORMANT_HARD_RETURN:
			return new WP6EOLFunction();
			
		// Add the remaining cases here
		default:
			// should not happen
			return NULL;
		}
}

void WP6SpaceFunction::parse(WP6LLListener *llListener)
{
	llListener->insertCharacter((guint16) ' ');
}

void WP6EOLFunction::parse(WP6LLListener *llListener)
{
	llListener->insertEOL();
}

void WP6HyphenFunction::parse(WP6LLListener *llListener)
{
	llListener->insertCharacter((guint16) '-');
}
