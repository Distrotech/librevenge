/* libwpd
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3HLListener.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"

_WP3ParsingState::_WP3ParsingState()
{
}

_WP3ParsingState::~_WP3ParsingState()
{
}

WP3HLListener::WP3HLListener(vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl) :
	WPXHLListener(pageList, listenerImpl),
	WP3LLListener(),
	m_parseState(new WP3ParsingState)
{
	m_textBuffer.clear();
}

WP3HLListener::~WP3HLListener() 
{
	delete m_parseState;
}


/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP3HLListener::insertCharacter(const uint16_t character)
{
        if (!isUndoOn())
		m_textBuffer.append(character);
}

void WP3HLListener::insertTab(const uint8_t tabType, const float tabPosition)
{
        if (!isUndoOn())
	{
		_flushText();
		m_listenerImpl->insertTab();
	}
}

void WP3HLListener::insertEOL()
{
        if (!isUndoOn())
	{
		_flushText();
		m_ps->m_numDeferredParagraphBreaks++;
	}
}

void WP3HLListener::endDocument()
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

void WP3HLListener::attributeChange(const bool isOn, const uint8_t attribute)
{
        if (!isUndoOn())
	{
		// flush everything which came before this change
		_flushText();

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

		m_ps->m_textAttributesChanged = true;
	}
}

void WP3HLListener::undoChange(const uint8_t undoType, const uint16_t undoLevel)
{
        if (undoType == 0x00) // begin invalid text
                m_isUndoOn = true;
        else if (undoType == 0x01) // end invalid text
                m_isUndoOn = false;
}

void WP3HLListener::marginChange(uint8_t side, uint16_t margin)
{
	if (!isUndoOn())
	{
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
	}
}

void WP3HLListener::indentFirstLineChange(int16_t offset)
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
	}
}



/****************************************
 private functions
*****************************************/

void WP3HLListener::_flushText(const bool fakeText)
{
	// create a new section, and a new paragraph, if our section attributes have changed and we have inserted
	// something into the document (or we have forced a break, which assumes the same condition)
	if (m_ps->m_sectionAttributesChanged && (m_textBuffer.getLen() > 0 || m_ps->m_numDeferredParagraphBreaks > 0 /*|| fakeText*/))
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
	else if (m_ps->m_textAttributesChanged && m_textBuffer.getLen())
	{
		_openSpan();
		m_ps->m_textAttributesChanged = false;
	}

	if (m_textBuffer.getLen())
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

void WP3HLListener::_openParagraph()
{
	_closeParagraph();
	/*uint8_t paragraphJustification;
	(m_parseState->m_tempParagraphJustification != 0) ? paragraphJustification = m_parseState->m_tempParagraphJustification :
		paragraphJustification = m_parseState->m_paragraphJustification;
	m_parseState->m_tempParagraphJustification = 0;
	*/

	WPXTabStop tmp_tabStop;
	vector<WPXTabStop> tabStops;
	for (int i=0; i<m_ps->m_tabStops.size(); i++)
	{
		tmp_tabStop = m_ps->m_tabStops[i];
		if (m_ps->m_isTabPositionRelative)
			tmp_tabStop.m_position -= m_ps->m_leftMarginByTabs;
		else
			tmp_tabStop.m_position -= m_ps->m_paragraphMarginLeft + m_ps->m_pageMarginLeft;
		/* TODO: fix situations where we have several columns or are inside a table and the tab stop
		 *       positions are absolute (relative to the paper edge). In this case, they have to be
		 *       computed for each column or each cell in table. (Fridrich) */
		tabStops.push_back(tmp_tabStop);
	}

	m_listenerImpl->openParagraph(0,
				      m_ps->m_paragraphMarginLeft, m_ps->m_paragraphMarginRight, m_ps->m_paragraphTextIndent,
				      m_ps->m_paragraphLineSpacing, m_ps->m_paragraphSpacingBefore, m_ps->m_paragraphSpacingAfter,
				      tabStops, m_ps->m_isParagraphColumnBreak, m_ps->m_isParagraphPageBreak);

	if (m_ps->m_numDeferredParagraphBreaks > 0)
		m_ps->m_numDeferredParagraphBreaks--;

	m_ps->m_isParagraphColumnBreak = false;
	m_ps->m_isParagraphPageBreak = false;
	m_ps->m_isParagraphOpened = true;
	m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange + m_ps->m_leftMarginByParagraphMarginChange;
	m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange + m_ps->m_rightMarginByParagraphMarginChange;
	m_ps->m_leftMarginByTabs = 0.0f;
	m_ps->m_rightMarginByTabs = 0.0f;
	m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange;
	m_ps->m_textIndentByTabs = 0.0f;	

	_openSpan();
}
