/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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

WP6EOLGroup::WP6EOLGroup(FILE *stream) :	
	WP6VariableLengthGroup(),
	m_colSpan(1),
	m_rowSpan(1),
	m_boundFromLeft(FALSE),
	m_boundFromAbove(FALSE),
	
	m_cellFgColor(NULL),
	m_cellBgColor(NULL)
{
	_read(stream);
}

void WP6EOLGroup::_readContents(FILE *stream)
{
	WPD_DEBUG_MSG(("WordPerfect: EOL Group: Reading Embedded Sub-Function Data\n"));
	guint16 sizeDeletableSubFunctionData;
	guint16 startPosition = ftell(stream);
	WPD_CHECK_FILE_READ_ERROR(fread(&sizeDeletableSubFunctionData, sizeof(guint16), 1, stream), 1);	
	WPD_DEBUG_MSG(("WordPerfect: EOL Group: Size of Deletable Sub-Function Data: %ld,  Size of Deletable and Non-deletable sub-function data: %ld\n", (long) sizeDeletableSubFunctionData, getSizeNonDeletable()));
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, sizeDeletableSubFunctionData, SEEK_CUR));
	while (ftell(stream) < (startPosition + getSizeNonDeletable()))
	{
		guint8 byte;
		guint16 numBytesToSkip = 0;
		WPD_CHECK_FILE_READ_ERROR(fread(&byte, sizeof(guint8), 1, stream), 1);
		guint16 startPosition2 = ftell(stream);
		switch (byte)
		{
			case WP6_EOL_GROUP_ROW_INFORMATION:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: ROW_INFORMATION\n"));
				numBytesToSkip = WP6_EOL_GROUP_ROW_INFORMATION_SIZE;
				break;
			case WP6_EOL_GROUP_CELL_FORMULA:
				guint16 embeddedSubGroupSize;
				WPD_CHECK_FILE_READ_ERROR(fread(&embeddedSubGroupSize, sizeof(guint16), 1, stream), 1);
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
				WPD_CHECK_FILE_READ_ERROR(fread(&numCellsSpannedHorizontally, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&numCellsSpannedVertically, sizeof(guint8), 1, stream), 1);
				WPD_DEBUG_MSG(("WordPerfect: num cells spanned (h:%ld, v:%ld)\n", 
						   numCellsSpannedHorizontally, numCellsSpannedVertically));
				if (numCellsSpannedHorizontally >= 128)
					m_boundFromLeft = TRUE;
				else
					m_colSpan = numCellsSpannedHorizontally;
				if (numCellsSpannedVertically >= 128)
					m_boundFromAbove = TRUE;
				else
					m_rowSpan = numCellsSpannedVertically;
				break;
			case WP6_EOL_GROUP_CELL_FILL_COLORS:
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: CELL_FILL_COLORS\n"));
				numBytesToSkip = WP6_EOL_GROUP_CELL_FILL_COLORS_SIZE;
				guint8 fR, fG, fB, fS;
				guint8 bR, bG, bB, bS;
			
				WPD_CHECK_FILE_READ_ERROR(fread(&fR, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&fG, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&fB, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&fS, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&bR, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&bG, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&bB, sizeof(guint8), 1, stream), 1);
				WPD_CHECK_FILE_READ_ERROR(fread(&bS, sizeof(guint8), 1, stream), 1);
			
				m_cellFgColor = new RGBSColor(fR,fG,fB,fS);
				m_cellBgColor = new RGBSColor(bR,bG,bB,bS);
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
				// unsupported: shouldn't happen! an error is likely to follow: 
				// pre-emptively throw a parsing exception
				WPD_DEBUG_MSG(("WordPerfect: EOL Group Embedded Sub-Function: UNKNOWN SUBFUNCTION (BAD BAD BAD)\n"));
				throw ParseException();
		}			
		
		WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, (startPosition2 + numBytesToSkip - 1 - ftell(stream)), SEEK_CUR));
	}
}

void WP6EOLGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an EOL group\n"));
	   
	/* WL FIXME: am I handling all the special cases properly here? quite possibly not */
	//if(!wordperfect_state->undo_on)/* && m_paragraphStyleState != beginPart2BeforeNumbering && 
		//			m_paragraphStyleState != beginPart2Numbering && 
				//	m_paragraphStyleState != end) */
	{
		switch(getSubGroup())
		{
			case 0: // 0x00 (beginning of file)
				break; // ignore
			case 1: // 0x01 (soft EOL)
			case 2: // 0x02 (soft EOC) 
			case 3: // 0x03 (soft EOC at EOP) 
			case 20: // 0x014 (deletable soft EOL)
			case 21: // 0x15 (deletable soft EOC) 
			case 22: // 0x16 (deleteable soft EOC at EOP)
				llListener->insertCharacter((guint16) ' ');
				break;
			case 4: // 0x04 (hard end-of-line)
			case 5: // 0x05 (hard EOL at EOC) 
			case 6: // 0x06 (hard EOL at EOP)
			case 23: // 0x17 (deletable hard EOL)
			case 24: // 0x18 (deletable hard EOL at EOC)
			case 25: // 0x19 (deletable hard EOL at EOP)
				llListener->insertEOL();
				break;
			case WP6_EOL_CHARACTER_HARD_END_OF_COLUMN: // 0x07 (hard end of column)
				llListener->insertBreak(WPX_COLUMN_BREAK);
				break;
			case 9: // hard EOP
			case 28: // deletable hard EOP
				llListener->insertBreak(WPX_PAGE_BREAK);
				break;
			case 0x0A: // Table Cell
				WPD_DEBUG_MSG(("WordPerfect: EOL group: table cell\n"));
				llListener->insertCell(m_colSpan, m_rowSpan, m_boundFromLeft, m_boundFromAbove, m_cellFgColor, m_cellBgColor);
				break;
			case 0x0B: // Table Row and Cell
				WPD_DEBUG_MSG(("WordPerfect: EOL group: table row and cell\n"));
				llListener->insertRow();
				llListener->insertCell(m_colSpan, m_rowSpan, m_boundFromLeft, m_boundFromAbove, m_cellFgColor, m_cellBgColor);
				break;
			case 0x11: // Table Off
				llListener->endTable();
				break;
			    
			default: // something else we don't support yet
				break;
		}
	}
}
