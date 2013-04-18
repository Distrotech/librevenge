/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
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
		m_subDocument = new WP3SubDocument(input, encryption, (unsigned)tmpSizeOfNote);
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
