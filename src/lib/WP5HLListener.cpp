/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "WP5HLListener.h"
#include "WP5FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

_WP5ParsingState::_WP5ParsingState()
{
}

_WP5ParsingState::~_WP5ParsingState()
{
}

WP5HLListener::WP5HLListener(std::vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl) :
	WPXHLListener(pageList, listenerImpl),
	WP5LLListener(),
	m_parseState(new WP5ParsingState)
{
	m_textBuffer.clear();
}

WP5HLListener::~WP5HLListener() 
{
	delete m_parseState;
}


/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP5HLListener::insertCharacter(const uint16_t character)
{
	appendUCS4(m_textBuffer, (uint32_t)character);
}

void WP5HLListener::insertTab(const uint8_t tabType, const float tabPosition)
{
	_flushText();
	m_listenerImpl->insertTab();
}

void WP5HLListener::insertEOL()
{
	_flushText();
	m_ps->m_numDeferredParagraphBreaks++;
}

void WP5HLListener::endDocument()
{
	// corner case: document ends in a list element
	/*if (m_parseState->m_styleStateSequence.getCurrentState() != NORMAL)
	{
		_flushText(); // flush the list text
		m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
		_flushText(true); // flush the list exterior (forcing a line break, to make _flushText think we've exited a list)
	}*/
	// corner case: document contains no end of lines
	/*else*/ if (!m_ps->m_isParagraphOpened && !m_ps->m_isParagraphClosed)
	{
		_flushText();
	}
	// NORMAL(ish) case document ends either inside a paragraph or outside of one,
	// but not inside an object
	else if (!m_ps->m_isParagraphClosed || !m_ps->m_isParagraphOpened)
	{
		_flushText();
	}

	// the only other possibility is a logical contradiction: a paragraph
	// may not be opened and closed at the same time

	// close the document nice and tight
	_closeSection();
	_closePageSpan();
	m_listenerImpl->endDocument();
}


/****************************************
 public 'parser' functions
*****************************************/

void WP5HLListener::attributeChange(const bool isOn, const uint8_t attribute)
{

	// flush everything which came before this change
	_flushText();

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

	m_ps->m_textAttributesChanged = true;
}

void WP5HLListener::marginChange(uint8_t side, uint16_t margin)
{
	//if (!isUndoOn())
	//{
		float marginInch = (float)((double)margin/ (double)WPX_NUM_WPUS_PER_INCH);
		bool marginChanged = false;

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
	//}
}


/****************************************
 private functions
*****************************************/

void WP5HLListener::_flushText(const bool fakeText)
{
	// create a new section, and a new paragraph, if our section attributes have changed and we have inserted
	// something into the document (or we have forced a break, which assumes the same condition)
	if (m_ps->m_sectionAttributesChanged && (m_textBuffer.len() > 0 || m_ps->m_numDeferredParagraphBreaks > 0/* || fakeText*/))
	{
		_openSection();
		//if (fakeText)
			_openParagraph();
	}

	if (m_ps->m_numDeferredParagraphBreaks > 0)
	{
		if (!m_ps->m_isParagraphOpened //&&
			// !(m_parseState->m_isTableOpened && !m_parseState->m_isTableCellOpened) // don't allow paragraphs to be opened when we have already opened a table, but no cell yet. - MARCM (is it really correct, or should this be fixed elsewhere??)
		)
			m_ps->m_numDeferredParagraphBreaks++;

		while (m_ps->m_numDeferredParagraphBreaks > 1)
			_openParagraph();
		_closeParagraph();
		m_ps->m_numDeferredParagraphBreaks = 0; // compensate for this by requiring a paragraph to be opened
	}
	else if (m_ps->m_textAttributesChanged && m_textBuffer.len())
	{
		_openSpan();
		m_ps->m_textAttributesChanged = false;
	}

	if (m_textBuffer.len())
	{
		if (!m_ps->m_isParagraphOpened)
		{
			_openParagraph();
			_openSpan();
		}

		m_listenerImpl->insertText(m_textBuffer);
		m_textBuffer.clear();
	}
}
