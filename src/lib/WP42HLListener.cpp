/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003-2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP42HLListener.h"
#include "WP42FileStructure.h"
#include "libwpd_internal.h"

WP42HLListener::WP42HLListener(std::vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl) :
	WPXHLListener(pageList, listenerImpl),
	WP42LLListener()
{
	m_textBuffer.clear();
}

/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP42HLListener::insertCharacter(const uint16_t character)
{
	if (m_ps->m_isSpanOpened)
		_openSpan();
	appendUCS4(m_textBuffer, (uint32_t)character);
}

void WP42HLListener::insertTab(const uint8_t tabType, const float tabPosition)
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

void WP42HLListener::insertEOL()
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

void WP42HLListener::endDocument()
{
	_closeSpan();
	_closeParagraph();
	_closeSection();
	_closePageSpan();
	m_listenerImpl->endDocument();
}


/****************************************
 public 'parser' functions
*****************************************/

void WP42HLListener::attributeChange(const bool isOn, const uint8_t attribute)
{
	_closeSpan();

	uint32_t textAttributeBit = 0;

	// FIXME: handle all the possible attribute bits
	switch (attribute)
	{
	/*case WP42_ATTRIBUTE_SUBSCRIPT:
		textAttributeBit = WPX_SUBSCRIPT_BIT;
		break;
	case WP42_ATTRIBUTE_SUPERSCRIPT:
		textAttributeBit = WPX_SUPERSCRIPT_BIT;
		break;
	case WP42_ATTRIBUTE_OUTLINE:
		textAttributeBit = WPX_OUTLINE_BIT;
		break;*/
	case WP42_ATTRIBUTE_ITALICS:
		textAttributeBit = WPX_ITALICS_BIT;
		break;
	case WP42_ATTRIBUTE_SHADOW:
		textAttributeBit = WPX_SHADOW_BIT;
		break;
	case WP42_ATTRIBUTE_REDLINE:
		textAttributeBit = WPX_REDLINE_BIT;
		break;
	/*case WP42_ATTRIBUTE_DOUBLE_UNDERLINE:
		textAttributeBit = WPX_DOUBLE_UNDERLINE_BIT;
		break;			*/
	case WP42_ATTRIBUTE_BOLD:
		textAttributeBit = WPX_BOLD_BIT;
		break;
	case WP42_ATTRIBUTE_STRIKE_OUT:
		textAttributeBit = WPX_STRIKEOUT_BIT;
		break;
	case WP42_ATTRIBUTE_UNDERLINE:
		textAttributeBit = WPX_UNDERLINE_BIT;
		break;
	}

	if (isOn)
		m_ps->m_textAttributeBits |= textAttributeBit;
	else
		m_ps->m_textAttributeBits ^= textAttributeBit;
}

/****************************************
 private functions
*****************************************/

void WP42HLListener::_flushText()
{
	if (m_textBuffer.len())
		m_listenerImpl->insertText(m_textBuffer);
	m_textBuffer.clear();
}
