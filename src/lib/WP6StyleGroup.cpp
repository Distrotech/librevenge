/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP6StyleGroup.h"
#include "libwpd_internal.h"

WP6StyleGroup_GlobalOnSubGroup::WP6StyleGroup_GlobalOnSubGroup(WPXInputStream *input)
{
	m_hash = readU16(input);
	m_systemStyleNumber = readU8(input);
}

void WP6StyleGroup_GlobalOnSubGroup::parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const
{
	listener->globalOn(m_systemStyleNumber);
}

WP6StyleGroup::WP6StyleGroup(WPXInputStream *input) :
	WP6VariableLengthGroup(),
	m_subGroupData(NULL)
{
	_read(input);
}

WP6StyleGroup::~WP6StyleGroup()
{
	if (m_subGroupData)
		delete(m_subGroupData);

}

void WP6StyleGroup::_readContents(WPXInputStream *input)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())	
	{
	case WP6_STYLE_GROUP_GLOBAL_ON:
		m_subGroupData = new WP6StyleGroup_GlobalOnSubGroup(input);
		break;
	}
}

void WP6StyleGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a style group\n"));

	if (getSubGroup() == WP6_STYLE_GROUP_GLOBAL_ON)
	{
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
	}
	else if (getSubGroup() == WP6_STYLE_GROUP_GLOBAL_OFF)
	{
		listener->globalOff();
	}
	else
	{
		if (!(getSubGroup() % 2) || getSubGroup() == 0)
			listener->styleGroupOn(getSubGroup());
		else
			listener->styleGroupOff(getSubGroup());
	}
}
