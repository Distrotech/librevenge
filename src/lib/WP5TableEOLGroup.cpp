/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5TableEOLGroup.h"
#include "WP5FileStructure.h"
#include "WP5Listener.h"
#include "libwpd_internal.h"

WP5TableEOLGroup::WP5TableEOLGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup(),
	m_cellVerticalAlignment(0),
	m_useCellAttributes(false),
	m_useCellJustification(false),
	m_columnNumber(0),
	m_colSpan(0),
	m_rowSpan(0),
	m_spannedFromAbove(false),
	m_cellAttributes(0),
	m_cellJustification(0)
{
	_read(input, encryption);
}

WP5TableEOLGroup::~WP5TableEOLGroup()
{
}

void WP5TableEOLGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	uint8_t tmpFlags, tmpColumnSpanning;
	switch (getSubGroup())
	{
	case WP5_TABLE_EOL_GROUP_BEGINNING_OF_COLUMN_AT_EOL:
		tmpFlags = readU8(input, encryption);
		if ((tmpFlags & 0x01) == 0x01)
			m_useCellJustification = true;
		if ((tmpFlags & 0x02) == 0x02)
			m_useCellAttributes = true;
		m_cellVerticalAlignment = (uint8_t)((tmpFlags & 0x0C) >> 2);
		m_columnNumber = readU8(input, encryption);
		tmpColumnSpanning = readU8(input, encryption);
		m_colSpan = tmpColumnSpanning & 0x7F;
		if ((tmpColumnSpanning & 0x80) == 0x80)
			m_spannedFromAbove = true;
		m_rowSpan = readU8(input, encryption);
		input->seek(4, WPX_SEEK_CUR);
		m_cellAttributes = readU16(input, encryption);
		m_cellJustification = readU8(input, encryption);
		break;
	case WP5_TABLE_EOL_GROUP_BEGINNING_OF_ROW_AT_EOL:
		break;
	case WP5_TABLE_EOL_GROUP_TABLE_OFF_AT_EOL:
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP5TableEOLGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Table EOL group\n"));

	switch (getSubGroup())
	{
	case WP5_TABLE_EOL_GROUP_BEGINNING_OF_COLUMN_AT_EOL:
		if (!m_spannedFromAbove)
		{
			RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
			listener->insertCell(m_colSpan, m_rowSpan, 0x00, 0, 0, &tmpCellBorderColor ,
			                     TOP, m_useCellAttributes, m_cellAttributes);
		}
		break;
	case WP5_TABLE_EOL_GROUP_BEGINNING_OF_ROW_AT_EOL:
		listener->insertRow(0, true, false);
		break;
	case WP5_TABLE_EOL_GROUP_TABLE_OFF_AT_EOL:
		listener->endTable();
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
