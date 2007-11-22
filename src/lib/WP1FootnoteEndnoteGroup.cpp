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

#include "WP1FootnoteEndnoteGroup.h"
#include "libwpd_internal.h"
#include "WP1SubDocument.h"

WP1FootnoteEndnoteGroup::WP1FootnoteEndnoteGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP1VariableLengthGroup(group),
	m_noteType(FOOTNOTE),
	m_noteNumber(0),
	m_subDocument(0)
{
	_read(input, encryption);
}

WP1FootnoteEndnoteGroup::~WP1FootnoteEndnoteGroup()
{
	if (m_subDocument)
		delete m_subDocument;
}

void WP1FootnoteEndnoteGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	int tmpSubDocumentSize = getSize() - 29;	
	uint8_t tmpNoteDefinition = readU8(input, encryption);
	if (tmpNoteDefinition & 0x02)
	{
		m_noteType = ENDNOTE;
		tmpSubDocumentSize = getSize() - 27;
	}

	m_noteNumber = readU16(input, encryption, true);
	input->seek(getSize() - tmpSubDocumentSize - 3, WPX_SEEK_CUR);

	WPD_DEBUG_MSG(("WP1SubDocument subDocumentSize = %i\n", tmpSubDocumentSize));
	if (tmpSubDocumentSize)
		m_subDocument = new WP1SubDocument(input, encryption, tmpSubDocumentSize);
}

void WP1FootnoteEndnoteGroup::parse(WP1Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Footnote Endnote group\n"));
	listener->insertNote(m_noteType, m_subDocument);
}
