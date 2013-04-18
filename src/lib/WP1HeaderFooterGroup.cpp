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

	unsigned tmpSubDocumentSize = getSize() - 0x13;
	input->seek(18, WPX_SEEK_CUR);
	WPD_DEBUG_MSG(("WP1SubDocument subDocumentSize = %u\n", tmpSubDocumentSize));
	if (tmpSubDocumentSize)
		m_subDocument = new WP1SubDocument(input, encryption, tmpSubDocumentSize);
}

void WP1HeaderFooterGroup::parse(WP1Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a HeaderFooter group\n"));
	listener->headerFooterGroup(m_definition, m_subDocument);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
