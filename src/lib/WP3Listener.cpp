/* libwpd
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004-2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP3Listener.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP3SubDocument.h"

_WP3ParsingState::_WP3ParsingState():
	m_colSpan(1),
	m_rowSpan(1),
	m_cellFillColor(NULL)
{
	m_textBuffer.clear();
	m_noteReference.clear();
}

_WP3ParsingState::~_WP3ParsingState()
{
	m_textBuffer.clear();
	m_noteReference.clear();
	DELETEP(m_cellFillColor);
}

WP3Listener::WP3Listener(std::vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl) :
	WPXListener(pageList, listenerImpl),
	m_parseState(new WP3ParsingState)
{
}

WP3Listener::~WP3Listener() 
{
	delete m_parseState;
}


/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP3Listener::insertCharacter(const uint16_t character)
{
        if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		appendUCS4(m_parseState->m_textBuffer, (uint32_t)character);
	}
}

void WP3Listener::insertTab(const uint8_t tabType, const float tabPosition)
{
        if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		else
			_flushText();
		m_listenerImpl->insertTab();
	}
}

void WP3Listener::insertEOL()
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

void WP3Listener::endDocument()
{
	_closeSpan();
	_closeParagraph();
	_closeSection();
	_closePageSpan();
	m_listenerImpl->endDocument();
}

void WP3Listener::defineTable(const uint8_t position, const uint16_t leftOffset)
{
	if (!isUndoOn())
	{
		switch (position & 0x07)
		{
		case 0:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN;
			break;
		case 1:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN;
			break;
		case 2:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_CENTER_BETWEEN_MARGINS;
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
		m_ps->m_tableDefinition.m_leftOffset = (float)((double)leftOffset / (double)WPX_NUM_WPUS_PER_INCH) - m_ps->m_paragraphMarginLeft;

		// remove all the old column information
		m_ps->m_tableDefinition.columns.clear();
		m_ps->m_tableDefinition.columnsProperties.clear();
		m_ps->m_numRowsToSkip.clear();
	}
}

void WP3Listener::addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter, const uint32_t attributes, const uint8_t alignment)
{
	if (!isUndoOn())
	{
		// define the new column
		WPXColumnDefinition colDef;
		colDef.m_width = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_leftGutter = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_rightGutter = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);

		// add the new column definition to our table definition
		m_ps->m_tableDefinition.columns.push_back(colDef);
		
		WPXColumnProperties colProp;
		colProp.m_attributes = attributes;
		colProp.m_alignment = alignment;
		
		m_ps->m_tableDefinition.columnsProperties.push_back(colProp);
		
		// initialize the variable that tells us how many columns to skip
		m_ps->m_numRowsToSkip.push_back(0);
	}
}

void WP3Listener::startTable()
{
	if (!isUndoOn())
	{
		// save the justification information. We will need it after the table ends.
		m_ps->m_paragraphJustificationBeforeTable = m_ps->m_paragraphJustification;
		if (m_ps->m_sectionAttributesChanged && !m_ps->m_isTableOpened)
			_closeSection();
		m_ps->m_sectionAttributesChanged = false;
		if (!m_ps->m_isTableOpened && !m_ps->m_isSectionOpened)
			_openSection();
		_openTable();
	}
}

void WP3Listener::insertRow()
{
	if (!isUndoOn())
		_openTableRow(0, true, false);
}

void WP3Listener::insertCell()
{
	if (!isUndoOn())
	{
		if (m_ps->m_currentTableRow < 0) // cell without a row, invalid
			throw ParseException();
		
		RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
		_openTableCell((uint8_t)m_parseState->m_colSpan, (uint8_t)m_parseState->m_rowSpan, 0x00000000,       
				 m_parseState->m_cellFillColor, NULL, &tmpCellBorderColor, TOP);
		DELETEP(m_parseState->m_cellFillColor);

		m_ps->m_isCellWithoutParagraph = true;
		m_ps->m_cellAttributeBits = 0x00000000;
	}
}

void WP3Listener::closeCell()
{
	if (!isUndoOn())
	{
		insertEOL();
		_closeTableCell();
		m_parseState->m_rowSpan = 1;
		m_parseState->m_colSpan = 1;
	}
}

void WP3Listener::closeRow()
{
	if (!isUndoOn())
	{
		closeCell();

		_closeTableRow();
	}
}

void WP3Listener::setTableCellSpan(const uint16_t colSpan, const uint16_t rowSpan)
{
	if (!isUndoOn())
	{
		m_parseState->m_colSpan=colSpan;
		m_parseState->m_rowSpan=rowSpan;
	}
}

void WP3Listener::setTableCellFillColor(const RGBSColor * cellFillColor)
{
	if (!isUndoOn())
	{
		if (m_parseState->m_cellFillColor)
			DELETEP(m_parseState->m_cellFillColor);
		m_parseState->m_cellFillColor = new RGBSColor(*cellFillColor);
	}
}

void WP3Listener::endTable()
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

void WP3Listener::attributeChange(const bool isOn, const uint8_t attribute)
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

void WP3Listener::undoChange(const uint8_t undoType, const uint16_t undoLevel)
{
        if (undoType == 0x00) // begin invalid text
                m_isUndoOn = true;
        else if (undoType == 0x01) // end invalid text
                m_isUndoOn = false;
}

void WP3Listener::marginChange(const uint8_t side, const uint16_t margin)
{
	if (!isUndoOn())
	{
		float marginInch = (float)((double)margin/ (double)WPX_NUM_WPUS_PER_INCH);

		switch(side)
		{
		case WPX_LEFT:
			m_ps->m_leftMarginByPageMarginChange = marginInch - m_ps->m_pageMarginLeft;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
						+ m_ps->m_leftMarginByParagraphMarginChange
						+ m_ps->m_leftMarginByTabs;
			break;
		case WPX_RIGHT:
			m_ps->m_rightMarginByPageMarginChange = marginInch - m_ps->m_pageMarginRight;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
						+ m_ps->m_rightMarginByParagraphMarginChange
						+ m_ps->m_rightMarginByTabs;
			break;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP3Listener::justificationChange(const uint8_t justification)
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

void WP3Listener::indentFirstLineChange(const int16_t offset)
{
	if (!isUndoOn())
	{
		float offsetInch = (float)((double)offset / (double)WPX_NUM_WPUS_PER_INCH);
		m_ps->m_textIndentByParagraphIndentChange = offsetInch;
		// This is necessary in case we have Indent First Line and Hard Back Tab
		// in the same time. The Hard Back Tab applies to the current paragraph
		// only. Indent First Line applies untill an new Indent First Line code.
		m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
					+ m_ps->m_textIndentByTabs;
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP3Listener::columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<float> &columnWidth,
		const std::vector<bool> &isFixedWidth)
{
	if (!isUndoOn())
	{
		// In WP, the last column ends with a hard column break code.
		// In this case, we do not really want to insert any column break
		m_ps->m_isParagraphColumnBreak = false;
		m_ps->m_isTextColumnWithoutParagraph = false;

		float remainingSpace = m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight
						- m_ps->m_leftMarginByPageMarginChange - m_ps->m_rightMarginByPageMarginChange;
		// determine the space that is to be divided between columns whose width is expressed in percentage of remaining space
		std::vector<WPXColumnDefinition> tmpColumnDefinition;
		tmpColumnDefinition.clear();
		if (numColumns > 1)
		{
			int i;
			for (i=0; i<columnWidth.size(); i++)
			{
				if (isFixedWidth[i])
					remainingSpace -= columnWidth[i];
			}
			WPXColumnDefinition tmpColumn;
			for (i=0; i<numColumns; i++)
			{
				if (i == 0)
					tmpColumn.m_leftGutter = 0.0f;
				else if (isFixedWidth[2*i-1])
					tmpColumn.m_leftGutter = 0.5f * columnWidth[2*i-1];
				else
					tmpColumn.m_leftGutter = 0.5f * remainingSpace * columnWidth[2*i-1];
				
				if (i >= (numColumns - 1))
					tmpColumn.m_rightGutter = 0.0f;
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
	}
}


void WP3Listener::setTextColor(const RGBSColor *fontColor)
{
	if (!isUndoOn())
	{
		_closeSpan();
		
		*(m_ps->m_fontColor) = *fontColor;
	}
}

void WP3Listener::setTextFont(const std::string fontName)
{
	if (!isUndoOn())
	{
		_closeSpan();
		
		m_ps->m_fontName->sprintf("%s", fontName.c_str());
	}
}

void WP3Listener::setFontSize(const uint16_t fontSize)
{
	if (!isUndoOn())
	{
		_closeSpan();
		
		m_ps->m_fontSize=float(fontSize);
	}
}

void WP3Listener::insertNoteReference(const std::string noteReference)
{
	if (!isUndoOn())
	{
		m_parseState->m_noteReference.sprintf("%s", noteReference.c_str());
	}
}

void WP3Listener::insertNote(const WPXNoteType noteType, const WP3SubDocument *subDocument)
{
	if (!isUndoOn())
	{
		_closeSpan();
		m_ps->m_isNote = true;
		WPXNumberingType numberingType = _extractWPXNumberingTypeFromBuf(m_parseState->m_noteReference, ARABIC);
		int number = _extractDisplayReferenceNumberFromBuf(m_parseState->m_noteReference, numberingType);
		m_parseState->m_noteReference.clear();
		
		WPXPropertyList propList;
		propList.insert("libwpd:number", number);

		if (noteType == FOOTNOTE)
			m_listenerImpl->openFootnote(propList);
		else
			m_listenerImpl->openEndnote(propList);

		handleSubDocument(subDocument, false, m_parseState->m_tableList, 0);

		if (noteType == FOOTNOTE)
			m_listenerImpl->closeFootnote();
		else
			m_listenerImpl->closeEndnote();
		m_ps->m_isNote = false;
	}
}

void WP3Listener::_handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice)
{
		// save our old parsing state on our "stack"
		WP3ParsingState *oldParseState = m_parseState;
	
		m_parseState = new WP3ParsingState();

		if (subDocument)
			subDocument->parse(this);
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
}
	
void WP3Listener::_openParagraph()
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

	WPXListener::_openParagraph();
}

/****************************************
 private functions
*****************************************/

void WP3Listener::_flushText()
{
	if (m_parseState->m_textBuffer.len())
		m_listenerImpl->insertText(m_parseState->m_textBuffer);
	m_parseState->m_textBuffer.clear();
}
