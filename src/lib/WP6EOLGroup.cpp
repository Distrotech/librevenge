/* libwpd2
 * Copyright (C) 2003 Net Integration Technologies
 * Copyright (C) 2002-2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6EOLGroup.h"
#include "WP6LLListener.h"
#include "WPXHLListener.h"
#include "libwpd_internal.h"

#include "WP6Parser.h" // for TableException
#include "WP6FillStylePacket.h" // for the fill packet

WP6EOLGroup::WP6EOLGroup(GsfInput *input) :	
	WP6VariableLengthGroup(),
	m_colSpan(1),
	m_rowSpan(1),
	m_boundFromLeft(false),
	m_boundFromAbove(false),
	
	m_cellFgColor(NULL),
	m_cellBgColor(NULL),
	
	m_cellBorders(0x00)
{
	_read(input);
}

WP6EOLGroup::~WP6EOLGroup()
{
	delete m_cellFgColor;
	delete m_cellBgColor;	
}

void WP6EOLGroup::_readContents(GsfInput *input)
{
	WPD_DEBUG_MSG(("WordPerfect: EOL Group: Reading Embedded Sub-Function Data\n"));
	guint16 sizeDeletableSubFunctionData;
	guint16 startPosition = gsf_input_tell(input);
	sizeDeletableSubFunctionData = gsf_le_read_guint16(input);		
	WPD_DEBUG_MSG(("WordPerfect: EOL Group: Size of Deletable Sub-Function Data: %ld,  Size of Deletable and Non-deletable sub-function data: %ld\n", (long) sizeDeletableSubFunctionData, getSizeNonDeletable()));
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, sizeDeletableSubFunctionData, G_SEEK_CUR));
	while (gsf_input_tell(input) < (startPosition + getSizeNonDeletable()))
	{
		guint8 byte;
		guint16 numBytesToSkip = 0;
		byte = gsf_le_read_guint8(input);
		gsf_off_t startPosition2 = gsf_input_tell(input);
		switch (byte)
		{
			case WP6_EOL_GROUP_ROW_INFORMATION:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: ROW_INFORMATION\n"));
				numBytesToSkip = WP6_EOL_GROUP_ROW_INFORMATION_SIZE;
				break;
			case WP6_EOL_GROUP_CELL_FORMULA:
				guint16 embeddedSubGroupSize;
				embeddedSubGroupSize = gsf_le_read_guint16(input);
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
				break;
			case WP6_EOL_GROUP_CELL_SPANNING_INFORMATION:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_SPANNING_INFORMATION\n"));
				numBytesToSkip = WP6_EOL_GROUP_CELL_SPANNING_INFORMATION_SIZE;
				guint8 numCellsSpannedHorizontally;
				guint8 numCellsSpannedVertically;
				numCellsSpannedHorizontally = gsf_le_read_guint8(input);
				numCellsSpannedVertically = gsf_le_read_guint8(input);
				WPD_DEBUG_MSG(("WordPerfect: num cells spanned (h:%ld, v:%ld)\n", 
						   numCellsSpannedHorizontally, numCellsSpannedVertically));
				if (numCellsSpannedHorizontally >= 128)
					m_boundFromLeft = true;
				else
					m_colSpan = numCellsSpannedHorizontally;
				if (numCellsSpannedVertically >= 128)
					m_boundFromAbove = true;
				else
					m_rowSpan = numCellsSpannedVertically;
				break;
			case WP6_EOL_GROUP_CELL_FILL_COLORS:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_FILL_COLORS\n"));
				numBytesToSkip = WP6_EOL_GROUP_CELL_FILL_COLORS_SIZE;
				guint8 fR, fG, fB, fS;
				guint8 bR, bG, bB, bS;
			
				fR = gsf_le_read_guint8(input);
				fG = gsf_le_read_guint8(input);
				fB = gsf_le_read_guint8(input);
				fS = gsf_le_read_guint8(input);
				bR = gsf_le_read_guint8(input);
				bG = gsf_le_read_guint8(input);
				bB = gsf_le_read_guint8(input);
				bS = gsf_le_read_guint8(input);
			
				m_cellFgColor = new RGBSColor(fR,fG,fB,fS);
				m_cellBgColor = new RGBSColor(bR,bG,bB,bS);
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded FG Color (%i, %i, %i, %i) BG Color (%i, %i, %i, %i)\n",
					       m_cellFgColor->m_r, m_cellFgColor->m_g, m_cellFgColor->m_b, m_cellFgColor->m_s,
					       m_cellBgColor->m_r, m_cellBgColor->m_g, m_cellBgColor->m_b, m_cellBgColor->m_s));
				break;
			case WP6_EOL_GROUP_CELL_LINE_COLOR:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_LINE_COLOR\n"));
				numBytesToSkip = WP6_EOL_GROUP_CELL_LINE_COLOR_SIZE;
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
				m_cellBorders = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
				break;
			case WP6_EOL_GROUP_CELL_RECALCULATION_ERROR_NUMBER:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_RECALCULATION_ERROR_NUMBER\n"));
				numBytesToSkip = WP6_EOL_GROUP_CELL_RECALCULATION_ERROR_NUMBER_SIZE;
				break;
			case WP6_EOL_GROUP_DONT_END_A_PARAGRAPH_STYLE_FOR_THIS_HARD_RETURN:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: DONT_END_A_PARAGRAPH_STYLE_FOR_THIS_HARD_RETURN\n"));
				numBytesToSkip = WP6_EOL_GROUP_DONT_END_A_PARAGRAPH_STYLE_FOR_THIS_HARD_RETURN_SIZE;
				break;
			default:
				// unsupported: shouldn't happen! an error may follow
				// HACK: one document seems to have a completely bogus value within 
				// pre-emptively throw a parsing exception
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: UNKNOWN SUBFUNCTION (%x) (BAD BAD BAD)\n", byte));
				throw ParseException();
		}			
		
		WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, (startPosition2 + numBytesToSkip - 1 - gsf_input_tell(input)), G_SEEK_CUR));
	}
}

void WP6EOLGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an EOL group\n"));

	// first off, grab any prefix information which may be useful
	const RGBSColor * cellFgColor = m_cellFgColor;
	const RGBSColor * cellBgColor = m_cellBgColor;
	for (int i=0; i<getNumPrefixIDs(); i++)
	{
		if (const WP6FillStylePacket *fsPacket = dynamic_cast<const WP6FillStylePacket *>(llListener->getPrefixDataPacket(getPrefixIDs()[i]))) 
		{
			cellFgColor = fsPacket->getFgColor();
			cellBgColor = fsPacket->getBgColor();
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
	case WP6_EOL_GROUP_DELETABLE_SOFT_EOL: // 0x014 (deletable soft EOL)
	case WP6_EOL_GROUP_DELETABLE_SOFT_EOC: // 0x15 (deletable soft EOC) 
	case WP6_EOL_GROUP_DELETABLE_SOFT_EOC_AT_EOP: // 0x16 (deleteable soft EOC at EOP)
		llListener->insertCharacter((guint16) ' ');
		break;
	case WP6_EOL_GROUP_HARD_EOL:
	case WP6_EOL_GROUP_HARD_EOL_AT_EOC:
	case WP6_EOL_GROUP_HARD_EOL_AT_EOP:
	case WP6_EOL_GROUP_DELETABLE_HARD_EOL: // 0x17 (deletable hard EOL)
	case WP6_EOL_GROUP_DELETABLE_HARD_EOL_AT_EOC: // 0x18 (deletable hard EOL at EOC)
	case WP6_EOL_GROUP_DELETABLE_HARD_EOL_AT_EOP: // 0x19 (deletable hard EOL at EOP)
		llListener->insertEOL();
		break;
	case WP6_EOL_CHARACTER_HARD_END_OF_COLUMN: // 0x07 (hard end of column)
		llListener->insertBreak(WPX_COLUMN_BREAK);
		break;
	case WP6_EOL_GROUP_HARD_EOP: // hard EOP
	case WP6_EOL_GROUP_DELETABLE_HARD_EOP: // deletable hard EOP
		llListener->insertBreak(WPX_PAGE_BREAK);
		break;
	case 0x0A: // Table Cell
		WPD_DEBUG_MSG(("WordPerfect: EOL group: table cell\n"));
		llListener->insertCell(m_colSpan, m_rowSpan, m_boundFromLeft, m_boundFromAbove, m_cellBorders, cellFgColor, cellBgColor);
		break;
	case WP6_EOL_GROUP_TABLE_ROW_AND_CELL:
	case WP6_EOL_GROUP_TABLE_ROW_AT_EOC:
	case WP6_EOL_GROUP_TABLE_ROW_AT_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOC:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOC_AT_HARD_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOP:
		WPD_DEBUG_MSG(("WordPerfect: EOL group: table row and cell\n"));
		llListener->insertRow();
		// the cellBorders variable already represent the cell border bits as well
		llListener->insertCell(m_colSpan, m_rowSpan, m_boundFromLeft, m_boundFromAbove, m_cellBorders, cellFgColor, cellBgColor);
		break;
	case WP6_EOL_GROUP_TABLE_OFF:
	case WP6_EOL_GROUP_TABLE_OFF_AT_EOC:
	case WP6_EOL_GROUP_TABLE_OFF_AT_EOC_AT_EOP:
		llListener->endTable();
		break;
	default: // something else we don't support yet
		break;
	}
	// search for soft page breaks and dispatch messages to that effect
	switch(getSubGroup())
	{
	case WP6_EOL_GROUP_SOFT_EOC_AT_EOP: // 0x03 (soft EOC at EOP) 
	case WP6_EOL_GROUP_DELETABLE_SOFT_EOC_AT_EOP: // 0x16 (deleteable soft EOC at EOP)		
	case WP6_EOL_GROUP_HARD_EOL_AT_EOP:
	case WP6_EOL_GROUP_DELETABLE_HARD_EOL_AT_EOP: // 0x19 (deletable hard EOL at EOP)
	case WP6_EOL_GROUP_TABLE_ROW_AT_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOC_AT_HARD_EOP:
	case WP6_EOL_GROUP_TABLE_ROW_AT_HARD_EOP:
	case WP6_EOL_GROUP_TABLE_OFF_AT_EOC_AT_EOP:
		llListener->insertBreak(WPX_SOFT_PAGE_BREAK);			    
	}
}
