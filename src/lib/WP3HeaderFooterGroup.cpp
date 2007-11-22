/* libwpd
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3HeaderFooterGroup.h"
#include "libwpd_internal.h"
#include "WP3FileStructure.h"

WP3HeaderFooterGroup::WP3HeaderFooterGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_definition(0),
	m_subDocument(0)
{
	_read(input, encryption);
}

WP3HeaderFooterGroup::~WP3HeaderFooterGroup()
{
}

void WP3HeaderFooterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	if (getSubGroup() <= WP3_HEADER_FOOTER_GROUP_FOOTER_B)  // omit watermarks for the while
	{
		input->seek(14, WPX_SEEK_CUR);
		uint16_t tmpSubDocumentLength = readU16(input, encryption, true);  // read first the old subdocument length
		input->seek(tmpSubDocumentLength, WPX_SEEK_CUR);  // and skip the old subdocument
		m_definition = readU8(input, encryption);
		input->seek(4, WPX_SEEK_CUR);
		tmpSubDocumentLength = readU16(input, encryption, true);
		if (tmpSubDocumentLength)
			m_subDocument = new WP3SubDocument(input, encryption, tmpSubDocumentLength);
	}
}

void WP3HeaderFooterGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a HeaderFooter group\n"));

	if (getSubGroup() <= WP3_HEADER_FOOTER_GROUP_FOOTER_B)  // omit watermarks for the while
	{
		listener->headerFooterGroup(getSubGroup(), m_definition, m_subDocument);
	}
}
