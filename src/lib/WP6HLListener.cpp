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

WP6HLListener::WP6HLListener(WPXHLListenerImpl *listenerImpl) :
	WP6LLListener(), 
	m_listenerImpl(listenerImpl),
	m_textArray(g_array_new(TRUE, FALSE, sizeof(guint16))),
	m_isParagraphOpened(FALSE),
	m_isParagraphClosed(FALSE)
{
}

WP6HLListener::~WP6HLListener()
{
	g_array_free(m_textArray, TRUE);
}

void WP6HLListener::insertCharacter(guint16 character)
{
	g_array_append_val(m_textArray, character);
}

void WP6HLListener::insertEOL()
{
	if (!m_isParagraphOpened)
		m_listenerImpl->openParagraph();
	else if (m_isParagraphOpened && !m_isParagraphClosed) {
		m_listenerImpl->closeParagraph();
		m_listenerImpl->openParagraph();
	}

	_flushText();       

	m_listenerImpl->closeParagraph();

	m_isParagraphOpened = FALSE;
	m_isParagraphClosed = TRUE;
}

void WP6HLListener::startDocument()
{
	m_listenerImpl->startDocument();
}

void WP6HLListener::endDocument()
{
	// corner case: document contains no end of lines
	if (!m_isParagraphOpened && !m_isParagraphClosed) {
		m_listenerImpl->openParagraph();
		_flushText();       
		m_listenerImpl->closeParagraph();
	}
	else if (!m_isParagraphClosed) {
		_flushText();
		m_listenerImpl->closeParagraph();
	}
	else if (!m_isParagraphOpened && m_textArray->len > 0) {
		m_listenerImpl->openParagraph();
		_flushText();
		m_listenerImpl->closeParagraph();
	}
	
	// the only other possibility is a logical contradiction: a paragraph
	// may not be opened and closed at the same time

	m_listenerImpl->endDocument();
}

void WP6HLListener::_flushText()
{
	m_listenerImpl->insertText((guint16 *)m_textArray->data, m_textArray->len);
	m_textArray = g_array_set_size(m_textArray, 0);
}
