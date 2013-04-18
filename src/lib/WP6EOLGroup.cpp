/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002-2003 William Lachance (wrlach@gmail.com)
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

#include "WP6EOLGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"

#include "WP6FillStylePacket.h" // for the fill packet

WP6EOLGroup::WP6EOLGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_colSpan(1),
	m_rowSpan(1),
	m_boundFromAbove(false),

	m_useCellAttributes(false),
	m_useCellJustification(false),
	m_ignoreInCalculations(false),
	m_cellIsLocked(false),
	m_cellAttributes(0),
	m_cellJustification(0),
	m_cellVerticalAlign(TOP),

	m_cellFgColor(0),
	m_cellBgColor(0),
	m_cellBorderColor(new RGBSColor(0x00,0x00,0x00,0x64)),

	m_cellBorders(0x00),

	m_isHeaderRow(false),
	m_isMinimumHeight(true),
	m_rowHeight(0),
	m_isDontEndAParagraphStyleForThisHardReturn(false)
{
	_read(input, encryption);
}

WP6EOLGroup::~WP6EOLGroup()
{
	DELETEP(m_cellFgColor);
	DELETEP(m_cellBgColor);
	DELETEP(m_cellBorderColor);
}

void WP6EOLGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	WPD_DEBUG_MSG(("WordPerfect: EOL Group: Reading Embedded Sub-Function Data\n"));
	long startPosition = input->tell();
	uint16_t sizeDeletableSubFunctionData = readU16(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: EOL Group: Size of Deletable Sub-Function Data: %ld,  Size of Deletable and Non-deletable sub-function data: %ld\n", (long) sizeDeletableSubFunctionData, (long) getSizeNonDeletable()));
	if ((long)sizeDeletableSubFunctionData > (long)getSizeNonDeletable())
	{
		WPD_DEBUG_MSG(("WordPerfect: EOL Group: Possible corruption detected, bailing out\n"));
		throw FileException();
	}

	input->seek(sizeDeletableSubFunctionData, WPX_SEEK_CUR);
	while ((long)input->tell() < (long)(startPosition + getSizeNonDeletable()))
	{
		uint8_t byte;
		uint16_t numBytesToSkip = 0;
		byte = readU8(input, encryption);
		long startPosition2 = input->tell();
		switch (byte)
		{
		case WP6_EOL_GROUP_ROW_INFORMATION:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: ROW_INFORMATION\n"));
			numBytesToSkip = WP6_EOL_GROUP_ROW_INFORMATION_SIZE;
			uint8_t rowFlags;
			rowFlags = readU8(input, encryption);
			if ((rowFlags & 0x04) == 0x04)
				m_isHeaderRow = true;
			if ((rowFlags & 0x02) == 0x02)
			{
				if ((rowFlags & 0x10) == 0x10)
					m_isMinimumHeight = true;
				else
					m_isMinimumHeight = false;
				m_rowHeight = readU16(input, encryption);
			}
			else
			{
				m_isMinimumHeight = true;
				m_rowHeight = 0x0000;
			}
			break;
		case WP6_EOL_GROUP_CELL_FORMULA:
			uint16_t embeddedSubGroupSize;
			embeddedSubGroupSize = readU16(input, encryption);
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_FORMULA (length: %ld)\n",
			               (long) embeddedSubGroupSize));
			numBytesToSkip = embeddedSubGroupSize;
			break;
		case WP6_EOL_GROUP_TOP_GUTTER_SPACING:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: TOP_GUTTER_SPACING\n"));
			numBytesToSkip = WP6_EOL_GROUP_TOP_GUTTER_SPACING_SIZE;
			break;
		case WP6_EOL_GROUP_BOTTOM_GUTTER_SPACING:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: BOTTOM_GUTTER_SPACING\n"));
			numBytesToSkip = WP6_EOL_GROUP_BOTTOM_GUTTER_SPACING_SIZE;
			break;
		case WP6_EOL_GROUP_CELL_INFORMATION:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_INFORMATION\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_INFORMATION_SIZE;
			uint8_t cellFlag, tmpCellVerticalAlign;
			uint16_t attributeWord1, attributeWord2;
			cellFlag = readU8(input, encryption);
			if ((cellFlag & 0x01) == 0x01)
				m_useCellAttributes = true;
			if ((cellFlag & 0x02) == 0x02)
				m_useCellJustification = true;
			if ((cellFlag & 0x40) == 0x40)
				m_ignoreInCalculations = true;
			if ((cellFlag & 0x80) == 0x80)
				m_cellIsLocked = true;
			m_cellJustification = (readU8(input, encryption) & 0x07);
			tmpCellVerticalAlign = readU8(input, encryption);
			switch (tmpCellVerticalAlign & 0x03)
			{
			case 0x00: // top
				m_cellVerticalAlign = TOP;
				break;
			case 0x01: // center
				m_cellVerticalAlign = MIDDLE;
				break;
			case 0x02: // bottom
				m_cellVerticalAlign = BOTTOM;
				break;
			case 0x03: // full
				m_cellVerticalAlign = FULL;
				break;
			default:
				break;
			}
			attributeWord1 = readU16(input, encryption);
			attributeWord2 = readU16(input, encryption);
			m_cellAttributes = (uint32_t)(((attributeWord2 & 0x03) << 16) + attributeWord1);
			break;
		case WP6_EOL_GROUP_CELL_SPANNING_INFORMATION:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_SPANNING_INFORMATION\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_SPANNING_INFORMATION_SIZE;
			m_colSpan = readU8(input, encryption);
			m_rowSpan = readU8(input, encryption);
			WPD_DEBUG_MSG(("WordPerfect: num cells spanned (h:%ld, v:%ld)\n",
			               (long) m_colSpan, (long) m_rowSpan));
			if (m_colSpan >= 128)
				m_boundFromAbove = true;
			break;
		case WP6_EOL_GROUP_CELL_FILL_COLORS:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_FILL_COLORS\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_FILL_COLORS_SIZE;
			uint8_t fR, fG, fB, fS;
			uint8_t bR, bG, bB, bS;

			fR = readU8(input, encryption);
			fG = readU8(input, encryption);
			fB = readU8(input, encryption);
			fS = readU8(input, encryption);
			bR = readU8(input, encryption);
			bG = readU8(input, encryption);
			bB = readU8(input, encryption);
			bS = readU8(input, encryption);

			m_cellFgColor = new RGBSColor(fR,fG,fB,fS);
			m_cellBgColor = new RGBSColor(bR,bG,bB,bS);
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded FG Color (%i, %i, %i, %i) BG Color (%i, %i, %i, %i)\n",
			               m_cellFgColor->m_r, m_cellFgColor->m_g, m_cellFgColor->m_b, m_cellFgColor->m_s,
			               m_cellBgColor->m_r, m_cellBgColor->m_g, m_cellBgColor->m_b, m_cellBgColor->m_s));
			break;
		case WP6_EOL_GROUP_CELL_LINE_COLOR:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_LINE_COLOR\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_LINE_COLOR_SIZE;

			m_cellBorderColor->m_r = readU8(input, encryption);
			m_cellBorderColor->m_g = readU8(input, encryption);
			m_cellBorderColor->m_b = readU8(input, encryption);
			m_cellBorderColor->m_s = readU8(input, encryption);

			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Border Color (%i, %i, %i, %i)\n",
			               m_cellBorderColor->m_r, m_cellBorderColor->m_g, m_cellBorderColor->m_b, m_cellBorderColor->m_s));
			break;
		case WP6_EOL_GROUP_CELL_NUMBER_TYPE:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_NUMBER_TYPE\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_NUMBER_TYPE_SIZE;
			break;
		case WP6_EOL_GROUP_CELL_FLOATING_POINT_NUMBER:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_FLOATING_POINT_NUMBER\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_FLOATING_POINT_NUMBER_SIZE;
			break;
		case WP6_EOL_GROUP_CELL_PREFIX_FLAG:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_PREFIX_FLAG\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_PREFIX_FLAG_SIZE;
			m_cellBorders = readU8(input, encryption);
			break;
		case WP6_EOL_GROUP_CELL_RECALCULATION_ERROR_NUMBER:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_RECALCULATION_ERROR_NUMBER\n"));
			numBytesToSkip = WP6_EOL_GROUP_CELL_RECALCULATION_ERROR_NUMBER_SIZE;
			break;
		case WP6_EOL_GROUP_DONT_END_A_PARAGRAPH_STYLE_FOR_THIS_HARD_RETURN:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: DONT_END_A_PARAGRAPH_STYLE_FOR_THIS_HARD_RETURN\n"));
			numBytesToSkip = WP6_EOL_GROUP_DONT_END_A_PARAGRAPH_STYLE_FOR_THIS_HARD_RETURN_SIZE;
			m_isDontEndAParagraphStyleForThisHardReturn = true;
			break;
		case 0x8e:
		case 0x8f:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: UNKNOWN SUBFUNCTION (%x) (BAD BAD BAD)\n", byte));
			numBytesToSkip = readU16(input, encryption); // It seems that these two unknow sub-functions have their
			// length information embedded: <subfunction>[length]...[length]<subfunction>
			break;
		default:
			WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: UNKNOWN SUBFUNCTION (%x) (BAD BAD BAD)\n", byte));
			WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected, bailing out\n"));
			throw FileException();
		}

		if (startPosition2 + numBytesToSkip - 1 - input->tell() < 0)
			throw FileException();
		input->seek((startPosition2 + numBytesToSkip - 1), WPX_SEEK_SET);
	}
}

void WP6EOLGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an EOL group\n"));

	// first off, grab any prefix information which may be useful
	const RGBSColor *cellFgColor = m_cellFgColor;
	const RGBSColor *cellBgColor = m_cellBgColor;
	const RGBSColor *cellBorderColor = m_cellBorderColor;

	if (!cellFgColor && !cellBgColor)
	{
		for (int i=0; i<getNumPrefixIDs(); i++)
		{
			if (const WP6FillStylePacket *fsPacket = dynamic_cast<const WP6FillStylePacket *>(listener->getPrefixDataPacket(getPrefixIDs()[i])))
			{

				cellFgColor = fsPacket->getFgColor();
				cellBgColor = fsPacket->getBgColor();
			}
		}
	}

	// main search + dispatch for messages
	switch(getSubGroup())
	{
	case 0: // 0x00 (beginning of file)
		break; // ignore
	case WP6_EOL_GROUP_SOFT_EOL:
	case WP6_EOL_GROUP_SOFT_EOC:
	case WP6_EOL_GROUP_SOFT_EOC_AT_EOP: // 0x03 (soft EOC at EOP)
		listener->insertCharacter((uint32_t) ' ');
		break;
	case WP6_EOL_GROUP_DELETABLE_HARD_EOL: // 0x17 (deletable hard EOL)
	case WP6_EOL_GROUP_DELETABLE_HARD_EOL_AT_EOC: // 0x18 (deletable hard EOL at EOC)
	case WP6_EOL_GROUP_DELETABLE_HARD_EOL_AT_EOP: // 0x19 (deletable hard EOL at EOP)
	case WP6_EOL_GROUP_DELETABLE_HARD_EOP: // deletable hard EOP
	case WP6_EOL_GROUP_HARD_EOL:
	case WP6_EOL_GROUP_HARD_EOL_AT_EOC:
	case WP6_EOL_GROUP_HARD_EOL_AT_EOP:
		listener->insertEOL();
		break;
	case WP6_EOL_GROUP_DELETABLE_SOFT_EOL: // 0x014 (deletable soft EOL)
		if (m_isDontEndAParagraphStyleForThisHardReturn)
			listener->handleLineBreak();
		break;
	case WP6_EOL_GROUP_HARD_EOC: // 0x07 (hard end of column)
	case WP6_EOL_GROUP_HARD_EOC_AT_EOP:
	case WP6_EOL_GROUP_DELETABLE_HARD_EOC:
	case WP6_EOL_GROUP_DELETABLE_HARD_EOC_AT_EOP:
		listener->insertBreak(WPX_COLUMN_BREAK);
		break;
	case WP6_EOL_GROUP_HARD_EOP: // hard EOP
		listener->insertBreak(WPX_PAGE_BREAK);
		break;
	case WP6_EOL_GROUP_TABLE_CELL: // Table Cell
		WPD_DEBUG_MSG(("WordPerfect: EOL group: table cell\n"));
		if (!m_boundFromAbove)
		{
			listener->insertCell(m_colSpan, m_rowSpan, m_cellBorders, cellFgColor, cellBgColor,
			                     cellBorderColor, m_cellVerticalAlign, m_useCellAttributes, m_cellAttributes);
			if (m_useCellJustification)
				listener->justificationChange(m_cellJustification);
		}
		break;
	case WP6_EOL_GROUP_TABLE_ROW_AND_CELL:
	case WP6_EOL_GROUP_TABLE_ROW_AT_EOC:
	case WP6_EOL_GROUP_TABLE_ROW_AT_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOC:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOC_AT_HARD_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOP:
		WPD_DEBUG_MSG(("WordPerfect: EOL group: table row and cell\n"));

		listener->insertRow(m_rowHeight, m_isMinimumHeight, m_isHeaderRow);
		// the cellBorders variable already represent the cell border bits as well
		if (!m_boundFromAbove)
		{
			listener->insertCell(m_colSpan, m_rowSpan, m_cellBorders, cellFgColor, cellBgColor,
			                     cellBorderColor, m_cellVerticalAlign, m_useCellAttributes, m_cellAttributes);
			if (m_useCellJustification)
				listener->justificationChange(m_cellJustification);
		}
		break;
	case WP6_EOL_GROUP_TABLE_OFF:
	case WP6_EOL_GROUP_TABLE_OFF_AT_EOC:
	case WP6_EOL_GROUP_TABLE_OFF_AT_EOC_AT_EOP:
		listener->endTable();
		break;
//		case WP6_EOL_GROUP_DELETABLE_SOFT_EOC: // 0x15 (deletable soft EOC)
//		case WP6_EOL_GROUP_DELETABLE_SOFT_EOC_AT_EOP: // 0x16 (deleteable soft EOC at EOP)
	default: // something else we don't support yet
		break;
	}

	// search for soft page breaks and dispatch messages to that effect
	switch(getSubGroup())
	{
	case WP6_EOL_GROUP_HARD_EOL_AT_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOC_AT_HARD_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOP:
	case WP6_EOL_GROUP_TABLE_OFF_AT_EOC_AT_EOP:
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
