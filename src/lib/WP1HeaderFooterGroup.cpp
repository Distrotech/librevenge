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

#include "WP1HeaderFooterGroup.h"
#include "libwpd_internal.h"
#include "WP1SubDocument.h"

WP1HeaderFooterGroup::WP1HeaderFooterGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP1VariableLengthGroup(group),
	m_definition(0),
	m_subDocument(0)
{
	_read(input, encryption);
}

WP1HeaderFooterGroup::~WP1HeaderFooterGroup()
{
}

void WP1HeaderFooterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_definition = readU8(input, encryption);

	int tmpSubDocumentSize = getSize() - 0x13;
	input->seek(18, WPX_SEEK_CUR);
	WPD_DEBUG_MSG(("WP1SubDocument subDocumentSize = %i\n", tmpSubDocumentSize));
	if (tmpSubDocumentSize)
		m_subDocument = new WP1SubDocument(input, encryption, tmpSubDocumentSize);
}

void WP1HeaderFooterGroup::parse(WP1Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a HeaderFooter group\n"));
	listener->headerFooterGroup(m_definition, m_subDocument);
}
