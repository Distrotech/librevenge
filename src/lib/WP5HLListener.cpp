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

_WP5ParsingState::_WP5ParsingState(bool sectionAttributesChanged) :
	m_textAttributeBits(0),
	m_textAttributesChanged(false),
	//m_fontSize(WP6_DEFAULT_FONT_SIZE),
	//fontName(g_string_new(WP5_DEFAULT_FONT_NAME)),
	
	m_isParagraphColumnBreak(false),
	m_isParagraphPageBreak(false),
	m_paragraphLineSpacing(1.0f),
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
	m_tempParagraphJustification(0),

	m_isSectionOpened(false),

	m_isParagraphOpened(false),
	m_isParagraphClosed(false),
	m_isSpanOpened(false),
	m_numDeferredParagraphBreaks(0),
	m_numRemovedParagraphBreaks(0)

	/*m_currentTable(NULL),
	m_nextTableIndice(0),
	m_currentTableCol(0),
	m_currentTableRow(0),
	m_isTableOpened(false),
	m_isTableRowOpened(false),
	m_isTableCellOpened(false),
	
	m_isPageSpanOpened(false),
	m_nextPageSpanIndice(0),
	m_numPagesRemainingInSpan(0),

	m_sectionAttributesChanged(sectionAttributesChanged),
	m_numColumns(1),
	m_marginLeft(0.0f), 
	m_marginRight(0.0f),
	m_pageMarginLeft(1.0f),
	m_pageMarginRight(1.0f),
	m_currentRow(-1),
	m_currentColumn(-1),
	
	m_currentListLevel(0),
	m_putativeListElementHasParagraphNumber(false),
	m_putativeListElementHasDisplayReferenceNumber(false),

	m_noteTextPID(0),
	m_inSubDocument(false)*/
{
}

_WP5ParsingState::~_WP5ParsingState()
{	
	// fixme: erase current fontname
}

WP5HLListener::WP5HLListener(WPXHLListenerImpl *listenerImpl)
	: WPXHLListener(),WP5LLListener(),
	m_listenerImpl(listenerImpl),
	m_parseState(new WP5ParsingState)
{
	m_textBuffer.clear();
}

/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP5HLListener::startDocument()
{
	m_listenerImpl->startDocument();	
	// FIXME: use the actual values, instead of making up some
	m_listenerImpl->openPageSpan(0, true,
				  1.0, 1.0,
				  1.0, 1.0);
	m_listenerImpl->openSection(1, 0.0f);
	m_listenerImpl->openParagraph(0, 0, 0, 0,
					"Times New Roman", 12.0f,
					1.0f,
					false, false);
	m_listenerImpl->openSpan(0, "Times New Roman", 12.0f);	
}

void WP5HLListener::insertCharacter(const guint16 character)
{
	m_textBuffer.append(character);
}

void WP5HLListener::insertTab(const guint8 tabType)
{
	_flushText();
	m_listenerImpl->insertTab();
}

void WP5HLListener::insertEOL()
{
	_flushText();
	m_listenerImpl->insertLineBreak();
	m_listenerImpl->openParagraph(0, 0, 0, 0,
					"Times New Roman", 12.0f,
					1.0f,
					false, false);
	_openSpan();	
}

void WP5HLListener::endDocument()
{
	_flushText();
	m_listenerImpl->endDocument();	
}


/****************************************
 public 'parser' functions
*****************************************/

void WP5HLListener::attributeChange(const bool isOn, const guint8 attribute)
{
	
	// flush everything which came before this change
	_flushText();
	
	guint32 textAttributeBit = 0;
	
	// FIXME: handle all the possible attribute bits
	switch (attribute)
	{
		case WP5_ATTRIBUTE_SUBSCRIPT:
			textAttributeBit = WPX_SUBSCRIPT_BIT;
			break;
		case WP5_ATTRIBUTE_SUPERSCRIPT:
			textAttributeBit = WPX_SUPERSCRIPT_BIT;
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
	}
	
	if (isOn) 
		m_parseState->m_textAttributeBits |= textAttributeBit;
	else
		m_parseState->m_textAttributeBits ^= textAttributeBit;

	m_parseState->m_textAttributesChanged = true;
}

/****************************************
 private functions
*****************************************/
/*
void WP5HLListener::_openParagraph()
{
	_closeParagraph();
	guint8 paragraphJustification;
	(m_parseState->m_tempParagraphJustification != 0) ? paragraphJustification = m_parseState->m_tempParagraphJustification :
		paragraphJustification = m_parseState->m_paragraphJustification;
	m_parseState->m_tempParagraphJustification = 0;
	
	m_listenerImpl->openParagraph(paragraphJustification, m_parseState->m_textAttributeBits,
				      m_parseState->m_marginLeft, m_parseState->m_marginRight,
				      m_parseState->m_fontName->str, m_parseState->m_fontSize, 
				      m_parseState->m_paragraphLineSpacing, 
				      m_parseState->m_isParagraphColumnBreak, m_parseState->m_isParagraphPageBreak);
	if (m_parseState->m_numDeferredParagraphBreaks > 0) 
		m_parseState->m_numDeferredParagraphBreaks--;

	m_parseState->m_isParagraphColumnBreak = false; 
	m_parseState->m_isParagraphPageBreak = false;
	m_parseState->m_isParagraphOpened = true;
}

void WP5HLListener::_closeParagraph()
{
	_closeSpan();
	if (m_parseState->m_isParagraphOpened)
		m_listenerImpl->closeParagraph();	

	m_parseState->m_isParagraphOpened = false;
}
*/
void WP5HLListener::_openSpan()
{
	_closeSpan();
	m_listenerImpl->openSpan(m_parseState->m_textAttributeBits, "Times New Roman", 12.0f);
	m_parseState->m_isSpanOpened = true;
}

void WP5HLListener::_closeSpan()
{
	if (m_parseState->m_isSpanOpened)
		m_listenerImpl->closeSpan();

	m_parseState->m_isSpanOpened = false;
}

void WP5HLListener::_flushText()
{
	if (m_parseState->m_textAttributesChanged && m_textBuffer.getLen())
	{
		_openSpan();
		m_parseState->m_textAttributesChanged = false;
	}
	
	if (m_textBuffer.getLen())
	{
		m_listenerImpl->insertText(m_textBuffer);
		m_textBuffer.clear();
	}
}
