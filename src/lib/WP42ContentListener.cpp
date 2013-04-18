/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP42ContentListener.h"
#include "WP42FileStructure.h"
#include "libwpd_internal.h"

#define WP42_NUM_TEXT_COLUMS_PER_INCH 12

_WP42ContentParsingState::_WP42ContentParsingState() :
	m_textBuffer()
{
}

_WP42ContentParsingState::~_WP42ContentParsingState()
{
}


WP42ContentListener::WP42ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP42SubDocument *> &subDocuments, WPXDocumentInterface *documentInterface) :
	WP42Listener(),
	WPXContentListener(pageList, documentInterface),
	m_parseState(new WP42ContentParsingState),
	m_subDocuments(subDocuments)
{
// Default line is 6 lpi, it means that the caracters are of 12 points
	*(m_ps->m_fontName) = "Courier";
	m_ps->m_fontSize = 12.0;
}

WP42ContentListener::~WP42ContentListener()
{
	delete m_parseState;
}


void WP42ContentListener::insertCharacter(uint32_t character)
{
	if (!isUndoOn())
	{
		uint32_t tmpCharacter = _mapNonUnicodeCharacter(character);
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		appendUCS4(m_parseState->m_textBuffer, tmpCharacter);
	}
}

void WP42ContentListener::insertTab(uint8_t /* tabType */, double /* tabPosition */)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		else
			_flushText();

		m_documentInterface->insertTab();
	}
}

void WP42ContentListener::insertEOL()
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

void WP42ContentListener::attributeChange(bool isOn, uint8_t attribute)
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
	default:
		break;
	}

	if (isOn)
		m_ps->m_textAttributeBits |= textAttributeBit;
	else
		m_ps->m_textAttributeBits &= ~textAttributeBit;
}

void WP42ContentListener::marginReset(uint8_t /* leftMargin */, uint8_t /* rightMargin */)
{
#if 0
	if (!isUndoOn())
	{
		double leftMarginInch = (double)(leftMargin/WP42_NUM_TEXT_COLUMS_PER_INCH);
		double rightMarginInch = m_ps->m_pageFormWidth - (double)((rightMargin + 1)/WP42_NUM_TEXT_COLUMS_PER_INCH);
		m_ps->m_leftMarginByPageMarginChange = leftMarginInch - m_ps->m_pageMarginLeft;
		m_ps->m_rightMarginByPageMarginChange = rightMarginInch - m_ps->m_pageMarginRight;
	}
#endif
}

void WP42ContentListener::headerFooterGroup(uint8_t /* headerFooterDefinition */, WP42SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);
}

void WP42ContentListener::_handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType /* subDocumentType */,
        WPXTableList /* tableList */, unsigned /* nextTableIndice */)
{
	// save our old parsing state on our "stack"
	WP42ContentParsingState *oldParseState = m_parseState;

	m_parseState = new WP42ContentParsingState();

	if (subDocument)
	{
		static_cast<const WP42SubDocument *>(subDocument)->parse(this);
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

void WP42ContentListener::_flushText()
{
	_insertText(m_parseState->m_textBuffer);
	m_parseState->m_textBuffer.clear();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
