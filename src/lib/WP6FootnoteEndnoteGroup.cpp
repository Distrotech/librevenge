/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
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

#include "WP6FootnoteEndnoteGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6FootnoteEndnoteGroup::WP6FootnoteEndnoteGroup(GsfInput *input) :
	WP6VariableLengthGroup()
{
	_read(input);
}

void WP6FootnoteEndnoteGroup::_readContents(GsfInput *input)
{
}

void WP6FootnoteEndnoteGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a footnote/endnote group\n"));
	
	if (!(getSubGroup() % 2) || getSubGroup() == 0) 
	{
		if (getNumPrefixIDs() < 1)
			throw ParseException();

		llListener->footnoteEndnoteGroupOn(getSubGroup(), getPrefixIDs()[0]);
	}
	else
		llListener->footnoteEndnoteGroupOff(getSubGroup());

}
