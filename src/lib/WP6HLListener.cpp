/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "WP6HLListener.h"
#include "WPXHLListenerImpl.h"
#include "WP6FileStructure.h" 
#include "WPXFileStructure.h"

WP6HLListener::WP6HLListener(WPXHLListenerImpl *listenerImpl) :
	WP6LLListener(), 
	m_listenerImpl(listenerImpl),
	m_textArray(g_array_new(TRUE, FALSE, sizeof(guint16))),
	m_textAttributeBits(0),
	m_textAttributesChanged(FALSE),
	
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
	m_paragraphJustificationChanged(FALSE),
	m_isParagraphOpened(FALSE),
	m_isParagraphClosed(FALSE),
	m_numDeferredParagraphBreaks(0),
	
	m_sectionAttributesChanged(TRUE),
	m_numColumns(1),
	m_marginLeft(1.0f),
	m_marginRight(1.0f),
	m_currentRow(-1),
	m_currentColumn(-1),
	
	m_isUndoOn(FALSE)
{
}

WP6HLListener::~WP6HLListener()
{
	g_array_free(m_textArray, TRUE);
}

void WP6HLListener::insertCharacter(guint16 character)
{
	if (!m_isUndoOn)
		g_array_append_val(m_textArray, character);
}

void WP6HLListener::insertEOL()
{
	if (!m_isUndoOn)
	{
		_flushText();
		m_numDeferredParagraphBreaks++;
	}
}

void WP6HLListener::insertBreak(guint8 breakType)
{
	if (!m_isUndoOn)
	{	
		_flushText();
		m_numDeferredParagraphBreaks++;
		switch (breakType) 
			{
			case WPX_COLUMN_BREAK:
				m_isParagraphColumnBreak = TRUE;
				break;
			case WPX_PAGE_BREAK:
				m_isParagraphPageBreak = TRUE;
				break;
			// TODO: (.. line break?)
			}
		//m_listenerImpl->insertBreak(breakType);
	}
}

void WP6HLListener::startDocument()
{
	m_listenerImpl->startDocument();
}

void WP6HLListener::undoChange(guint8 undoType, guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START)
		m_isUndoOn = TRUE;
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END)
		m_isUndoOn = FALSE;		
}

void WP6HLListener::attributeChange(gboolean isOn, guint8 attribute)
{
	if (!m_isUndoOn)
	{
		// flush everything which came before this change
		_flushText();
		
		guint32 textAttributeBit = 0;
		
		// FIXME: handle all the possible attribute bits
		switch (attribute)
		{
			case WP6_ATTRIBUTE_SUBSCRIPT:
				textAttributeBit = WPX_SUBSCRIPT_BIT;
				break;
			case WP6_ATTRIBUTE_SUPERSCRIPT:
				textAttributeBit = WPX_SUPERSCRIPT_BIT;
				break;
			case WP6_ATTRIBUTE_ITALICS:
				textAttributeBit = WPX_ITALICS_BIT;
				break;
			case WP6_ATTRIBUTE_BOLD:
				textAttributeBit = WPX_BOLD_BIT;
				break;
			case WP6_ATTRIBUTE_STRIKE_OUT:
				textAttributeBit = WPX_STRIKEOUT_BIT;
				break;
			case WP6_ATTRIBUTE_UNDERLINE:
				textAttributeBit = WPX_UNDERLINE_BIT;
				break;
		}
		
		if (isOn) 
			m_textAttributeBits |= textAttributeBit;
		else
			m_textAttributeBits ^= textAttributeBit;
		
		m_textAttributesChanged = TRUE;
	}
}

void WP6HLListener::justificationChange(guint8 justification)
{
	if (!m_isUndoOn)
	{
		switch (justification)
		{
			case WP6_PARAGRAPH_JUSTIFICATION_LEFT:
				m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_LEFT;
				break;
			case WP6_PARAGRAPH_JUSTIFICATION_FULL:
				m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_LEFT;
				break;
			case WP6_PARAGRAPH_JUSTIFICATION_CENTER:
				m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
				break;
			case WP6_PARAGRAPH_JUSTIFICATION_RIGHT:
				m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
				break;
			case WP6_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES:
				m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES;
				break;
			case WP6_PARAGRAPH_JUSTIFICATION_RESERVED:
				m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RESERVED;
				break;
		}
		
		m_paragraphJustificationChanged = TRUE;
	}
}

void WP6HLListener::marginChange(guint8 side, guint16 margin)
{
	if (!m_isUndoOn)
	{
		// flush everything which came before this change
		// eliminating one paragraph break which is now implicit in
		// a paragraph change -- UNLESS the paragraph break represents
		// something else than its name suggests, such as a paragraph
		// or column break (FIXME: probably should move this to a general
		// helper function when we add more section changing messages)
		if (!m_sectionAttributesChanged && m_numDeferredParagraphBreaks > 0 &&
		    !m_isParagraphColumnBreak && !m_isParagraphPageBreak)
			m_numDeferredParagraphBreaks--;		

		_flushText();
		
		float marginInch = ((float)margin) / (float)WPX_NUM_WPUS_PER_INCH;

		switch(side)
		{
			case WP6_COLUMN_GROUP_LEFT_MARGIN_SET:
				m_marginLeft = marginInch;
				break;
			case WP6_COLUMN_GROUP_RIGHT_MARGIN_SET:
				m_marginRight = marginInch;
				break;
		}

		m_sectionAttributesChanged = TRUE;
	}	
}

void WP6HLListener::columnChange(guint8 numColumns)
{
	if (!m_isUndoOn)
	{
		// flush everything which came before this change
		// eliminating one paragraph break which is now implicit in
		// a paragraph change -- UNLESS the paragraph break represents
		// something else than its name suggests, such as a paragraph
		// or column break (FIXME: probably should move this to a general
		// helper function when we add more section changing messages)
		if (!m_sectionAttributesChanged && m_numDeferredParagraphBreaks > 0 &&
		    !m_isParagraphColumnBreak && !m_isParagraphPageBreak)
			m_numDeferredParagraphBreaks--;		
		
		_flushText();
		
		m_numColumns = numColumns;
		
		m_sectionAttributesChanged = TRUE;
	}
}

void WP6HLListener::endDocument()
{
	// corner case: document contains no end of lines
	if (!m_isParagraphOpened && !m_isParagraphClosed)
	{
		m_listenerImpl->openParagraph(m_paragraphJustification, m_textAttributeBits,
					      FALSE, FALSE);
		_flushText();       
	}
	else if (!m_isParagraphClosed || !m_isParagraphOpened)
	{
		_flushText();
	}
	
	// the only other possibility is a logical contradiction: a paragraph
	// may not be opened and closed at the same time

	m_listenerImpl->endDocument();
}

void WP6HLListener::startTable()
{
	if (!m_isUndoOn) 
		{			
			_flushText();
			m_listenerImpl->openTable();
		}
}

void WP6HLListener::insertRow()
{
	if (!m_isUndoOn) 
		{			
			_flushText();
			m_currentRow++;
			m_currentColumn = -1;
			m_listenerImpl->openRow();
		}
}

void WP6HLListener::insertCell(guint8 colSpan, guint8 rowSpan, gboolean boundFromLeft, gboolean boundFromAbove, RGBSColor * cellFgColor, RGBSColor * cellBgColor)
{
	if (!m_isUndoOn) 
		{			
			_flushText();
			m_currentColumn++;
			if (!boundFromLeft && !boundFromAbove) {
				m_listenerImpl->openCell(m_currentColumn, m_currentRow, colSpan, rowSpan, cellFgColor, cellBgColor);
				m_numDeferredParagraphBreaks++;
			}
		}
}

void WP6HLListener::endTable()
{
	if (!m_isUndoOn) 
		{			
			_flushText();
			m_listenerImpl->closeTable();
			m_currentRow = 0;
			m_currentColumn = 0;
			m_numDeferredParagraphBreaks++;
		}
}

void WP6HLListener::_flushText()
{		
	if (m_sectionAttributesChanged && m_textArray->len > 0)
	{
		m_listenerImpl->openSection(m_numColumns, m_marginLeft, m_marginRight);
		m_listenerImpl->openParagraph(m_paragraphJustification, m_textAttributeBits,
					      m_isParagraphColumnBreak, m_isParagraphPageBreak);
		m_isParagraphColumnBreak = FALSE; m_isParagraphPageBreak = FALSE;
		m_isParagraphOpened = TRUE;
		m_sectionAttributesChanged = FALSE;
		if (m_numDeferredParagraphBreaks > 0)
			m_numDeferredParagraphBreaks--;
	}

	if (m_numDeferredParagraphBreaks > 0)
	{
		while (m_numDeferredParagraphBreaks > 0)
		{
			m_listenerImpl->openParagraph(m_paragraphJustification, m_textAttributeBits, 
						      m_isParagraphColumnBreak, m_isParagraphPageBreak);
			m_isParagraphColumnBreak = FALSE; m_isParagraphPageBreak = FALSE;
			m_numDeferredParagraphBreaks--;
		}
		m_isParagraphOpened = TRUE;
	}
	else if (m_textAttributesChanged && m_textArray->len > 0)
		m_listenerImpl->openSpan(m_textAttributeBits);

	m_listenerImpl->insertText((guint16 *)m_textArray->data, m_textArray->len);
	
	m_textArray = g_array_set_size(m_textArray, 0);	

	m_textAttributesChanged = FALSE;
	m_paragraphJustificationChanged = FALSE;
}
