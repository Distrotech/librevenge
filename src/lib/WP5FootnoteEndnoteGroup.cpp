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

#include <math.h>
#include "WP5FootnoteEndnoteGroup.h"
#include "WP5FileStructure.h"
#include "WP5Parser.h"

WP5FootnoteEndnoteGroup::WP5FootnoteEndnoteGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup(),
	m_subDocument(0),
	m_noteReference()
{
	_read(input, encryption);
}

WP5FootnoteEndnoteGroup::~WP5FootnoteEndnoteGroup()
{
	delete m_subDocument;
}

void WP5FootnoteEndnoteGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	int tmpSizeOfNote = getSize() - 8;
	uint8_t tmpFlags = readU8(input, encryption);
	tmpSizeOfNote -= 1;
	int tmpNumOfNote = readU16(input, encryption);
	tmpSizeOfNote -= 2;
	if (getSubGroup() == WP5_FOOTNOTE_ENDNOTE_GROUP_FOOTNOTE)
	{
		int tmpNumOfAdditionalPages = readU8(input, encryption);
		tmpSizeOfNote -= 1;
		input->seek(2*(tmpNumOfAdditionalPages+1) + 9, WPX_SEEK_CUR);
		tmpSizeOfNote -= 2*(tmpNumOfAdditionalPages+1) + 9;
	}
	else /* WP5_FOOTNOTE_ENDNOTE_GROUP_ENDNOTE */
	{
		input->seek(4, WPX_SEEK_CUR);
		tmpSizeOfNote -= 4;
	}		
	
	m_subDocument = new WP5SubDocument(input, encryption, tmpSizeOfNote);
	if (tmpFlags & 0x80)
		m_noteReference.sprintf("%c", tmpNumOfNote);
	else
		m_noteReference.sprintf("%i", tmpNumOfNote);
}

void WP5FootnoteEndnoteGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Footnote/Endnote group\n"));
	listener->insertNoteReference(m_noteReference);
	switch (getSubGroup())
	{
	case WP5_FOOTNOTE_ENDNOTE_GROUP_FOOTNOTE:
		listener->insertNote(FOOTNOTE, m_subDocument);
		break;
	case WP5_FOOTNOTE_ENDNOTE_GROUP_ENDNOTE:
		listener->insertNote(ENDNOTE, m_subDocument);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
