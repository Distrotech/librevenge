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

#include "WP1ContentListener.h"
#include "WP1FileStructure.h"
#include "libwpd_internal.h"
#include "WP1SubDocument.h"

// #define WP1_NUM_TEXT_COLUMS_PER_INCH 12

_WP1ContentParsingState::_WP1ContentParsingState() :
	m_textBuffer(),
	m_numDeferredTabs(0),
	m_footNoteNumber(0),
	m_endNoteNumber(0)
{
}

_WP1ContentParsingState::~_WP1ContentParsingState()
{
}


WP1ContentListener::WP1ContentListener(std::list<WPXPageSpan> &pageList, std::vector<WP1SubDocument *> &subDocuments, WPXDocumentInterface *documentInterface) :
	WP1Listener(),
	WPXContentListener(pageList, documentInterface),
	m_parseState(new WP1ContentParsingState),
	m_subDocuments(subDocuments)
{
	*(m_ps->m_fontName) = "Geneva";
	m_ps->m_fontSize = 12.0;
}

WP1ContentListener::~WP1ContentListener()
{
	delete m_parseState;
}


void WP1ContentListener::insertCharacter(uint32_t character)
{
	if (!isUndoOn())
	{
		uint32_t tmpCharacter = _mapNonUnicodeCharacter(character);

		if (!m_ps->m_isSpanOpened)
			_openSpan();
		for (; m_parseState->m_numDeferredTabs > 0; m_parseState->m_numDeferredTabs--)
		{
			m_documentInterface->insertTab();
		}
		appendUCS4(m_parseState->m_textBuffer, tmpCharacter);
	}
}

void WP1ContentListener::insertExtendedCharacter(uint8_t extendedCharacter)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();
		for (; m_parseState->m_numDeferredTabs > 0; m_parseState->m_numDeferredTabs--)
		{
			m_documentInterface->insertTab();
		}
		if (extendedCharacter <= 0x20)
			appendUCS4(m_parseState->m_textBuffer, (uint32_t)0x20);
		else
			appendUCS4(m_parseState->m_textBuffer, (uint32_t)(_mapNonUnicodeCharacter(macRomanCharacterMap[extendedCharacter - 0x20])));
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

			m_documentInterface->insertTab();
		}
	}
}

void WP1ContentListener::insertEOL()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
			_openSpan();
		for (; m_parseState->m_numDeferredTabs > 0; m_parseState->m_numDeferredTabs--)
		{
			m_documentInterface->insertTab();
		}

		if (m_ps->m_isParagraphOpened)
			_closeParagraph();
		if (m_ps->m_isListElementOpened)
			_closeListElement();
	}
}

void WP1ContentListener::insertNote(const WPXNoteType noteType, WP1SubDocument *subDocument)
{
	if (!isUndoOn() && !m_ps->m_isNote)
	{
		if (!m_ps->m_isParagraphOpened)
			_openParagraph();
		else
		{
			_flushText();
			_closeSpan();
		}

		m_ps->m_isNote = true;

		WPXPropertyList propList;

		if (noteType == FOOTNOTE)
		{
			propList.insert("libwpd:number", ++(m_parseState->m_footNoteNumber));
			m_documentInterface->openFootnote(propList);
		}
		else
		{
			propList.insert("libwpd:number", ++(m_parseState->m_endNoteNumber));
			m_documentInterface->openEndnote(propList);
		}

		WPXTableList tableList;
		handleSubDocument(subDocument, WPX_SUBDOCUMENT_NOTE, tableList, 0);

		if (noteType == FOOTNOTE)
			m_documentInterface->closeFootnote();
		else
			m_documentInterface->closeEndnote();
		m_ps->m_isNote = false;
	}
}


void WP1ContentListener::attributeChange(bool isOn, uint8_t attribute)
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
	default:
		break;
	}

	if (isOn)
		m_ps->m_textAttributeBits |= textAttributeBit;
	else
		m_ps->m_textAttributeBits &= ~textAttributeBit;
}

void WP1ContentListener::fontPointSize(uint8_t pointSize)
{
	if (!isUndoOn())
	{
		_closeSpan();

		m_ps->m_fontSize=double(pointSize);
	}
}

void WP1ContentListener::fontId(uint16_t id)
{
	if (!isUndoOn())
	{
		_closeSpan();

		switch (id)
		{
		case 0x0002:
		case 0xFF02:
			*m_ps->m_fontName = "New York";
			break;
		case 0x0003:
		case 0xFF03:
			*m_ps->m_fontName = "Geneva";
			break;
		case 0x0004:
		case 0xFF04:
			*m_ps->m_fontName = "Monaco";
			break;
		case 0x0005:
		case 0xFF05:
			*m_ps->m_fontName = "Venice";
			break;
		case 0x0006:
		case 0xFF06:
			*m_ps->m_fontName = "London";
			break;
		case 0x0007:
		case 0xFF07:
			*m_ps->m_fontName = "Athens";
			break;
		case 0x0008:
		case 0xFF08:
			*m_ps->m_fontName = "San Francisco";
			break;
		case 0x0009:
		case 0xFF09:
			*m_ps->m_fontName = "Toronto";
			break;
		case 0x000B:
		case 0xFF0B:
			*m_ps->m_fontName = "Cairo";
			break;
		case 0x000C:
		case 0xFF0C:
			*m_ps->m_fontName = "Los Angeles";
			break;
		case 0x000D:
		case 0xFF0D:
			*m_ps->m_fontName = "Zapf Dingbats";
			break;
		case 0x0010:
		case 0xFF10:
			*m_ps->m_fontName = "Palatino";
			break;
		case 0x0014:
		case 0xFF14:
			*m_ps->m_fontName = "Times";
			break;
		case 0x0015:
		case 0xFF15:
			*m_ps->m_fontName = "Helvetica";
			break;
		case 0x0016:
		case 0xFF16:
			*m_ps->m_fontName = "Courier";
			break;
		case 0x0017:
		case 0xFF17:
			*m_ps->m_fontName = "Symbol";
			break;
		case 0x0018:
		case 0xFF18:
			*m_ps->m_fontName = "Mobile";
			break;
		case 0x07D2:
			*m_ps->m_fontName = "Charcoal";
			break;
		case 0x0B07:
			*m_ps->m_fontName = "EngraversGothic BT Regular";
			break;
		case 0x0B48:
			*m_ps->m_fontName = "Swiss721 BlkEx BT Black";
			break;
		case 0x0DB0:
			*m_ps->m_fontName = "GeoSla703 Lt BT Light Italic";
			break;
		case 0x0DBF:
			*m_ps->m_fontName = "Humanst521 Lt BT Light Italic";
			break;
		case 0x0DC0:
			*m_ps->m_fontName = "Humanst521 Cn BT Bold";
			break;
		case 0x0EC8:
			*m_ps->m_fontName = "Arrus BT Bold";
			break;
		case 0x0EC9:
			*m_ps->m_fontName = "Arrus BT Bold Italic";
			break;
		case 0x0ECA:
			*m_ps->m_fontName = "Arrus Blk BT Black";
			break;
		case 0x0ECB:
			*m_ps->m_fontName = "Arrus Blk BT Black Italic";
			break;
		case 0x0ECC:
			*m_ps->m_fontName = "Arrus BT Italic";
			break;
		case 0x0ECD:
			*m_ps->m_fontName = "Arrus BT Roman";
			break;
		case 0x0F7C:
			*m_ps->m_fontName = "OzHandicraft BT Roman";
			break;
		case 0x12ED:
			*m_ps->m_fontName = "BernhardMod BT Italic";
			break;
		case 0x12EE:
			*m_ps->m_fontName = "BernhardMod BT Bold";
			break;
		case 0x12EF:
			*m_ps->m_fontName = "BernhardMod BT Bold Italic";
			break;
		case 0x3275:
			*m_ps->m_fontName = "Ribbon131 Bd BT Bold";
			break;
		case 0x3B98:
			*m_ps->m_fontName = "Blackletter686 BT Regular";
			break;
		case 0x3BA2:
			*m_ps->m_fontName = "Brush738 BT Regular";
			break;
		case 0x3BCF:
			*m_ps->m_fontName = "BernhardMod BT Roman";
			break;
		case 0x3BF4:
			*m_ps->m_fontName = "CaslonOpnface BT Regular";
			break;
		case 0x3C86:
			*m_ps->m_fontName = "Onyx BT Regular";
			break;
		case 0x3C3F:
			*m_ps->m_fontName = "GeoSla703 Lt BT Light";
			break;
		case 0x3C4A:
			*m_ps->m_fontName = "Humanst521 Lt BT Light";
			break;
		case 0x3C4C:
			*m_ps->m_fontName = "Humanst521 Cn BT Regular";
			break;
		case 0x3FFF:
			*m_ps->m_fontName = "Chicago";
			break;
		default:
			*m_ps->m_fontName = "Geneva";
			break;
		}
	}
}

void WP1ContentListener::marginReset(uint16_t leftMargin, uint16_t rightMargin)
{
	if (!isUndoOn())
	{
		if (leftMargin)
		{
			double leftMarginInch = (double)((double)leftMargin/72.0);
			m_ps->m_leftMarginByPageMarginChange = leftMarginInch - m_ps->m_pageMarginLeft;
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
			                              + m_ps->m_leftMarginByParagraphMarginChange
			                              + m_ps->m_leftMarginByTabs;

		}
		if (rightMargin)
		{
			double rightMarginInch = (double)((double)rightMargin/72.0);
			m_ps->m_rightMarginByPageMarginChange = rightMarginInch - m_ps->m_pageMarginRight;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
			                               + m_ps->m_rightMarginByParagraphMarginChange
			                               + m_ps->m_rightMarginByTabs;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP1ContentListener::leftIndent(uint16_t leftMarginOffset)
{
	if (!isUndoOn())
	{
		if (m_ps->m_isParagraphOpened)
			insertTab();
		else
		{
			m_parseState->m_numDeferredTabs = 0;
			m_ps->m_leftMarginByTabs += (double)((double)leftMarginOffset / 72.0);
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
			                              + m_ps->m_leftMarginByParagraphMarginChange
			                              + m_ps->m_leftMarginByTabs;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP1ContentListener::leftRightIndent(uint16_t leftRightMarginOffset)
{
	if (!isUndoOn())
	{
		if (m_ps->m_isParagraphOpened)
			insertTab();
		else
		{
			m_parseState->m_numDeferredTabs = 0;
			m_ps->m_leftMarginByTabs += (double)((double)leftRightMarginOffset / 72.0);
			m_ps->m_rightMarginByTabs += (double)((double)leftRightMarginOffset / 72.0);
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
			                              + m_ps->m_leftMarginByParagraphMarginChange
			                              + m_ps->m_leftMarginByTabs;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
			                               + m_ps->m_rightMarginByParagraphMarginChange
			                               + m_ps->m_rightMarginByTabs;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP1ContentListener::leftMarginRelease(uint16_t release)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened)
		{
			if (!m_parseState->m_numDeferredTabs)
			{
				m_ps->m_textIndentByTabs -= (double)((double)release / 72.0);
				m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
				                              + m_ps->m_textIndentByTabs;
			}
			else
				m_parseState->m_numDeferredTabs--;
		}
		m_ps->m_listReferencePosition = m_ps->m_paragraphMarginLeft + m_ps->m_paragraphTextIndent;
	}
}

void WP1ContentListener::justificationChange(uint8_t justification)
{
	if (!isUndoOn())
	{
		switch (justification)
		{
		case 0x01:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
			break;
		case 0x02:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
			break;
		case 0x03:
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_FULL;
			break;
		default: // case 0x00 and all other invalid stuff
			m_ps->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_LEFT;
			break;
		}
	}
}

void WP1ContentListener::headerFooterGroup(uint8_t /* headerFooterDefinition */, WP1SubDocument *subDocument)
{
	if (subDocument)
		m_subDocuments.push_back(subDocument);
}

void WP1ContentListener::setTabs(const std::vector<WPXTabStop> &tabStops)
{
	if (!isUndoOn())
	{
		m_ps->m_isTabPositionRelative = false;
		m_ps->m_tabStops = tabStops;
	}
}

void WP1ContentListener::flushRightOn()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			m_parseState->m_numDeferredTabs = 0;
			m_ps->m_tempParagraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
		}
		else
			insertTab();
	}
}

void WP1ContentListener::centerOn()
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			m_parseState->m_numDeferredTabs = 0;
			m_ps->m_tempParagraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
		}
		else
			insertTab();
	}
}

void WP1ContentListener::insertPicture(uint16_t width, uint16_t height, const WPXBinaryData &binaryData)
{
	if (!isUndoOn())
	{
		if (!m_ps->m_isSpanOpened)
			_openSpan();

		WPXPropertyList propList;
		propList.insert("svg:width", (double)((double)width/72.0));
		propList.insert("svg:height", (double)((double)height/72.0));
		propList.insert("text:anchor-type", "as-char");
		m_documentInterface->openFrame(propList);

		propList.clear();
		propList.insert("libwpd:mimetype", "image/pict");
		m_documentInterface->insertBinaryObject(propList, binaryData);

		m_documentInterface->closeFrame();
	}
}

void WP1ContentListener::_handleSubDocument(const WPXSubDocument *subDocument, WPXSubDocumentType /* subDocumentType */,
        WPXTableList /* tableList */, unsigned /* nextTableIndice */)
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
	_insertText(m_parseState->m_textBuffer);
	m_parseState->m_textBuffer.clear();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
