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
#include "WP3TablesGroup.h"
#include "WP3FileStructure.h"
#include "WP3Listener.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"

WP3TablesGroup::WP3TablesGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_tableMode(0),
	m_offsetFromLeftEdge(0),
	m_topGutterSpacing(0),
	m_leftGutterSpacing(0),
	m_bottomGutterSpacing(0),
	m_rightGutterSpacing(0),
	m_numColumns(0),
	m_columnMode(32),
	m_numberFormat(32),
	m_columnWidth(32),
	m_rightOffsetForDecimalAlign(32),
	m_colSpan(0),
	m_rowSpan(0),
	m_cellFillColor(RGBSColor(0xff, 0xff, 0xff))
{
	_read(input, encryption);
}

WP3TablesGroup::~WP3TablesGroup()
{
}

void WP3TablesGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	uint8_t i;
	long startPosition = 0;
	switch (getSubGroup())
	{
	case WP3_TABLES_GROUP_TABLE_FUNCTION:
		startPosition = input->tell();
		input->seek(71, WPX_SEEK_CUR);
		m_tableMode = readU8(input, encryption);
		m_offsetFromLeftEdge = readU32(input, encryption, true);
		m_topGutterSpacing = readU32(input, encryption, true);
		m_leftGutterSpacing = readU32(input, encryption, true);
		m_bottomGutterSpacing = readU32(input, encryption, true);
		m_rightGutterSpacing = readU32(input, encryption, true);
		input->seek(3, WPX_SEEK_CUR);
		m_numColumns = readU8(input, encryption);
		if ((m_numColumns > 32) || ((input->tell() - startPosition + m_numColumns*10) > (getSize() - 4)))
			throw FileException();
		for (i=0; i<m_numColumns; i++)
		{
			if (input->atEOS())
				throw FileException();
			m_columnMode[i] = readU8(input, encryption);
			m_numberFormat[i] = readU8(input, encryption);
			m_columnWidth[i] = readU32(input, encryption, true);
			m_rightOffsetForDecimalAlign[i] = readU32(input, encryption, true);
		}
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_SPAN:
		m_colSpan = readU16(input, encryption, true);
		m_rowSpan = readU16(input, encryption, true);
		m_colSpan++;
		m_rowSpan++;
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_TOP_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_LEFT_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_BOTTOM_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_RIGHT_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_TOP_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_LEFT_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_BOTTOM_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_RIGHT_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_FILL_COLOR_PATTERN:
	{
		uint16_t tmpRed = readU16(input, encryption, true);
		uint16_t tmpGreen = readU16(input, encryption, true);
		uint16_t tmpBlue = readU16(input, encryption, true);
		m_cellFillColor = RGBSColor(tmpRed, tmpGreen, tmpBlue);
	}
	break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_VERTICAL_ALIGNMENT:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_MODE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_ROW_MODE:
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3TablesGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Tables group\n"));

	uint8_t i;
	switch (getSubGroup())
	{
	case WP3_TABLES_GROUP_TABLE_FUNCTION:
		listener->defineTable(m_tableMode, fixedPointToWPUs(m_offsetFromLeftEdge));
		for (i=0; i<m_numColumns && i < 32; i++)
			listener->addTableColumnDefinition(fixedPointToWPUs(m_columnWidth[i]), fixedPointToWPUs(m_leftGutterSpacing),
			                                   fixedPointToWPUs(m_rightGutterSpacing), 0, LEFT);
		listener->startTable();
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_SPAN:
		listener->setTableCellSpan(m_colSpan, m_rowSpan);
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_TOP_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_LEFT_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_BOTTOM_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_RIGHT_LINE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_TOP_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_LEFT_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_BOTTOM_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_RIGHT_LINE_COLOR:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_FILL_COLOR_PATTERN:
		listener->setTableCellFillColor(&m_cellFillColor);
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_VERTICAL_ALIGNMENT:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_CELL_MODE:
		break;
	case WP3_TABLES_GROUP_SET_TABLE_ROW_MODE:
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
