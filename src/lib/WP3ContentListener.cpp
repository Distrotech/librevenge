/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP3ContentListener.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP3SubDocument.h"
#include "WPDocument.h"
#include <algorithm>
#include <limits>

_WP3ContentParsingState::_WP3ContentParsingState():
	m_colSpan(1),
	m_rowSpan(1),
	m_textBuffer(),
	m_cellFillColor(0),
	m_noteReference(),
	m_tableList()
{
}

_WP3ContentParsingState::~_WP3ContentParsingState()
{
	DELETEP(m_cellFillColor);
}

WP3ContentListener::WP3ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP3SubDocument *>&subDocuments, WPXDocumentInterface *documentInterface) :
	WP3Listener(),
	WPXContentListener(pageList, documentInterface),
	m_parseState(new WP3ContentParsingState),
	m_subDocuments(subDocuments)
{
}

WP3ContentListener::~WP3ContentListener() 
{
	delete m_parseState;
}


/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP3ContentListener::insertCharacter(const uint16_t character)
{
	if (!isUndoOn())
	{
		uint16_t tmpCharacter = _mapNonUnicodeCharacter(character);
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		appendUCS4(m_parseState->m_textBuffer, (uint32_t)tmpCharacter);
	}
}

void WP3ContentListener::insertTab()
{
        if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			if (m_ps->m_tabStops.empty() || (_getNextTabStop() == (std::numeric_limits<double>::max)()))
				m_ps->m_textIndentByTabs += 0.5f;
			else
				m_ps->m_textIndentByTabs = _getNextTabStop() - (m_ps->m_leftMarginByTabs  + m_ps->m_textIndentByParagraphIndentChange);
			m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
				+ m_ps->m_textIndentByTabs;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
				+ m_ps->m_leftMarginByParagraphMarginChange + m_ps->m_leftMarginByTabs;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
				+ m_ps->m_rightMarginByParagraphMarginChange + m_ps->m_rightMarginByTabs;
			
			m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
		}
		else
		{
			if (!m_ps->m_isSpanOpened)
				_openSpan();
			else
				_flushText();
			m_documentInterface->insertTab();
		}
	}
}

void WP3ContentListener::insertTab(const uint8_t tabType, const double /* tabPosition */)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			switch (tabType)
			{
			case WP3_TAB_GROUP_CENTER:
				m_ps->m_tempParagraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
				return;
			case WP3_TAB_GROUP_FLUSH_RIGHT:
				m_ps->m_tempParagraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
				return;
			default:
				insertTab();
				return;
			}
		}
		else
			insertTab();
	}
}

void WP3ContentListener::insertEOL()
{
	if (!isUndoOn())
	{
		if (m_ps->m_isTableOpened)
		{
			if (!m_ps->m_isTableRowOpened)
				insertRow();
			
			if (!m_ps->m_isTableCellOpened)
			{
				insertCell();
			}
		}
			
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
			_openSpan();
		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();
	}

}

void WP3ContentListener::defineTable(const uint8_t position, const uint16_t leftOffset)
{
	if (!isUndoOn())
	{
		switch (position & 0x07)
		{
		case 0:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN;
			break;
		case 1:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_CENTER_BETWEEN_MARGINS;
			break;
		case 2:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN;
			break;
		case 3:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_FULL;
			break;
		case 4:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ABSOLUTE_FROM_LEFT_MARGIN;
			break;
		default:
			// should not happen
			break;
		}
		// Note: WordPerfect has an offset from the left edge of the page. We translate it to the offset from the left margin
		m_ps->m_tableDefinition.m_leftOffset = _movePositionToFirstColumn( (double)((double)leftOffset / (double)WPX_NUM_WPUS_PER_INCH) ) - m_ps->m_paragraphMarginLeft;

		// remove all the old column information
		m_ps->m_tableDefinition.m_columns.clear();
		m_ps->m_tableDefinition.m_columnsProperties.clear();
		m_ps->m_numRowsToSkip.clear();
	}
}

void WP3ContentListener::addTableColumnDefinition(const uint32_t width, const uint32_t /* leftGutter */,
						const uint32_t /* rightGutter */, const uint32_t attributes, const uint8_t alignment)
{
	if (!isUndoOn())
	{
		// define the new column
		WPXColumnDefinition colDef;
		colDef.m_width = (double)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_leftGutter = (double)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_rightGutter = (double)((double)width / (double)WPX_NUM_WPUS_PER_INCH);

		// add the new column definition to our table definition
		m_ps->m_tableDefinition.m_columns.push_back(colDef);
		
		WPXColumnProperties colProp;
		colProp.m_attributes = attributes;
		colProp.m_alignment = alignment;
		
		m_ps->m_tableDefinition.m_columnsProperties.push_back(colProp);
		
		// initialize the variable that tells us how many row to skip
		m_ps->m_numRowsToSkip.push_back(0);
	}
}

void WP3ContentListener::startTable()
{
	if (!isUndoOn())
	{
		// save the justification information. We will need it after the table ends.
		m_ps->m_paragraphJustificationBeforeTable = m_ps->m_paragraphJustification;
		if (m_ps->m_sectionAttributesChanged && !m_ps->m_isTableOpened && !m_ps->m_inSubDocument)
		{
			_closeSection();
			_openSection();
			m_ps->m_sectionAttributesChanged = false;
		}
		if (!m_ps->m_isPageSpanOpened && !m_ps->m_inSubDocument)
		{
			_openPageSpan();
			_openSection();
		}
		_openTable();
	}
}

void WP3ContentListener::insertRow()
{
	if (!isUndoOn())
		_openTableRow(0, true, false);
}

void WP3ContentListener::insertCell()
{
	if (!isUndoOn())
	{
		if (m_ps->m_currentTableRow < 0) // cell without a row, invalid
			throw ParseException();
		
		RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
		_openTableCell((uint8_t)m_parseState->m_colSpan, (uint8_t)m_parseState->m_rowSpan, 0x00000000,       
				 m_parseState->m_cellFillColor, 0, &tmpCellBorderColor, TOP);
		DELETEP(m_parseState->m_cellFillColor);

		m_ps->m_isCellWithoutParagraph = true;
		m_ps->m_cellAttributeBits = 0x00000000;
	}
}

void WP3ContentListener::closeCell()
{
	if (!isUndoOn())
	{
		insertEOL();
		_closeTableCell();
		m_parseState->m_rowSpan = 1;
		m_parseState->m_colSpan = 1;
	}
}

void WP3ContentListener::closeRow()
{
	if (!isUndoOn())
	{
		closeCell();

		_closeTableRow();
	}
}

void WP3ContentListener::setTableCellSpan(const uint16_t colSpan, const uint16_t rowSpan)
{
	if (!isUndoOn())
	{
		m_parseState->m_colSpan=colSpan;
		m_parseState->m_rowSpan=rowSpan;
	}
}

void WP3ContentListener::setTableCellFillColor(const RGBSColor * cellFillColor)
{
	if (!isUndoOn())
	{
		if (m_parseState->m_cellFillColor)
			DELETEP(m_parseState->m_cellFillColor);
		m_parseState->m_cellFillColor = new RGBSColor(*cellFillColor);
	}
}

void WP3ContentListener::endTable()
{
	if (!isUndoOn())
	{
		_flushText();
		_closeTable();
		// restore the justification that was there before the table.
		m_ps->m_paragraphJustification = m_ps->m_paragraphJustificationBeforeTable;
	}
}


/****************************************
 public 'parser' functions
*****************************************/

void WP3ContentListener::attributeChange(const bool isOn, const uint8_t attribute)
{
        if (!isUndoOn())
	{
		_closeSpan();

		uint32_t textAttributeBit = 0;

		// FIXME: handle all the possible attribute bits
		switch (attribute)
		{
			case WP3_ATTRIBUTE_BOLD:
				textAttributeBit = WPX_BOLD_BIT;
				break;
			case WP3_ATTRIBUTE_ITALICS:
				textAttributeBit = WPX_ITALICS_BIT;
				break;
			case WP3_ATTRIBUTE_UNDERLINE:
				textAttributeBit = WPX_UNDERLINE_BIT;
				break;
			case WP3_ATTRIBUTE_OUTLINE:
				textAttributeBit = WPX_OUTLINE_BIT;
				break;
			case WP3_ATTRIBUTE_SHADOW:
				textAttributeBit = WPX_SHADOW_BIT;
				break;
			case WP3_ATTRIBUTE_REDLINE:
				textAttributeBit = WPX_REDLINE_BIT;
				break;
			case WP3_ATTRIBUTE_STRIKE_OUT:
				textAttributeBit = WPX_STRIKEOUT_BIT;
				break;
			case WP3_ATTRIBUTE_SUBSCRIPT:
				textAttributeBit = WPX_SUBSCRIPT_BIT;
				break;
			case WP3_ATTRIBUTE_SUPERSCRIPT:
				textAttributeBit = WPX_SUPERSCRIPT_BIT;
				break;
			case WP3_ATTRIBUTE_DOUBLE_UNDERLINE:
				textAttributeBit = WPX_DOUBLE_UNDERLINE_BIT;
				break;
			case WP3_ATTRIBUTE_EXTRA_LARGE:
				textAttributeBit = WPX_EXTRA_LARGE_BIT;
				break;
			case WP3_ATTRIBUTE_VERY_LARGE:
				textAttributeBit = WPX_VERY_LARGE_BIT;
				break;
			case WP3_ATTRIBUTE_LARGE:
				textAttributeBit = WPX_LARGE_BIT;
				break;
			case WP3_ATTRIBUTE_SMALL_PRINT:
				textAttributeBit = WPX_SMALL_PRINT_BIT;
				break;
			case WP3_ATTRIBUTE_FINE_PRINT:
				textAttributeBit = WPX_FINE_PRINT_BIT;
				break;		
			case WP3_ATTRIBUTE_SMALL_CAPS:
				textAttributeBit = WPX_SMALL_CAPS_BIT;
				break;
		}

		if (isOn)
			m_ps->m_textAttributeBits |= textAttributeBit;
		else
			m_ps->m_textAttributeBits ^= textAttributeBit;
	}
}

void WP3ContentListener::undoChange(const uint8_t undoType, const uint16_t /* undoLevel */)
{
        if (undoType == 0x00) // begin invalid text
                setUndoOn(true);
        else if (undoType == 0x01) // end invalid text
                setUndoOn(false);
}

void WP3ContentListener::marginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn())
	{
		double marginInch = (double)((double)margin/ (double)WPX_NUM_WPUS_PER_INCH);

		switch(side)
		{
		case WPX_LEFT:
			if (m_ps->m_numColumns > 1)
			{
				m_ps->m_leftMarginByPageMarginChange = 0.0;
				m_ps->m_sectionMarginLeft = marginInch - m_ps->m_pageMarginLeft;
			}
			else
			{
				m_ps->m_leftMarginByPageMarginChange = marginInch - m_ps->m_pageMarginLeft;
				m_ps->m_sectionMarginLeft = 0.0;
			}
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
						+ m_ps->m_leftMarginByParagraphMarginChange
						+ m_ps->m_leftMarginByTabs;
			break;
		case WPX_RIGHT:
			if (m_ps->m_numColumns > 1)
			{
				m_ps->m_rightMarginByPageMarginChange = 0.0;
				m_ps->m_sectionMarginRight = marginInch - m_ps->m_pageMarginRight;
			}
			else
			{
				m_ps->m_rightMarginByPageMarginChange = marginInch - m_ps->m_pageMarginRight;
				m_ps->m_sectionMarginRight = 0.0;
			}
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
						+ m_ps->m_rightMarginByParagraphMarginChange
						+ m_ps->m_rightMarginByTabs;
			break;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP3ContentListener::justificationChange(const uint8_t justification)
{
	if (!isUndoOn())
	{
		// could be done simply by:
		// m_ps->m_paragraphJustification = justification;
		switch (justification)
		{
		case 0x00:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_LEFT;
			break;
		case 0x01:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
			break;
		case 0x02:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
			break;
		case 0x03:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_FULL;
			break;
		case 0x04:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES;
			break;
		case 0x05:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_DECIMAL_ALIGNED;
			break;
		}
	}
}

void WP3ContentListener::indentFirstLineChange(const int16_t offset)
{
	if (!isUndoOn())
	{
		double offsetInch = (double)((double)offset / (double)WPX_NUM_WPUS_PER_INCH);
		m_ps->m_textIndentByParagraphIndentChange = offsetInch;
		// This is necessary in case we have Indent First Line and Hard Back Tab
		// in the same time. The Hard Back Tab applies to the current paragraph
		// only. Indent First Line applies untill an new Indent First Line code.
		m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
					+ m_ps->m_textIndentByTabs;
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP3ContentListener::setTabs(const bool isRelative, const std::vector<WPXTabStop> tabStops)
{
	if (!isUndoOn())
	{
		m_ps->m_isTabPositionRelative = isRelative;
		m_ps->m_tabStops = tabStops;
	}
}

void WP3ContentListener::columnChange(const WPXTextColumnType /* columnType */, const uint8_t numColumns,
					const std::vector<double> &columnWidth, const std::vector<bool> &isFixedWidth)
{
	if (!isUndoOn())
	{
		_openPageSpan();

		int oldColumnNum = m_ps->m_numColumns;

		// In WP, the last column ends with a hard column break code.
		// In this case, we do not really want to insert any column break
		m_ps->m_isParagraphColumnBreak = false;
		m_ps->m_isTextColumnWithoutParagraph = false;

		double remainingSpace = m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_sectionMarginLeft
			- m_ps->m_pageMarginRight - m_ps->m_sectionMarginRight
			- m_ps->m_leftMarginByPageMarginChange - m_ps->m_rightMarginByPageMarginChange;
		// determine the space that is to be divided between columns whose width is expressed in percentage of remaining space
		std::vector<WPXColumnDefinition> tmpColumnDefinition;
		tmpColumnDefinition.clear();
		if (numColumns > 1)
		{
			int i;
			for (i=0; i<(int)columnWidth.size(); i++)
			{
				if (isFixedWidth[i])
					remainingSpace -= columnWidth[i];
			}
			WPXColumnDefinition tmpColumn;
			for (i=0; i<numColumns; i++)
			{
				if (i == 0)
					tmpColumn.m_leftGutter = 0.0;
				else if (isFixedWidth[2*i-1])
					tmpColumn.m_leftGutter = 0.5f * columnWidth[2*i-1];
				else
					tmpColumn.m_leftGutter = 0.5f * remainingSpace * columnWidth[2*i-1];
				
				if (i >= (numColumns - 1))
					tmpColumn.m_rightGutter = 0.0;
				else if (isFixedWidth[2*i+1])
					tmpColumn.m_rightGutter = 0.5f * columnWidth[2*i+1];
				else
					tmpColumn.m_rightGutter = 0.5f * remainingSpace * columnWidth[2*i+1];

				if (isFixedWidth[2*i])
					tmpColumn.m_width = columnWidth[2*i];
				else
					tmpColumn.m_width = remainingSpace * columnWidth[2*i];
				
				tmpColumn.m_width += tmpColumn.m_leftGutter + tmpColumn.m_rightGutter;
				
				tmpColumnDefinition.push_back(tmpColumn);
			}
		}

		if (!m_ps->m_inSubDocument && !m_ps->m_isTableOpened)
			_closeSection();
		else
			m_ps->m_sectionAttributesChanged = true;
		m_ps->m_numColumns = numColumns;
		m_ps->m_textColumns = tmpColumnDefinition;
		m_ps->m_isTextColumnWithoutParagraph = true;
		if ((m_ps->m_numColumns > 1 && oldColumnNum <= 1) || (m_ps->m_numColumns <= 1 && oldColumnNum > 1))
		{
			m_ps->m_paragraphMarginLeft -= m_ps->m_leftMarginByPageMarginChange;
			m_ps->m_listReferencePosition -= m_ps->m_leftMarginByPageMarginChange;
			m_ps->m_paragraphMarginRight -= m_ps->m_rightMarginByPageMarginChange;
			std::swap(m_ps->m_leftMarginByPageMarginChange, m_ps->m_sectionMarginLeft);
			std::swap(m_ps->m_rightMarginByPageMarginChange, m_ps->m_sectionMarginRight);
			m_ps->m_paragraphMarginLeft += m_ps->m_leftMarginByPageMarginChange;
			m_ps->m_listReferencePosition += m_ps->m_leftMarginByPageMarginChange;
			m_ps->m_paragraphMarginRight += m_ps->m_rightMarginByPageMarginChange;
		}
	}
}


void WP3ContentListener::setTextColor(const RGBSColor *fontColor)
{
	if (!isUndoOn())
	{
		_closeSpan();
		
		*(m_ps->m_fontColor) = *fontColor;
	}
}

void WP3ContentListener::setTextFont(const WPXString &fontName)
{
	if (!isUndoOn())
	{
		_closeSpan();
		
		*(m_ps->m_fontName) = fontName;
	}
}

void WP3ContentListener::setFontSize(const uint16_t fontSize)
{
	if (!isUndoOn())
	{
		_closeSpan();
		
		m_ps->m_fontSize=double(fontSize);
	}
}

void WP3ContentListener::insertPageNumber(const WPXString &pageNumber)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		m_parseState->m_textBuffer.append(pageNumber);
	}
}

void WP3ContentListener::insertNoteReference(const WPXString &noteReference)
{
	if (!isUndoOn())
	{
		m_parseState->m_noteReference = noteReference;
	}
}

void WP3ContentListener::insertNote(const WPXNoteType noteType, const WP3SubDocument *subDocument)
{
	if (!isUndoOn() && !m_ps->m_isNote)
	{
		if (!m_ps->m_isParagraphOpened)
			_openParagraph();
		else
		{
			_flushText();
			_closeSpan();
		}
		m_ps->m_isNote = true;
		WPXNumberingType numberingType = _extractWPXNumberingTypeFromBuf(m_parseState->m_noteReference, ARABIC);
		int number = _extractDisplayReferenceNumberFromBuf(m_parseState->m_noteReference, numberingType);
		m_parseState->m_noteReference.clear();
		
		WPXPropertyList propList;
		propList.insert("libwpd:number", number);

		if (noteType == FOOTNOTE)
			m_documentInterface->openFootnote(propList);
		else
			m_documentInterface->openEndnote(propList);

		handleSubDocument(subDocument, WPX_SUBDOCUMENT_NOTE, m_parseState->m_tableList, 0);

		if (noteType == FOOTNOTE)
			m_documentInterface->closeFootnote();
		else
			m_documentInterface->closeEndnote();
		m_ps->m_isNote = false;
	}
}

void WP3ContentListener::backTab()
{
	if (!isUndoOn() && !m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
	{
		if (m_ps->m_tabStops.empty() || (_getPreviousTabStop() == (std::numeric_limits<double>::max)()))
			m_ps->m_textIndentByTabs -= 0.5f;
		else
			m_ps->m_textIndentByTabs = _getPreviousTabStop() - (m_ps->m_leftMarginByTabs  + m_ps->m_textIndentByParagraphIndentChange);

		m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
			+ m_ps->m_textIndentByTabs;
		m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
			+ m_ps->m_leftMarginByParagraphMarginChange + m_ps->m_leftMarginByTabs;
		m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
			+ m_ps->m_rightMarginByParagraphMarginChange + m_ps->m_rightMarginByTabs;
			
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP3ContentListener::leftIndent()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			if (m_ps->m_tabStops.empty() || (_getNextTabStop() == (std::numeric_limits<double>::min)()))
				m_ps->m_leftMarginByTabs += 0.5f;
			else
				m_ps->m_leftMarginByTabs = _getNextTabStop() - (m_ps->m_textIndentByTabs + m_ps->m_textIndentByParagraphIndentChange);

			if (m_ps->m_paragraphTextIndent != 0.0)
				m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;

			m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
				+ m_ps->m_textIndentByTabs;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
				+ m_ps->m_leftMarginByParagraphMarginChange + m_ps->m_leftMarginByTabs;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
				+ m_ps->m_rightMarginByParagraphMarginChange + m_ps->m_rightMarginByTabs;
			
			m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
		}
		else
			insertTab();
	}
}

void WP3ContentListener::leftIndent(const double offset)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			if (offset == 0.0)
				m_ps->m_leftMarginByTabs += 0.5f;
			else
				m_ps->m_leftMarginByTabs += (offset / 72.0);

			if (m_ps->m_paragraphTextIndent != 0.0)
				m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;

			m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
				+ m_ps->m_textIndentByTabs;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
				+ m_ps->m_leftMarginByParagraphMarginChange + m_ps->m_leftMarginByTabs;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
				+ m_ps->m_rightMarginByParagraphMarginChange + m_ps->m_rightMarginByTabs;
			
			m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
		}
		else
			insertTab();
	}
}

void WP3ContentListener::leftRightIndent()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			if (m_ps->m_tabStops.empty() || (_getNextTabStop() == (std::numeric_limits<double>::min)()))
				m_ps->m_leftMarginByTabs += 0.5f;
			else
				m_ps->m_leftMarginByTabs = _getNextTabStop() - (m_ps->m_textIndentByTabs + m_ps->m_textIndentByParagraphIndentChange);
			
			if (m_ps->m_paragraphTextIndent != 0.0)
				m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;

			m_ps->m_rightMarginByTabs = m_ps->m_leftMarginByTabs;

			m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
				+ m_ps->m_textIndentByTabs;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
				+ m_ps->m_leftMarginByParagraphMarginChange + m_ps->m_leftMarginByTabs;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
				+ m_ps->m_rightMarginByParagraphMarginChange + m_ps->m_rightMarginByTabs;
			
			m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
		}
		else
			insertTab();
	}
}

void WP3ContentListener::leftRightIndent(const double offset)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			if (offset == 0.0)
				m_ps->m_leftMarginByTabs += 0.5f;
			else
				m_ps->m_leftMarginByTabs += (offset / 72.0);

			if (m_ps->m_paragraphTextIndent != 0.0)
				m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;

			m_ps->m_rightMarginByTabs = m_ps->m_leftMarginByTabs;

			m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
				+ m_ps->m_textIndentByTabs;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
				+ m_ps->m_leftMarginByParagraphMarginChange + m_ps->m_leftMarginByTabs;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
				+ m_ps->m_rightMarginByParagraphMarginChange + m_ps->m_rightMarginByTabs;
			
			m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
		}
		else
			insertTab();
	}
}

void WP3ContentListener::insertPicture(double height, double width, double verticalOffset, double horizontalOffset, uint8_t leftColumn, uint8_t rightColumn,
			uint16_t figureFlags, const WPXBinaryData &binaryData)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();

		WPXPropertyList propList;
		_handleFrameParameters( propList, height, width, verticalOffset, horizontalOffset, leftColumn, rightColumn, figureFlags );
		m_documentInterface->openFrame(propList);
		
		propList.clear();
		propList.insert("libwpd:mimetype", "image/pict");
		m_documentInterface->insertBinaryObject(propList, binaryData);
		
		m_documentInterface->closeFrame();
	}
}

void WP3ContentListener::insertTextBox(double height, double width, double verticalOffset, double horizontalOffset, uint8_t leftColumn, uint8_t rightColumn,
			uint16_t figureFlags, const WP3SubDocument * subDocument, const WP3SubDocument *caption)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();

		WPXPropertyList propList;
		_handleFrameParameters( propList, height, width, verticalOffset, horizontalOffset, leftColumn, rightColumn, figureFlags );
		m_documentInterface->openFrame(propList);
		
		propList.clear();

		if (subDocument || caption)
		{
			m_documentInterface->openTextBox(propList);
		
			// Positioned objects like text boxes are special beasts. They can contain all hierarchical elements up
			// to the level of sections. They cannot open or close a page span though.
			if (subDocument)
				handleSubDocument(subDocument, WPX_SUBDOCUMENT_TEXT_BOX, m_parseState->m_tableList, 0);
			if (caption)
				handleSubDocument(caption, WPX_SUBDOCUMENT_TEXT_BOX, m_parseState->m_tableList, 0);

			m_documentInterface->closeTextBox();
		}
		
		m_documentInterface->closeFrame();
	}
}

void WP3ContentListener::insertWP51Table(double height, double width, double verticalOffset, double horizontalOffset, uint8_t leftColumn, uint8_t rightColumn,
			uint16_t figureFlags, const WP3SubDocument * subDocument, const WP3SubDocument *caption)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();

		WPXPropertyList propList;
		_handleFrameParameters( propList, height, width, verticalOffset, horizontalOffset, leftColumn, rightColumn, figureFlags );
		m_documentInterface->openFrame(propList);

		propList.clear();

		if (subDocument || caption)
		{
			m_documentInterface->openTextBox(propList);
		
			// Positioned objects like text boxes are special beasts. They can contain all hierarchical elements up
			// to the level of sections. They cannot open or close a page span though.
			if (subDocument)
				WPDocument::parseSubDocument(subDocument->getStream(), m_documentInterface, WPD_FILE_FORMAT_WP5);

			if (caption)
				handleSubDocument(caption, WPX_SUBDOCUMENT_TEXT_BOX, m_parseState->m_tableList, 0);

			m_documentInterface->closeTextBox();
		}

		m_documentInterface->closeFrame();
	}
}

void WP3ContentListener::_handleFrameParameters( WPXPropertyList &propList, double height, double width, double verticalOffset, double horizontalOffset,
		uint8_t /* leftColumn */, uint8_t /* rightColumn */, uint16_t figureFlags  )
{
	propList.insert("svg:width", (double)((double)width/72.0));
	propList.insert("svg:height", (double)((double)height/72.0));
	
	if ( figureFlags & 0x0080 )
		propList.insert( "style:wrap", "dynamic" );
	else
		propList.insert( "style:wrap", "none" );

	if ( ( figureFlags & 0x0300 ) == 0x0000 ) // paragraph
	{
		propList.insert("text:anchor-type", "paragraph");
		propList.insert("style:vertical-rel", "paragraph" );
		propList.insert("style:horizontal-rel", "paragraph");
		switch ( figureFlags & 0x0003 )
		{
		case 0x01:
			if (horizontalOffset == 0.0)
				propList.insert("style:horizontal-pos", "right");
			else
			{
				propList.insert( "style:horizontal-pos", "from-left");
				propList.insert( "svg:x", (double)((double)horizontalOffset/72.0 - (double)width/72.0 +
					(double)(m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight - m_ps->m_sectionMarginLeft
					- m_ps->m_sectionMarginRight - m_ps->m_paragraphMarginLeft - m_ps->m_paragraphMarginRight)));
			}
			break;
		case 0x02:
			if (horizontalOffset == 0.0)
				propList.insert("style:horizontal-pos", "center");
			else
			{
				propList.insert( "style:horizontal-pos", "from-left");
				propList.insert( "svg:x", (double)((double)horizontalOffset/72.0 - (double)width/(2.0*72.0) +
					(double)(m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight - m_ps->m_sectionMarginLeft
					- m_ps->m_sectionMarginRight - m_ps->m_paragraphMarginLeft - m_ps->m_paragraphMarginRight)/2.0));
			}
			break;
		case 0x03:
			propList.insert("svg:width", (double)(m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight - m_ps->m_sectionMarginLeft
				- m_ps->m_sectionMarginRight - m_ps->m_paragraphMarginLeft - m_ps->m_paragraphMarginRight) );
			propList.insert("style:horizontal-pos", "center");
			break;
		case 0x00:
		default:
			if (horizontalOffset == 0.0)
				propList.insert("style:horizontal-pos", "left");
			else
			{
				propList.insert( "style:horizontal-pos", "from-left");
				propList.insert( "svg:x", (double)((double)horizontalOffset/72.0));
			}
			break;
		}

		if (verticalOffset == 0.0)
			propList.insert( "style:vertical-pos", "top" );
		else
		{
			propList.insert( "style:vertical-pos", "from-top" );
			propList.insert( "svg:y", (double)((double)verticalOffset/72.0));
		}

	}
	else if ( ( figureFlags & 0x0300 ) == 0x0100 ) // page
	{
		propList.insert("text:anchor-type", "char");
		
		if ( ( figureFlags & 0x1f08 ) == 0x0100 ) // full page
		{
			propList.insert("svg:width", (double)(m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight ) );
			propList.insert("svg:height", (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom ) );
			propList.insert("style:vertical-rel", "page-content" );
			propList.insert("style:vertical-pos", "middle" );
			propList.insert("style:horizontal-rel", "page-content" );
			propList.insert("style:horizontal-pos", "center" );
		}
		else if ( ( figureFlags & 0x1f08 ) == 0x1108 ) // abs. page
		{
			propList.insert("style:vertical-rel", "page" );
			propList.insert("style:vertical-pos", "from-top" );
			propList.insert("svg:y", (double)((double)verticalOffset/72.0));
			propList.insert("style:horizontal-rel", "page-start-margin" );
			propList.insert("style:horizontal-pos", "from-left" );
			propList.insert("svg:x", (double)((double)horizontalOffset/72.0));
		}
		else
		{
			propList.insert("style:vertical-rel", "page-content" );
			propList.insert("style:horizontal-rel", "page-content" );
			switch ( ( figureFlags & 0x1c00 ) >> 10 )
			{
			case 0x00:
				propList.insert("svg:height", (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom ) );
				propList.insert("style:vertical-rel", "page-content" );
				propList.insert("style:vertical-pos", "middle" );
				break;
			case 0x01:
				if ( verticalOffset == 0.0)
					propList.insert("style:vertical-pos", "top" );
				else
				{
					propList.insert("style:vertical-pos", "from-top" );
					double newPosition = (double)((double)verticalOffset/72.0);
					if (newPosition > (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0) )
						newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0);
					propList.insert("svg:y", newPosition);
				}
				break;
			case 0x02:
				if (verticalOffset == 0.0)
					propList.insert("style:vertical-pos", "middle" );
				else
				{
					propList.insert("style:vertical-pos", "from-top" );
					double newPosition = (double)((m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0)/2.0);
					if (newPosition > (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0) )
						newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0);
					propList.insert("svg:y", newPosition);
				}
				break;
			case 0x03:
				if (verticalOffset == 0.0)
					propList.insert("style:vertical-pos", "bottom" );
				else
				{
					propList.insert("style:vertical-pos", "from-top" );
					double newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0 + (double)verticalOffset/72.0);
					if (newPosition > (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0) )
						newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
						- (double)height/72.0);
					propList.insert("svg:y", newPosition);
				}
				break;
			case 0x04:
				propList.insert("style:vertical-rel", "page" );
				propList.insert("style:vertical-pos", "from-top" );
				propList.insert("svg:y", (double)((double)verticalOffset/72.0));
				break;		
			default:
				break;
			}
			
			switch ( figureFlags & 0x0003 )
			{
			case 0x00:
				if ( horizontalOffset == 0.0 )
					propList.insert( "style:horizontal-pos", "left");
				else
				{
					propList.insert( "style:horizontal-pos", "from-left");
					propList.insert( "svg:x", (double)((double)horizontalOffset/72.0));
				}
				break;
			case 0x01:
				if ( horizontalOffset == 0.0 )
					propList.insert( "style:horizontal-pos", "right");
				else
				{
					propList.insert( "style:horizontal-pos", "from-left");
					propList.insert( "svg:x", (double)(m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight
						- (double)width/72.0 + (double)horizontalOffset/72.0));
				}
				break;
			case 0x02:
				if ( horizontalOffset == 0.0 )
					propList.insert( "style:horizontal-pos", "center" );
				else
				{
					propList.insert( "style:horizontal-pos", "from-left");
					propList.insert( "svg:x", (double)((m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight
						- (double)width/72.0)/2.0 + (double)horizontalOffset/72.0));
				}
				break;
			case 0x03:
				propList.insert("svg:width", (double)(m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight ) );
				propList.insert("style:horizontal-rel", "page-content" );
				propList.insert("style:horizontal-pos", "center" );
				break;
			default:
				break;
			}
		}
	}
	else if ( ( figureFlags & 0x0300 ) == 0x0200 ) // character
	{
		propList.insert("text:anchor-type", "as-char");
		if ( ( figureFlags & 0x1c00 ) == 0x0000 )
			propList.insert( "style:vertical-rel", "baseline" );
		else
			propList.insert( "style:vertical-rel", "line" );
		switch ( ( figureFlags & 0x1c00 ) >> 10 )
		{
			case 0x01:
				if ( verticalOffset == 0.0 )
					propList.insert( "style:vertical-pos", "top" );
				else
				{
					propList.insert( "style:vertical-pos", "from-top" );
					propList.insert( "svg:y", (double)((double)verticalOffset/72.0));
				}
				break;
			case 0x02:
				if ( verticalOffset == 0.0 )
					propList.insert( "style:vertical-pos", "middle" );
				else
				{
					propList.insert( "style:vertical-pos", "from-top" );
					propList.insert( "svg:y", (double)((double)verticalOffset/72.0 - (double)height/(2.0*72.0)));
				}
				break;
			case 0x00:
			case 0x03:
				if ( verticalOffset == 0.0 )
					propList.insert( "style:vertical-pos", "bottom" );
				else
				{
					propList.insert( "style:vertical-pos", "from-top" );
					propList.insert( "svg:y", (double)((double)verticalOffset/72.0 - (double)height/72.0));
				}
				break;
			case 0x04:
				break;
			default:
				break;
		}
	}
}

void WP3ContentListener::_handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, 
						WPXTableList /* tableList */, int /* nextTableIndice */)
{
	// save our old parsing state on our "stack"
	WP3ContentParsingState *oldParseState = m_parseState;

	m_parseState = new WP3ContentParsingState();
	if (subDocumentType == WPX_SUBDOCUMENT_TEXT_BOX || subDocumentType == WPX_SUBDOCUMENT_COMMENT_ANNOTATION)
	{
		m_ps->m_pageMarginRight = 0.0;
		m_ps->m_pageMarginLeft = 0.0;
	}

	bool oldIsUndoOn = isUndoOn();
	setUndoOn(false);

	if (subDocumentType == WPX_SUBDOCUMENT_HEADER_FOOTER)
	{
		marginChange(WPX_LEFT, WPX_NUM_WPUS_PER_INCH);
		marginChange(WPX_RIGHT, WPX_NUM_WPUS_PER_INCH);
	}

	if (subDocument)
		static_cast<const WP3SubDocument *>(subDocument)->parse(this);
	else
		_openSpan();

	// Close the sub-document properly
	if (m_ps->m_isParagraphOpened)
		_closeParagraph();
	if (m_ps->m_isListElementOpened)
		_closeListElement();

	m_ps->m_currentListLevel = 0;
	_changeList();

#if 0
	_closeSection();
#endif

	// restore our old parsing state
	delete m_parseState;
	m_parseState = oldParseState;
	setUndoOn(oldIsUndoOn);
}
	
void WP3ContentListener::headerFooterGroup(const uint8_t /* headerFooterType */, const uint8_t /* occurenceBits */, WP3SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);			
}	

void WP3ContentListener::_openParagraph()
{

	if (m_ps->m_isTableOpened)
	{
		if (!m_ps->m_isTableRowOpened)
			insertRow();
					
		if (!m_ps->m_isTableCellOpened)
		{
			insertCell();
		}
	}

	WPXContentListener::_openParagraph();
}

/****************************************
 private functions
*****************************************/

void WP3ContentListener::_flushText()
{
	_insertText(m_parseState->m_textBuffer);
	m_parseState->m_textBuffer.clear();
}
