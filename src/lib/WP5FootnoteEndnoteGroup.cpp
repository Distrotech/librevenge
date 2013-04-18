/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

	m_subDocument = new WP5SubDocument(input, encryption, (unsigned)tmpSizeOfNote);
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
