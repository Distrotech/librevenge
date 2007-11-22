/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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
#include "WP3FootnoteEndnoteGroup.h"
#include "WP3FileStructure.h"
#include "WP3Parser.h"

WP3FootnoteEndnoteGroup::WP3FootnoteEndnoteGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_subDocument(0)
{
	_read(input, encryption);
}

WP3FootnoteEndnoteGroup::~WP3FootnoteEndnoteGroup()
{
	delete m_subDocument;
}

void WP3FootnoteEndnoteGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	int tmpSizeOfNote = getSize() - 8;
	input->seek(25, WPX_SEEK_CUR);
	tmpSizeOfNote -= 25;
	unsigned tmpNumOfPages = readU16(input, encryption, true);
	tmpSizeOfNote -= 2;
	input->seek(4*tmpNumOfPages, WPX_SEEK_CUR);
	tmpSizeOfNote -= 4*tmpNumOfPages;
	unsigned tmpNumBreakTableEntries = readU16(input, encryption, true);
	tmpSizeOfNote -= 2;
	input->seek(6*tmpNumBreakTableEntries, WPX_SEEK_CUR);
	tmpSizeOfNote -= 6*tmpNumBreakTableEntries;

	// here we skipped all the useless junk and we are at the beginning of the
	// actual subdocument. tmpSizeOfNote should give the size in bytes of the
	// subdocument

	if (tmpSizeOfNote > 0)
		m_subDocument = new WP3SubDocument(input, encryption, tmpSizeOfNote);
}

void WP3FootnoteEndnoteGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Footnote/Endnote group\n"));
	switch (getSubGroup())
	{
	case WP3_FOOTNOTE_ENDNOTE_GROUP_FOOTNOTE_FUNCTION:
		listener->insertNote(FOOTNOTE, m_subDocument);
		break;
	case WP3_FOOTNOTE_ENDNOTE_GROUP_ENDNOTE_FUNCTION:
		listener->insertNote(ENDNOTE, m_subDocument);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
