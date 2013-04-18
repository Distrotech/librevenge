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

#include "WP42HeaderFooterGroup.h"
#include "libwpd_internal.h"
#include <vector>

WP42HeaderFooterGroup::WP42HeaderFooterGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP42MultiByteFunctionGroup(group),
	m_definition(0),
	m_subDocument(0)
{
	_read(input, encryption);
}

WP42HeaderFooterGroup::~WP42HeaderFooterGroup()
{
}

void WP42HeaderFooterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	input->seek(4, WPX_SEEK_CUR);
	long tmpStartPosition = input->tell();
	while (readU8(input, encryption) != 0xD1)
	{
	}
	input->seek(-3, WPX_SEEK_CUR);
	long tmpSubDocumentSize = 0;
	if (readU8(input, encryption) == 0xFF)
		tmpSubDocumentSize = input->tell() - tmpStartPosition - 1;
	WPD_DEBUG_MSG(("WP42SubDocument startPosition = %li; SubDocumentSize = %li\n", tmpStartPosition, tmpSubDocumentSize));
	input->seek(1, WPX_SEEK_CUR);
	m_definition = readU8(input, encryption);
	input->seek(tmpStartPosition, WPX_SEEK_SET);
	if (tmpSubDocumentSize > 2)
		m_subDocument = new WP42SubDocument(input, encryption, (unsigned)tmpSubDocumentSize);
}

void WP42HeaderFooterGroup::parse(WP42Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a HeaderFooter group\n"));
	listener->headerFooterGroup(m_definition, m_subDocument);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
