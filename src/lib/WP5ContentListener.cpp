/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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
	m_tableList()
{
}

_WP5ContentParsingState::~_WP5ContentParsingState()
{
}

WP5ContentListener::WP5ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP5SubDocument*> &subDocuments, WPXDocumentInterface *listenerImpl) :
	WP5Listener(),
	WPXContentListener(pageList, listenerImpl),
	m_parseState(new WP5ContentParsingState),
	m_subDocuments(subDocuments),
	m_defaultFontSize(12.0f),
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

void WP5ContentListener::insertCharacter(const uint16_t character)
{
	if (!m_ps->m_isSpanOpened)
		_openSpan();
	appendUCS4(m_parseState->m_textBuffer, (uint32_t)character);
}

void WP5ContentListener::insertTab(const uint8_t /* tabType */, float /* tabPosition */)
{
	if (!m_ps->m_isSpanOpened)
		_openSpan();
	else
		_flushText();

	m_documentInterface->insertTab();
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

void WP5ContentListener::defineTable(const uint8_t position, const uint16_t leftOffset)
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
		m_ps->m_tableDefinition.m_columns.clear();
		m_ps->m_tableDefinition.m_columnsProperties.clear();
		m_ps->m_numRowsToSkip.clear();
	}
}

void WP5ContentListener::addTableColumnDefinition(const uint32_t width, const uint32_t /* leftGutter */,
						const uint32_t /* rightGutter */, const uint32_t attributes, const uint8_t alignment)
{
	if (!isUndoOn())
	{
		// define the new column
		WPXColumnDefinition colDef;
		colDef.m_width = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_leftGutter = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_rightGutter = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);

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

void WP5ContentListener::insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow)
{
	if (!isUndoOn())
	{
		_flushText();
		float rowHeightInch = (float)((double) rowHeight / (double)WPX_NUM_WPUS_PER_INCH);
		_openTableRow(rowHeightInch, isMinimumHeight, isHeaderRow);
	}
}

void WP5ContentListener::insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits,
			const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
			const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
			const bool useCellAttributes, const uint32_t cellAttributes)
{
	if (!isUndoOn())
	{
		if (m_ps->m_currentTableRow < 0) // cell without a row, invalid
			throw ParseException();
		_flushText();
		_openTableCell(colSpan, rowSpan, borderBits, cellFgColor, cellBgColor,
					cellBorderColor, cellVerticalAlignment);
		m_ps->m_isCellWithoutParagraph = true;
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

void WP5ContentListener::attributeChange(const bool isOn, const uint8_t attribute)
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
		float marginInch = (float)((double)margin/ (double)WPX_NUM_WPUS_PER_INCH);

		switch(side)
		{
		case WPX_LEFT:
			if (m_ps->m_numColumns > 1)
			{
				m_ps->m_leftMarginByPageMarginChange = 0.0f;
				m_ps->m_sectionMarginLeft = marginInch - m_ps->m_pageMarginLeft;
			}
			else
			{
				m_ps->m_leftMarginByPageMarginChange = marginInch - m_ps->m_pageMarginLeft;
				m_ps->m_sectionMarginLeft = 0.0f;
			}
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
						+ m_ps->m_leftMarginByParagraphMarginChange
						+ m_ps->m_leftMarginByTabs;
			break;
		case WPX_RIGHT:
			if (m_ps->m_numColumns > 1)
			{
				m_ps->m_rightMarginByPageMarginChange = 0.0f;
				m_ps->m_sectionMarginRight = marginInch - m_ps->m_pageMarginRight;
			}
			else
			{
				m_ps->m_rightMarginByPageMarginChange = marginInch - m_ps->m_pageMarginRight;
				m_ps->m_sectionMarginRight = 0.0f;
			}
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
						+ m_ps->m_rightMarginByParagraphMarginChange
						+ m_ps->m_rightMarginByTabs;
			break;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP5ContentListener::characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue)
{
	if (!isUndoOn())
	{
		_closeSpan();
		m_ps->m_fontColor->m_r = red;
		m_ps->m_fontColor->m_g = green;
		m_ps->m_fontColor->m_b = blue;
 	}
}

void WP5ContentListener::setFont(const WPXString &fontName, const float fontSize)
{
	if (!isUndoOn())
	{
		_closeSpan();
		*(m_ps->m_fontName) = fontName;
		m_ps->m_fontSize = fontSize;
	}
}

void WP5ContentListener::insertNoteReference(const WPXString &noteReference)
{
	if (!isUndoOn() && !m_ps->m_isNote)
	{
		m_parseState->m_noteReference = noteReference;
	}
}

void WP5ContentListener::insertNote(const WPXNoteType noteType, const WP5SubDocument *subDocument)
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
			m_documentInterface->openFootnote(propList);
		else
			m_documentInterface->openEndnote(propList);

		handleSubDocument(subDocument, false, m_parseState->m_tableList, 0);

		if (noteType == FOOTNOTE)
			m_documentInterface->closeFootnote();
		else
			m_documentInterface->closeEndnote();
		m_ps->m_isNote = false;
	}
}

void WP5ContentListener::_handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter,
				WPXTableList /* tableList */, int /* nextTableIndice */)
{
	// save our old parsing state on our "stack"
	WP5ContentParsingState *oldParseState = m_parseState;

	m_parseState = new WP5ContentParsingState();
	setFont(m_defaultFontName, m_defaultFontSize);

	if (isHeaderFooter)
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

void WP5ContentListener::headerFooterGroup(const uint8_t /* headerFooterType */, const uint8_t /* occurenceBits */,
						WP5SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);
}	

void WP5ContentListener::setDefaultFont(const WPXString &fontName, const float fontSize)
{
	m_defaultFontName = fontName;
	m_defaultFontSize = fontSize;
}


/****************************************
 private functions
*****************************************/

void WP5ContentListener::_flushText()
{
	if (m_parseState->m_textBuffer.len())
		m_documentInterface->insertText(m_parseState->m_textBuffer);
	m_parseState->m_textBuffer.clear();
}

