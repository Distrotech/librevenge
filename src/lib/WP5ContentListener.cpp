/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell, Inc. (http://www.novell.com)
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

#include "WP5ContentListener.h"
#include "WP5FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP5SubDocument.h"

_WP5ContentParsingState::_WP5ContentParsingState() :
	m_textBuffer(),
	m_noteReference(),
	m_tableList(),
	m_isFrameOpened(false)
{
}

_WP5ContentParsingState::~_WP5ContentParsingState()
{
}

WP5ContentListener::WP5ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP5SubDocument*> &subDocuments, WPXDocumentInterface *documentInterface) :
	WP5Listener(),
	WPXContentListener(pageList, documentInterface),
	m_parseState(new WP5ContentParsingState),
	m_subDocuments(subDocuments),
	m_defaultFontSize(12.0),
	m_defaultFontName("Times New Roman")
{
}

WP5ContentListener::~WP5ContentListener() 
{
	delete m_parseState;
}


/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP5ContentListener::insertCharacter(uint16_t character)
{
	uint16_t tmpCharacter = _mapNonUnicodeCharacter(character);
	if (!m_ps->m_isSpanOpened)
		_openSpan();
	appendUCS4(m_parseState->m_textBuffer, (uint32_t)tmpCharacter);
}

void WP5ContentListener::insertTab(uint8_t tabType, double tabPosition)
{
	bool tmpHasTabPositionInformation = true;
	if (tabPosition >= (double)((double)0xFFFE/(double)WPX_NUM_WPUS_PER_INCH) || tabPosition == 0.0)
		tmpHasTabPositionInformation = false;

	if (!isUndoOn())
	{
		switch ((tabType & 0xE8) >> 3)
		{
		case WP5_TAB_GROUP_CENTER_TAB:
		case WP5_TAB_GROUP_RIGHT_TAB:
		case WP5_TAB_GROUP_DECIMAL_TAB:
			if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
			{
				if (m_ps->m_currentListLevel == 0)
					_openParagraph();
				else
					_openListElement();
			}
			break;

		default:
			break;
		}

		// Following tabs are converted as formating if the paragraph is not opened
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			switch ((tabType & 0xE8) >> 3)
			{
			case WP5_TAB_GROUP_CENTER_ON_MARGINS:
			case WP5_TAB_GROUP_CENTER_ON_CURRENT_POSITION:
				m_ps->m_tempParagraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
				break;

			case WP5_TAB_GROUP_FLUSH_RIGHT:
				m_ps->m_tempParagraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
				break;

			case WP5_TAB_GROUP_LEFT_TAB: // converted as first line indent
				if (!tmpHasTabPositionInformation)
					m_ps->m_textIndentByTabs += 0.5f;
				else
					m_ps->m_textIndentByTabs = tabPosition - m_ps->m_paragraphMarginLeft - m_ps->m_pageMarginLeft 
						- m_ps->m_sectionMarginLeft - m_ps->m_textIndentByParagraphIndentChange;
				break;

			case WP5_TAB_GROUP_BACK_TAB: // converted as hanging indent
				if (!tmpHasTabPositionInformation)
					m_ps->m_textIndentByTabs -= 0.5f;
				else
					m_ps->m_textIndentByTabs = tabPosition - m_ps->m_paragraphMarginLeft - m_ps->m_pageMarginLeft
						- m_ps->m_sectionMarginLeft - m_ps->m_textIndentByParagraphIndentChange;
				break;

			default:
				break;
			}
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

			switch ((tabType & 0xF8) >> 3)
			{
			case WP5_TAB_GROUP_LEFT_TAB:
			case WP5_TAB_GROUP_CENTER_ON_MARGINS:
			case WP5_TAB_GROUP_CENTER_ON_CURRENT_POSITION:
			case WP5_TAB_GROUP_CENTER_TAB:
			case WP5_TAB_GROUP_FLUSH_RIGHT:
			case WP5_TAB_GROUP_RIGHT_TAB:
			case WP5_TAB_GROUP_DECIMAL_TAB:
				m_documentInterface->insertTab();
				break;
			
			default:
				break;
			}
		}
	}
}

void WP5ContentListener::insertIndent(uint8_t indentType, double indentPosition)
{
	bool tmpHasIndentPositionInformation = true;
	if (indentPosition >= (double)((double)0xFFFE/(double)WPX_NUM_WPUS_PER_INCH) || indentPosition == 0.0)
		tmpHasIndentPositionInformation = false;

	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			switch (indentType & 0x01)
			{
			case WP5_INDENT_GROUP_LEFT_INDENT:  // converted as left paragraph margin offset
				if (!tmpHasIndentPositionInformation)
					m_ps->m_leftMarginByTabs += 0.5f;
				else
					m_ps->m_leftMarginByTabs = indentPosition - m_ps->m_pageMarginLeft - m_ps->m_sectionMarginRight
						- m_ps->m_leftMarginByPageMarginChange - m_ps->m_leftMarginByParagraphMarginChange;
				if (m_ps->m_paragraphTextIndent != 0.0)
					m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;
				break;

			case WP5_INDENT_GROUP_LEFT_RIGHT_INDENT: // converted as left and right paragraph margin offset
				if (!tmpHasIndentPositionInformation)
					m_ps->m_leftMarginByTabs += 0.5f;
				else
					m_ps->m_leftMarginByTabs = indentPosition - m_ps->m_pageMarginLeft - m_ps->m_sectionMarginLeft
						- m_ps->m_leftMarginByPageMarginChange - m_ps->m_leftMarginByParagraphMarginChange;
				m_ps->m_rightMarginByTabs = m_ps->m_leftMarginByTabs;
				if (m_ps->m_paragraphTextIndent != 0.0)
					m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;
				break;	
				
			default:
				break;
			}
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

void WP5ContentListener::insertEOL()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
			_openSpan();
		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();
	}
}

void WP5ContentListener::defineTable(uint8_t position, uint16_t leftOffset)
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
		m_ps->m_tableDefinition.m_leftOffset = (double)((double)leftOffset / (double)WPX_NUM_WPUS_PER_INCH) - m_ps->m_paragraphMarginLeft;

		// remove all the old column information
		m_ps->m_tableDefinition.m_columns.clear();
		m_ps->m_tableDefinition.m_columnsProperties.clear();
		m_ps->m_numRowsToSkip.clear();
	}
}

void WP5ContentListener::addTableColumnDefinition(uint32_t width, uint32_t /* leftGutter */,
						uint32_t /* rightGutter */, uint32_t attributes, uint8_t alignment)
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

		// initialize the variable that tells us how many columns to skip
		m_ps->m_numRowsToSkip.push_back(0);
	}
}

void WP5ContentListener::startTable()
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

void WP5ContentListener::insertRow(uint16_t rowHeight, bool isMinimumHeight, bool isHeaderRow)
{
	if (!isUndoOn())
	{
		_flushText();
		double rowHeightInch = (double)((double) rowHeight / (double)WPX_NUM_WPUS_PER_INCH);
		_openTableRow(rowHeightInch, isMinimumHeight, isHeaderRow);
	}
}

void WP5ContentListener::insertCell(uint8_t colSpan, uint8_t rowSpan, uint8_t borderBits,
			const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
			const RGBSColor * cellBorderColor, WPXVerticalAlignment cellVerticalAlignment, 
			bool useCellAttributes, uint32_t cellAttributes)
{
	if (!isUndoOn())
	{
		if (m_ps->m_currentTableRow < 0) // cell without a row, invalid
			throw ParseException();
		_flushText();
		_openTableCell(colSpan, rowSpan, borderBits, cellFgColor, cellBgColor,
					cellBorderColor, cellVerticalAlignment);
		m_ps->m_isCellWithoutParagraph = true;
		if (m_ps->m_currentTableCol < 1)
			return;
		if (useCellAttributes)
			m_ps->m_cellAttributeBits = cellAttributes;
		else
			m_ps->m_cellAttributeBits = m_ps->m_tableDefinition.m_columnsProperties[m_ps->m_currentTableCol-1].m_attributes;
		justificationChange(m_ps->m_tableDefinition.m_columnsProperties[m_ps->m_currentTableCol-1].m_alignment);
	}
}

void WP5ContentListener::endTable()
{
	if (!isUndoOn())
	{
		_flushText();
		if (!m_ps->m_isTableOpened)
			return;
		_closeTable();
		// restore the justification that was there before the table.
		m_ps->m_paragraphJustification = m_ps->m_paragraphJustificationBeforeTable;
	}
}


/****************************************
 public 'parser' functions
*****************************************/

void WP5ContentListener::attributeChange(bool isOn, uint8_t attribute)
{
	if (!isUndoOn())
	{
		_closeSpan();

		uint32_t textAttributeBit = 0;

		// FIXME: handle all the possible attribute bits
		switch (attribute)
		{
			case WP5_ATTRIBUTE_EXTRA_LARGE:
				textAttributeBit = WPX_EXTRA_LARGE_BIT;
				break;
			case WP5_ATTRIBUTE_VERY_LARGE:
				textAttributeBit = WPX_VERY_LARGE_BIT;
				break;
			case WP5_ATTRIBUTE_LARGE:
				textAttributeBit = WPX_LARGE_BIT;
				break;
			case WP5_ATTRIBUTE_SMALL_PRINT:
				textAttributeBit = WPX_SMALL_PRINT_BIT;
				break;
			case WP5_ATTRIBUTE_FINE_PRINT:
				textAttributeBit = WPX_FINE_PRINT_BIT;
				break;		
			case WP5_ATTRIBUTE_SUPERSCRIPT:
				textAttributeBit = WPX_SUPERSCRIPT_BIT;
				break;
			case WP5_ATTRIBUTE_SUBSCRIPT:
				textAttributeBit = WPX_SUBSCRIPT_BIT;
				break;
			case WP5_ATTRIBUTE_OUTLINE:
				textAttributeBit = WPX_OUTLINE_BIT;
				break;
			case WP5_ATTRIBUTE_ITALICS:
				textAttributeBit = WPX_ITALICS_BIT;
				break;
			case WP5_ATTRIBUTE_SHADOW:
				textAttributeBit = WPX_SHADOW_BIT;
				break;
			case WP5_ATTRIBUTE_REDLINE:
				textAttributeBit = WPX_REDLINE_BIT;
				break;
			case WP5_ATTRIBUTE_DOUBLE_UNDERLINE:
				textAttributeBit = WPX_DOUBLE_UNDERLINE_BIT;
				break;
			case WP5_ATTRIBUTE_BOLD:
				textAttributeBit = WPX_BOLD_BIT;
				break;
			case WP5_ATTRIBUTE_STRIKE_OUT:
				textAttributeBit = WPX_STRIKEOUT_BIT;
				break;
			case WP5_ATTRIBUTE_UNDERLINE:
				textAttributeBit = WPX_UNDERLINE_BIT;
				break;
			case WP5_ATTRIBUTE_SMALL_CAPS:
				textAttributeBit = WPX_SMALL_CAPS_BIT;
				break;
		}

		if (isOn)
			m_ps->m_textAttributeBits |= textAttributeBit;
		else
			m_ps->m_textAttributeBits ^= textAttributeBit;
	}
}

void WP5ContentListener::marginChange(uint8_t side, uint16_t margin)
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

void WP5ContentListener::characterColorChange(uint8_t red, uint8_t green, uint8_t blue)
{
	if (!isUndoOn())
	{
		_closeSpan();
		m_ps->m_fontColor->m_r = red;
		m_ps->m_fontColor->m_g = green;
		m_ps->m_fontColor->m_b = blue;
 	}
}

void WP5ContentListener::setFont(const WPXString &fontName, double fontSize)
{
	if (!isUndoOn())
	{
		_closeSpan();
		*(m_ps->m_fontName) = fontName;
		m_ps->m_fontSize = fontSize;
	}
}

void WP5ContentListener::setTabs(const std::vector<WPXTabStop> &tabStops, uint16_t tabOffset)
{
	if (!isUndoOn())
	{
		m_ps->m_isTabPositionRelative = ((tabOffset & 0xffff) != 0xffff);
		m_ps->m_tabStops = tabStops;
	}
}


void WP5ContentListener::insertNoteReference(const WPXString &noteReference)
{
	if (!isUndoOn() && !m_ps->m_isNote)
	{
		m_parseState->m_noteReference = noteReference;
	}
}

void WP5ContentListener::insertNote(WPXNoteType noteType, const WP5SubDocument *subDocument)
{
	if (!isUndoOn())
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

void WP5ContentListener::_handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType subDocumentType, 
	WPXTableList /* tableList */, int /* nextTableIndice */)
{
	// save our old parsing state on our "stack"
	WP5ContentParsingState *oldParseState = m_parseState;

	m_parseState = new WP5ContentParsingState();
	setFont(m_defaultFontName, m_defaultFontSize);

	if (subDocumentType == WPX_SUBDOCUMENT_HEADER_FOOTER)
	{
		marginChange(WPX_LEFT, WPX_NUM_WPUS_PER_INCH);
		marginChange(WPX_RIGHT, WPX_NUM_WPUS_PER_INCH);
	}

	if (subDocument)
		static_cast<const WP5SubDocument *>(subDocument)->parse(this);
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

void WP5ContentListener::headerFooterGroup(uint8_t /* headerFooterType */, uint8_t /* occurenceBits */,
						WP5SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);
}	

void WP5ContentListener::setDefaultFont(const WPXString &fontName, double fontSize)
{
	m_defaultFontName = fontName;
	m_defaultFontSize = fontSize;
}

void WP5ContentListener::boxOn(uint8_t positionAndType, uint8_t alignment, uint16_t width, uint16_t height, uint16_t x, uint16_t y)
{
/*
Paragraph:
	- Vertical position:
		+ offset from top of paragraph
	- Horizontal position:
		+ Left
		+ Right
		+ Centre
		+ Full

Page:
	- Vertical position:
		+ Full Page
		+ Top
		+ Centre
		+ Bottom
		+ Set Position
			* Offset from the top of page
	- Horizontal position
		+ Margins
			* Left
			* Right
			* Centre
			* Full
		+ Columns (Which column)
			* Left
			* Right
			* Centre
			* Full
		+ Set Position
			* Offset from left of page

Character:
	- Vertical position:
		+ Top
		+ Centre
		+ Bottom
		+ Baseline
	- Horizontal Position:
		+ N/A
*/

	if (isUndoOn() || (m_ps->m_isTableOpened && !m_ps->m_isTableCellOpened))
		return;
		 
	if (!m_ps->m_isSpanOpened)
		_openSpan();
	else
		_flushText();

	WPXPropertyList propList;

	propList.insert("svg:height", (double)((double)height/(double)WPX_NUM_WPUS_PER_INCH));
	propList.insert("svg:width", (double)((double)width/(double)WPX_NUM_WPUS_PER_INCH));

	if ( alignment & 0x80 )
		propList.insert( "style:wrap", "dynamic" );
	else
		propList.insert( "style:wrap", "none" );


	switch (positionAndType & 0x03)
	{
	case 0x00: // Paragraph anchored
		propList.insert("text:anchor-type", "paragraph");
		break;
	case 0x01: // Page anchored
		propList.insert("text:anchor-type", "char"); // ugly workaround to remediate OOo's wrong implementation of ODF
		break;
	case 0x02: // Char
		propList.insert("text:anchor-type", "as-char");
		break;
	default:
		break;
	}
	
	propList.insert("style:vertical-rel", "page-content");
	switch ( (positionAndType &  0x1c) >> 2 )
	{
	case 0x00:
		propList.insert("svg:height", (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom ) );
		propList.insert("style:vertical-rel", "page-content" );
		propList.insert("style:vertical-pos", "middle" );
		break;
	case 0x01:
		if ( y == 0.0)
			propList.insert("style:vertical-pos", "top" );
		else
		{
			propList.insert("style:vertical-pos", "from-top" );
			double newPosition = (double)((double)y/(double)WPX_NUM_WPUS_PER_INCH);
			if (newPosition > (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
				- (double)height/(double)WPX_NUM_WPUS_PER_INCH) )
			{
				newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
				- (double)height/(double)WPX_NUM_WPUS_PER_INCH);
			}
			propList.insert("svg:y", newPosition);
		}
		break;
	case 0x02:
		if (y == 0.0)
			propList.insert("style:vertical-pos", "middle" );
		else
		{
			propList.insert("style:vertical-pos", "from-top" );
			double newPosition = (double)((m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
				- (double)height/(double)WPX_NUM_WPUS_PER_INCH)/2.0);
			if (newPosition > (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
				- (double)height/(double)WPX_NUM_WPUS_PER_INCH) )
			{
				newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
					- (double)height/(double)WPX_NUM_WPUS_PER_INCH);
			}
			propList.insert("svg:y", newPosition);
		}
		break;
	case 0x03:
		if (y == 0.0)
			propList.insert("style:vertical-pos", "bottom" );
		else
		{
			propList.insert("style:vertical-pos", "from-top" );
			double newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
				- (double)height/(double)WPX_NUM_WPUS_PER_INCH + (double)y/(double)WPX_NUM_WPUS_PER_INCH);
			if (newPosition > (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
				- (double)height/(double)WPX_NUM_WPUS_PER_INCH) )
			{
				newPosition = (double)(m_ps->m_pageFormLength - m_ps->m_pageMarginTop - m_ps->m_pageMarginBottom
					- (double)height/(double)WPX_NUM_WPUS_PER_INCH);
			}
			propList.insert("svg:y", newPosition);
		}
		break;
	case 0x04:
		propList.insert("style:vertical-rel", "page" );
		propList.insert("style:vertical-pos", "from-top" );
		propList.insert("svg:y", (double)((double)y/(double)WPX_NUM_WPUS_PER_INCH));
		break;
	default:
		break;
	}
	
	propList.insert("style:horizontal-rel", "page-content" );
	switch ( alignment & 0x03 )
	{
	case 0x00:
		if ( x == 0.0 )
			propList.insert( "style:horizontal-pos", "left");
		else
		{
			propList.insert( "style:horizontal-pos", "from-left");
			propList.insert( "svg:x", (double)((double)x/(double)WPX_NUM_WPUS_PER_INCH));
		}
		break;
	case 0x01:
		if ( x == 0.0 )
			propList.insert( "style:horizontal-pos", "right");
		else
		{
			propList.insert( "style:horizontal-pos", "from-left");
			propList.insert( "svg:x", (double)(m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight
				- (double)width/(double)WPX_NUM_WPUS_PER_INCH + (double)x/(double)WPX_NUM_WPUS_PER_INCH));
		}
		break;
	case 0x02:
		if ( x == 0.0 )
			propList.insert( "style:horizontal-pos", "center" );
		else
		{
			propList.insert( "style:horizontal-pos", "from-left");
			propList.insert( "svg:x", (double)((m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight
				- (double)width/(double)WPX_NUM_WPUS_PER_INCH)/2.0 + (double)x/(double)WPX_NUM_WPUS_PER_INCH));
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
	
	m_documentInterface->openFrame(propList);
	m_parseState->m_isFrameOpened = true;
}

void WP5ContentListener::boxOff()
{
	if (!isUndoOn() && m_parseState->m_isFrameOpened)
	{
		m_documentInterface->closeFrame();
		m_parseState->m_isFrameOpened = false;
	}
}

void WP5ContentListener::insertGraphicsData(const WPXBinaryData *data)
{
	if (isUndoOn() || !m_parseState->m_isFrameOpened)
		return;

	if (data) 
	{
		WPXPropertyList propList;
		propList.insert("libwpd:mimetype", "image/x-wpg");
		m_documentInterface->insertBinaryObject(propList, *data);
	}
}


/****************************************
 private functions
*****************************************/

void WP5ContentListener::_flushText()
{
	_insertText(m_parseState->m_textBuffer);
	m_parseState->m_textBuffer.clear();
}

