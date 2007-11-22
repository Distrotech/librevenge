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

#include "WP5HeaderFooterGroup.h"
#include "libwpd_internal.h"

WP5HeaderFooterGroup::WP5HeaderFooterGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup(),
	m_occurenceBits(0),
	m_subDocument(0)
{
	_read(input, encryption);
}

WP5HeaderFooterGroup::~WP5HeaderFooterGroup()
{
}

void WP5HeaderFooterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	int tmpSubDocumentLength = getSize() - 26;
	WPD_DEBUG_MSG(("WordPerfect: reading HeaderFooter group. SubDocument size: %i\n", tmpSubDocumentLength));
	input->seek(7, WPX_SEEK_CUR);
	m_occurenceBits = readU8(input, encryption);
	if (m_occurenceBits)
	{
		input->seek(10, WPX_SEEK_CUR);
		if (tmpSubDocumentLength)
			m_subDocument = new WP5SubDocument(input, encryption, tmpSubDocumentLength);
	}
}

void WP5HeaderFooterGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a HeaderFooter group\n"));

	listener->headerFooterGroup(getSubGroup(), m_occurenceBits, m_subDocument);
}
