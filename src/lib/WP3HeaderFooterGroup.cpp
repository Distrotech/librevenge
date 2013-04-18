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

#include "WP3HeaderFooterGroup.h"
#include "libwpd_internal.h"
#include "WP3FileStructure.h"

WP3HeaderFooterGroup::WP3HeaderFooterGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_definition(0),
	m_subDocument(0)
{
	_read(input, encryption);
}

WP3HeaderFooterGroup::~WP3HeaderFooterGroup()
{
}

void WP3HeaderFooterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	if (getSubGroup() <= WP3_HEADER_FOOTER_GROUP_FOOTER_B)  // omit watermarks for the while
	{
		input->seek(14, WPX_SEEK_CUR);
		uint16_t tmpSubDocumentLength = readU16(input, encryption, true);  // read first the old subdocument length
		input->seek(tmpSubDocumentLength, WPX_SEEK_CUR);  // and skip the old subdocument
		m_definition = readU8(input, encryption);
		input->seek(4, WPX_SEEK_CUR);
		tmpSubDocumentLength = readU16(input, encryption, true);
		if (tmpSubDocumentLength)
			m_subDocument = new WP3SubDocument(input, encryption, tmpSubDocumentLength);
	}
}

void WP3HeaderFooterGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a HeaderFooter group\n"));

	if (getSubGroup() <= WP3_HEADER_FOOTER_GROUP_FOOTER_B)  // omit watermarks for the while
	{
		listener->headerFooterGroup(getSubGroup(), m_definition, m_subDocument);
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
