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

#include "WP42HLListener.h"
#include "WP42FileStructure.h"

WP42HLListener::WP42HLListener(WPXHLListenerImpl *listenerImpl)
	: m_listenerImpl(listenerImpl),
	m_textAttributesChanged(false),
	m_textAttributeBits(0)
{
	m_textBuffer.clear();
}

/****************************************
 public 'HLListenerImpl' functions
*****************************************/

void WP42HLListener::startDocument()
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

void WP42HLListener::insertCharacter(const guint16 character)
{
	m_textBuffer.append(character);
}

void WP42HLListener::insertTab(const guint8 tabType)
{
	_flushText();
	m_listenerImpl->insertTab();
}

void WP42HLListener::insertEOL()
{
	_flushText();
	m_listenerImpl->insertLineBreak();
	m_listenerImpl->openParagraph(0, 0, 0, 0,
					"Times New Roman", 12.0f,
					1.0f,
					false, false);
	_openSpan();	
}

void WP42HLListener::endDocument()
{
	_flushText();
	m_listenerImpl->endDocument();	
}


/****************************************
 public 'parser' functions
*****************************************/

void WP42HLListener::attributeChange(const bool isOn, const guint8 attribute)
{
	
	// flush everything which came before this change
	_flushText();
	
	guint32 textAttributeBit = 0;
	
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
		m_textAttributeBits |= textAttributeBit;
	else
		m_textAttributeBits ^= textAttributeBit;

	m_textAttributesChanged = true;
}

/****************************************
 private functions
*****************************************/

void WP42HLListener::_openSpan()
{
	m_listenerImpl->openSpan(m_textAttributeBits, "Times New Roman", 12.0f);
}

void WP42HLListener::_flushText()
{
	if (m_textAttributesChanged && m_textBuffer.getLen())
	{
		_openSpan();
		m_textAttributesChanged = false;
	}
	
	if (m_textBuffer.getLen())
	{
		m_listenerImpl->insertText(m_textBuffer);
		m_textBuffer.clear();
	}
}
