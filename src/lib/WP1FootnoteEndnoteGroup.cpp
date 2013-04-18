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
	unsigned tmpSubDocumentSize = getSize() - 29;
	uint8_t tmpNoteDefinition = readU8(input, encryption);
	if (tmpNoteDefinition & 0x02)
	{
		m_noteType = ENDNOTE;
		tmpSubDocumentSize = getSize() - 27;
	}

	m_noteNumber = readU16(input, encryption, true);
	input->seek(getSize() - tmpSubDocumentSize - 3, WPX_SEEK_CUR);

	WPD_DEBUG_MSG(("WP1SubDocument subDocumentSize = %u\n", tmpSubDocumentSize));
	if (tmpSubDocumentSize)
		m_subDocument = new WP1SubDocument(input, encryption, tmpSubDocumentSize);
}

void WP1FootnoteEndnoteGroup::parse(WP1Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Footnote Endnote group\n"));
	listener->insertNote(m_noteType, m_subDocument);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
