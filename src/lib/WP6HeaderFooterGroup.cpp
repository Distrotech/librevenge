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

#include "WP6HeaderFooterGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6HeaderFooterGroup::WP6HeaderFooterGroup(GsfInput *input) :
	WP6VariableLengthGroup()
{
	_read(input);
}

void WP6HeaderFooterGroup::_readContents(GsfInput *input)
{
	m_occurenceBits = gsf_le_read_guint16(input);
}

ParseResult WP6HeaderFooterGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an HeaderFooter group\n"));
	WPXHeaderFooterType type;
	switch (getSubGroup())
	{
	case WP6_HEADER_FOOTER_GROUP_HEADER_A:
		type = HEADER_A;
		break;
	case WP6_HEADER_FOOTER_GROUP_HEADER_B:
		type = HEADER_B;
		break;
	case WP6_HEADER_FOOTER_GROUP_FOOTER_A:
		type = FOOTER_A;
		break;
	case WP6_HEADER_FOOTER_GROUP_FOOTER_B:
		type = FOOTER_B;
		break;
	case WP6_HEADER_FOOTER_GROUP_WATERMARK_A:
		type = WATERMARK_A;
		break;
	case WP6_HEADER_FOOTER_GROUP_WATERMARK_B:
		type = WATERMARK_B;
		break;
	}

	if (getNumPrefixIDs() > 0) // FIXME: perhaps we should throw an exception at some point if this isn't true..
	{
		//llListener->headerFooterGroup(type, m_occurenceBits, getPrefixIDs()[0]);
	}
	
	return PARSE_OK;
}
