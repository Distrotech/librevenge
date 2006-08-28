/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
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

#include "WP1ContentListener.h"
#include "WP1FileStructure.h"
#include "libwpd_internal.h"

#define WP1_NUM_TEXT_COLUMS_PER_INCH 12

_WP1ContentParsingState::_WP1ContentParsingState() :
	m_numDeferredTabs(0)
{
	m_textBuffer.clear();
}

_WP1ContentParsingState::~_WP1ContentParsingState()
{
	m_textBuffer.clear();
}


WP1ContentListener::WP1ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP1SubDocument *> &subDocuments, WPXHLListenerImpl *listenerImpl) :
	WP1Listener(),
	WPXContentListener(pageList, listenerImpl),
	m_subDocuments(subDocuments),
	m_parseState(new WP1ContentParsingState)
{
// Default line is 6 lpi, it means that the caracters are of 12 points
	*(m_ps->m_fontName) = "Geneva";
	m_ps->m_fontSize = 12.0f;
}

WP1ContentListener::~WP1ContentListener() 
{
	delete m_parseState;
}


void WP1ContentListener::insertCharacter(const uint16_t character)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		for (;m_parseState->m_numDeferredTabs > 0; m_parseState->m_numDeferredTabs--)
		{
			m_listenerImpl->insertTab();
		}
		appendUCS4(m_parseState->m_textBuffer, (uint32_t)character);
	}
}

void WP1ContentListener::insertExtendedCharacter(const uint8_t extendedCharacter)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		for (;m_parseState->m_numDeferredTabs > 0; m_parseState->m_numDeferredTabs--)
		{
			m_listenerImpl->insertTab();
		}
		if (extendedCharacter <= 0x20)
			appendUCS4(m_parseState->m_textBuffer, (uint32_t)0x20);
		else
			appendUCS4(m_parseState->m_textBuffer, (uint32_t)macintoshCharacterMap[extendedCharacter - 0x20]);
	}
}

void WP1ContentListener::insertTab()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened)
			m_parseState->m_numDeferredTabs++;
		else
		{
			if (!m_ps->m_isSpanOpened)
				_openSpan();
			else
				_flushText();

			m_listenerImpl->insertTab();
		}
	}
}

void WP1ContentListener::insertEOL()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
			_openSpan();
		for (;m_parseState->m_numDeferredTabs > 0; m_parseState->m_numDeferredTabs--)
		{
			m_listenerImpl->insertTab();
		}

		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();
	}
}

void WP1ContentListener::attributeChange(const bool isOn, const uint8_t attribute)
{
	_closeSpan();

	uint32_t textAttributeBit = 0;

	switch (attribute)
	{
	case WP1_ATTRIBUTE_SUBSCRIPT:
		textAttributeBit = WPX_SUBSCRIPT_BIT;
		break;
	case WP1_ATTRIBUTE_SUPERSCRIPT:
		textAttributeBit = WPX_SUPERSCRIPT_BIT;
		break;
	case WP1_ATTRIBUTE_OUTLINE:
		textAttributeBit = WPX_OUTLINE_BIT;
		break;
	case WP1_ATTRIBUTE_ITALICS:
		textAttributeBit = WPX_ITALICS_BIT;
		break;
	case WP1_ATTRIBUTE_SHADOW:
		textAttributeBit = WPX_SHADOW_BIT;
		break;
	case WP1_ATTRIBUTE_REDLINE:
		textAttributeBit = WPX_REDLINE_BIT;
		break;
	case WP1_ATTRIBUTE_BOLD:
		textAttributeBit = WPX_BOLD_BIT;
		break;
	case WP1_ATTRIBUTE_STRIKE_OUT:
		textAttributeBit = WPX_STRIKEOUT_BIT;
		break;
	case WP1_ATTRIBUTE_UNDERLINE:
		textAttributeBit = WPX_UNDERLINE_BIT;
		break;
	}

	if (isOn)
		m_ps->m_textAttributeBits |= textAttributeBit;
	else
		m_ps->m_textAttributeBits ^= textAttributeBit;
}

void WP1ContentListener::fontPointSize(const uint8_t pointSize)
{
	if (!isUndoOn())
	{
		_closeSpan();
		
		m_ps->m_fontSize=float(pointSize);
	}
}

void WP1ContentListener::marginReset(const uint16_t leftMargin, const uint16_t rightMargin)
{
	if (!isUndoOn())
	{
		if (leftMargin)
		{
			float leftMarginInch = (float)((double)leftMargin/72.0f);
			m_ps->m_leftMarginByPageMarginChange = leftMarginInch - m_ps->m_pageMarginLeft;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
						+ m_ps->m_leftMarginByParagraphMarginChange
						+ m_ps->m_leftMarginByTabs;

		}
		if (rightMargin)
		{
			float rightMarginInch = (float)((double)rightMargin/72.0f);
			m_ps->m_rightMarginByPageMarginChange = rightMarginInch - m_ps->m_pageMarginRight;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
						+ m_ps->m_rightMarginByParagraphMarginChange
						+ m_ps->m_rightMarginByTabs;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP1ContentListener::leftIndent(const uint16_t leftMarginOffset)
{
	if (!isUndoOn())
	{
		if (m_ps->m_isParagraphOpened)
			insertTab();
		else
		{
			m_parseState->m_numDeferredTabs = 0;
			m_ps->m_leftMarginByTabs = (float)((double)leftMarginOffset / 72.0f);
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
						+ m_ps->m_leftMarginByParagraphMarginChange
						+ m_ps->m_leftMarginByTabs;
			m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft
						+ m_ps->m_paragraphTextIndent;
		}
	}
}

void WP1ContentListener::leftRightIndent(const uint16_t leftRightMarginOffset)
{
	if (!isUndoOn())
	{
		if (m_ps->m_isParagraphOpened)
			insertTab();
		else
		{
			m_parseState->m_numDeferredTabs = 0;
			m_ps->m_leftMarginByTabs = m_ps->m_rightMarginByTabs =
						(float)((double)leftRightMarginOffset / 72.0f);
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
						+ m_ps->m_leftMarginByParagraphMarginChange
						+ m_ps->m_leftMarginByTabs;
			m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft
						+ m_ps->m_paragraphTextIndent;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
						+ m_ps->m_rightMarginByParagraphMarginChange
						+ m_ps->m_rightMarginByTabs;
		}
	}
}

void WP1ContentListener::leftMarginRelease(const uint16_t release)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened)
		{
			if (!m_parseState->m_numDeferredTabs)
			{
				m_ps->m_textIndentByTabs -= (float)((double)release / 72.0f);
				m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
					+ m_ps->m_textIndentByTabs;
			}
			else
				m_parseState->m_numDeferredTabs--;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP1ContentListener::justificationChange(const uint8_t justification)
{
	if (!isUndoOn())
	{
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
		}
	}
}

void WP1ContentListener::headerFooterGroup(const uint8_t headerFooterDefinition, WP1SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);			
}	

void WP1ContentListener::setTabs(const std::vector<WPXTabStop> tabStops)
{
	if (!isUndoOn())
	{
		m_ps->m_isTabPositionRelative = false;
		m_ps->m_tabStops = tabStops;
	}
}

void WP1ContentListener::_handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice)
{
	// save our old parsing state on our "stack"
	WP1ContentParsingState *oldParseState = m_parseState;

	m_parseState = new WP1ContentParsingState();

	if (subDocument)
	{
		static_cast<const WP1SubDocument *>(subDocument)->parse(this);
	}
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

/****************************************
 private functions
*****************************************/

void WP1ContentListener::_flushText()
{
	if (m_parseState->m_textBuffer.len())
		m_listenerImpl->insertText(m_parseState->m_textBuffer);
	m_parseState->m_textBuffer.clear();
}
