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
		if (tmpSubDocumentLength > 0)
			m_subDocument = new WP5SubDocument(input, encryption, (unsigned)tmpSubDocumentLength);
	}
}

void WP5HeaderFooterGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a HeaderFooter group\n"));

	listener->headerFooterGroup(getSubGroup(), m_occurenceBits, m_subDocument);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
