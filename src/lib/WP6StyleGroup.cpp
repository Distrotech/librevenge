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

#include "WP6StyleGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6StyleGroup_GlobalOnSubGroup::WP6StyleGroup_GlobalOnSubGroup(GsfInput *input)
{
	m_hash = gsf_le_read_guint16(input);
	m_systemStyleNumber = gsf_le_read_guint8(input);
}

ParseResult WP6StyleGroup_GlobalOnSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	llListener->globalOn(m_systemStyleNumber);
	return PARSE_OK;
}

WP6StyleGroup::WP6StyleGroup(GsfInput *input) :
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

void WP6StyleGroup::_readContents(GsfInput *input)
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

ParseResult WP6StyleGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a style group\n"));

	if (getSubGroup() == WP6_STYLE_GROUP_GLOBAL_ON)
		return m_subGroupData->parse(llListener, getNumPrefixIDs(), getPrefixIDs());
	else if (getSubGroup() == WP6_STYLE_GROUP_GLOBAL_OFF)
		llListener->globalOff();
	else {
		if (!(getSubGroup() % 2) || getSubGroup() == 0)
			llListener->styleGroupOn(getSubGroup());
		else
			llListener->styleGroupOff(getSubGroup());
	}

	return PARSE_OK;
}
