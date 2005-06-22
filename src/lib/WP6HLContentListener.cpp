/* libwpd
 * Copyright (C) 2002-2005 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004-2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include <ctype.h>
#include "libwpd_math.h"
#include "WP6HLContentListener.h"
#include "WPXHLListenerImpl.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"
#include "libwpd_internal.h"

#include "WP6PrefixData.h"
#include "WPXTable.h"

#include <assert.h>

#define WP6_DEFAULT_FONT_SIZE 12.0f
#define WP6_DEFAULT_FONT_NAME "Times New Roman"

// HACK: this function is really cheesey
int _extractNumericValueFromRoman(const char romanChar)
{
	switch (romanChar)
	{
	case 'I':
	case 'i':
		return 1;
	case 'V':
	case 'v':
		return 5;
	case 'X':
	case 'x':
		return 10;
	default:
		throw ParseException();
	}
	return 1;
}

// _extractDisplayReferenceNumberFromBuf: given a nuWP6_DEFAULT_FONT_SIZEmber string in UCS2 represented
// as letters, numbers, or roman numerals.. return an integer value representing its number
// HACK: this function is really cheesey
// NOTE: if the input is not valid, the output is unspecified
int _extractDisplayReferenceNumberFromBuf(const WPXString &buf, const WPXNumberingType listType)
{
	if (listType == LOWERCASE_ROMAN || listType == UPPERCASE_ROMAN)
	{
		int currentSum = 0;
		int lastMark = 0;
		int currentMark = 0;
		WPXString::Iter i(buf);
		for (i.rewind(); i.next();)
		{
			int currentMark = _extractNumericValueFromRoman(*(i()));
			if (lastMark < currentMark) {
				currentSum = currentMark - lastMark;
			}
			else
				currentSum+=currentMark;
			lastMark = currentMark;
		}
		return currentSum;
	}
	else if (listType == LOWERCASE || listType == UPPERCASE)
	{
		// FIXME: what happens to a lettered list that goes past z? ah
		// the sweet mysteries of life
		if (buf.len()==0)
			throw ParseException();
		char c = buf.cstr()[0];
		if (listType==LOWERCASE)
			c = toupper(c);
		return (c - 64);
	}
	else if (listType == ARABIC)
	{
		int currentSum = 0;
		WPXString::Iter i(buf);
		for (i.rewind(); i.next();)
		{
			currentSum *= 10;
			currentSum+=(*(i())-48);
		}
		return currentSum;
	}

	return 1;
}

WPXNumberingType _extractWPXNumberingTypeFromBuf(const WPXString &buf, const WPXNumberingType putativeWPXNumberingType)
{
	WPXString::Iter i(buf);
	for (i.rewind(); i.next();)
	{
		if ((*(i()) == 'I' || *(i()) == 'V' || *(i()) == 'X') &&
		    (putativeWPXNumberingType == LOWERCASE_ROMAN || putativeWPXNumberingType == UPPERCASE_ROMAN))
			return UPPERCASE_ROMAN;
		else if ((*(i()) == 'i' || *(i()) == 'v' || *(i()) == 'x') &&
		    (putativeWPXNumberingType == LOWERCASE_ROMAN || putativeWPXNumberingType == UPPERCASE_ROMAN))
			return LOWERCASE_ROMAN;
		else if (*(i()) >= 'A' && *(i()) <= 'Z')
			return UPPERCASE;
		else if (*(i()) >= 'a' && *(i()) <= 'z')
			return LOWERCASE;
	}

	return ARABIC;
}

WPXString _numberingTypeToString(WPXNumberingType t)
{
	WPXString sListTypeSymbol("1");
	switch (t)
	{
	case ARABIC:
		sListTypeSymbol.sprintf("1");
		break;	
	case LOWERCASE:
		sListTypeSymbol.sprintf("a");
		break;	
	case UPPERCASE:
		sListTypeSymbol.sprintf("A");
		break;	
 	case LOWERCASE_ROMAN:
		sListTypeSymbol.sprintf("i");
		break;	
 	case UPPERCASE_ROMAN:
		sListTypeSymbol.sprintf("I");
		break;
	}

	return sListTypeSymbol;
}



WP6OutlineDefinition::WP6OutlineDefinition(const WP6OutlineLocation outlineLocation, const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag)
{
	_updateNumberingMethods(outlineLocation, numberingMethods);
}

WP6OutlineDefinition::WP6OutlineDefinition()
{
	uint8_t numberingMethods[WP6_NUM_LIST_LEVELS];
	for (int i=0; i<WP6_NUM_LIST_LEVELS; i++)
		numberingMethods[i] = WP6_INDEX_HEADER_OUTLINE_STYLE_ARABIC_NUMBERING;

	_updateNumberingMethods(paragraphGroup, numberingMethods);
}

// update: updates a partially made list definition (usual case where this is used: an
// outline style is defined in a prefix packet, then you are given more information later
// in the document)
// FIXME: make sure this is in the right place
void WP6OutlineDefinition::update(const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag)
{
	_updateNumberingMethods(paragraphGroup, numberingMethods);
}

void WP6OutlineDefinition::_updateNumberingMethods(const WP6OutlineLocation outlineLocation, const uint8_t *numberingMethods)
{
	for (int i=0; i<WP6_NUM_LIST_LEVELS; i++)
	{
		switch (numberingMethods[i])
		{
		case WP6_INDEX_HEADER_OUTLINE_STYLE_ARABIC_NUMBERING:
			m_listTypes[i] = ARABIC;
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_LOWERCASE_NUMBERING:
			m_listTypes[i] = LOWERCASE;
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_UPPERCASE_NUMBERING:
			m_listTypes[i] = UPPERCASE;
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_LOWERCASE_ROMAN_NUMBERING:
			m_listTypes[i] = LOWERCASE_ROMAN;
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_UPPERCASE_ROMAN_NUMBERING:
			m_listTypes[i] = UPPERCASE_ROMAN;
			break;
			//case WP6_INDEX_HEADER_OUTLINE_STYLE_LEADING_ZERO_ARABIC_NUMBERING:
			//break;
		default:
			m_listTypes[i] = ARABIC;
		}
	}
	WPD_DEBUG_MSG(("WordPerfect: Updated List Types: (%i %i %i %i %i %i %i %i)\n",
		       m_listTypes[0], m_listTypes[1], m_listTypes[2], m_listTypes[3],
		       m_listTypes[4], m_listTypes[5], m_listTypes[6], m_listTypes[7]));

}

_WP6ParsingState::_WP6ParsingState(WPXTableList tableList, int nextTableIndice) :
	m_paragraphMarginBottomAbsolute(0.0f),
	m_paragraphMarginBottomRelative(1.0f),

	m_numRemovedParagraphBreaks(0),

	m_tableList(tableList),
	m_currentTable(NULL),
	m_nextTableIndice(nextTableIndice),

	m_currentListLevel(0),
	m_putativeListElementHasParagraphNumber(false),
	m_putativeListElementHasDisplayReferenceNumber(false),
	
	m_noteTextPID(0),

	m_leaderCharacter('.'),
	m_leaderNumSpaces(0)

{
}

_WP6ParsingState::~_WP6ParsingState()
{
	// FIXME: erase current fontname
}

WP6HLContentListener::WP6HLContentListener(std::vector<WPXPageSpan *> *pageList, WPXTableList tableList, WPXHLListenerImpl *listenerImpl) :
	WP6HLListener(pageList, listenerImpl),
	m_parseState(new WP6ParsingState(tableList))
{
}

WP6HLContentListener::~WP6HLContentListener()
{
	typedef std::map<int, WP6OutlineDefinition *>::iterator Iter;
	for (Iter outline = m_outlineDefineHash.begin(); outline != m_outlineDefineHash.end(); outline++)
	{
		delete(outline->second);
	}
	delete m_parseState;
}

void WP6HLContentListener::setExtendedInformation(const uint16_t type, const WPXString &data)
{
	switch (type)
	{
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_AUTHOR):
			m_metaData.insert("dc:creator", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_SUBJECT):
			m_metaData.insert("dc:subject", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_PUBLISHER):
			m_metaData.insert("dc:publisher", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_CATEGORY):
			m_metaData.insert("dc:type", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_KEYWORDS):
			m_metaData.insert("libwpd:keywords", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_LANGUAGE):
			m_metaData.insert("dc:language", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_ABSTRACT):
			m_metaData.insert("libwpd:abstract", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DESCRIPTIVE_NAME):
			m_metaData.insert("libwpd:descriptive-name", data);
			break;
		case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DESCRIPTIVE_TYPE):
			m_metaData.insert("libwpd:descriptive-type", data);
			break;
	}
}

void WP6HLContentListener::setAlignmentCharacter(const uint16_t character)
{
	if (!isUndoOn())
	{
		m_ps->m_alignmentCharacter = character;
	}
}

void WP6HLContentListener::setLeaderCharacter(const uint16_t character, const uint8_t numSpaces)
{
	assert(m_ps->m_tabStops.size() == m_parseState->m_tempUsePreWP9LeaderMethod.size());

	if (!isUndoOn())
	{
		m_parseState->m_leaderCharacter = character;
		m_parseState->m_leaderNumSpaces = numSpaces;
		for (int i=0; i<m_ps->m_tabStops.size(); i++)
		{
			// change the leader information for those tab stops that use pre-WP9 leader method
			if (m_parseState->m_tempUsePreWP9LeaderMethod[i])
			{
				m_ps->m_tabStops[i].m_leaderCharacter = m_parseState->m_leaderCharacter;
				m_ps->m_tabStops[i].m_leaderNumSpaces = m_parseState->m_leaderNumSpaces;
			}
		}
	}
}

void WP6HLContentListener::insertCharacter(const uint16_t character)
{
	if (!isUndoOn())
	{
		if (m_parseState->m_styleStateSequence.getCurrentState() == STYLE_BODY ||
		    m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
			appendUCS4(m_parseState->m_bodyText, (uint32_t)character);
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_BEFORE_NUMBERING)
		{
			appendUCS4(m_parseState->m_textBeforeNumber, (uint32_t)character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING)
		{
			// left delimeter (or the bullet if there is no display referencing)
			appendUCS4(m_parseState->m_textBeforeDisplayReference, (uint32_t)character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == DISPLAY_REFERENCING)
		{
			// the actual paragraph number (in varying forms)
			appendUCS4(m_parseState->m_numberText, (uint32_t)character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_NUMBERING_AFTER_DISPLAY_REFERENCING)
		{
			// right delimeter (if there was a display no. ref. group)
			appendUCS4(m_parseState->m_textAfterDisplayReference, (uint32_t)character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_AFTER_NUMBERING)
		{
			appendUCS4(m_parseState->m_textAfterNumber, (uint32_t)character);
		}
	}
}

void WP6HLContentListener::defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops, 
					  const std::vector<bool> &usePreWP9LeaderMethods)
{
	if (!isUndoOn())
	{
		// define whether the position of tabs is relative to the left paragraph margin
		// or to the left edge of the sheet
		m_ps->m_isTabPositionRelative = isRelative;
		// define the tab stops
		m_parseState->m_tempTabStops = m_ps->m_tabStops = tabStops;
		m_parseState->m_tempUsePreWP9LeaderMethod = usePreWP9LeaderMethods;
		// take care of the pre-WP9 leader method
		setLeaderCharacter(m_parseState->m_leaderCharacter, m_parseState->m_leaderNumSpaces);
	}
}


void WP6HLContentListener::insertTab(const uint8_t tabType, const float tabPosition)
{
	if (!isUndoOn())
	{

		_flushText();
		// Fridrich Strba -- HACK: we force each tab to be in its own span in order
		// to prevent previous span's properties to apply to the tab also if they changed.
		m_ps->m_textAttributesChanged = true;

		// open new section if section attributes changed
		if (m_ps->m_sectionAttributesChanged && !m_ps->m_isTableOpened)
		{
			_openSection();
			m_ps->m_sectionAttributesChanged = false;
		}

		if (m_parseState->m_styleStateSequence.getCurrentState() == STYLE_BODY ||
		    m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
		{

			// First of all, open paragraph for tabs that always are converted as tabs
			switch ((tabType & 0xF8) >> 3)
			{
			case WP6_TAB_GROUP_TABLE_TAB:
			// case WP6_TAB_GROUP_LEFT_TAB:
			case WP6_TAB_GROUP_BAR_TAB:
			// Uncomment when the TabGroup is properly implemented
			//case WP6_TAB_GROUP_CENTER_ON_MARGINS:
			//case WP6_TAB_GROUP_CENTER_ON_CURRENT_POSITION:
			case WP6_TAB_GROUP_CENTER_TAB:
			//case WP6_TAB_GROUP_FLUSH_RIGHT:
			case WP6_TAB_GROUP_RIGHT_TAB:
			case WP6_TAB_GROUP_DECIMAL_TAB:
				if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
					if (m_parseState->m_currentListLevel == 0)
						_openParagraph();
					else
						_openListElement();
					_openSpan();
				break;
			
			default:
				break;
			}
			
			// Following tabs are converted as formating if the paragraph is not opened
			if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
			{
				switch ((tabType & 0xF8) >> 3)
				{
				// Begin of code to be removed when the TabGroup is properly implemented
				case WP6_TAB_GROUP_CENTER_ON_MARGINS:
				case WP6_TAB_GROUP_CENTER_ON_CURRENT_POSITION:
					m_ps->m_tempParagraphJustification = WP6_PARAGRAPH_JUSTIFICATION_CENTER;
					break;

				case WP6_TAB_GROUP_FLUSH_RIGHT:
					m_ps->m_tempParagraphJustification = WP6_PARAGRAPH_JUSTIFICATION_RIGHT;
					break;
				// End of code to be removed when the TabGroup is properly implemented

				case WP6_TAB_GROUP_LEFT_TAB: // converted as first line indent
					if (tabPosition >= (float)((double)0xFFFE/(double)WPX_NUM_WPUS_PER_INCH))
						// fall-back solution if we are not able to read the tabPosition
						m_ps->m_textIndentByTabs += 0.5f;
					else
						m_ps->m_textIndentByTabs = tabPosition - m_ps->m_paragraphMarginLeft
							- m_ps->m_pageMarginLeft - m_ps->m_textIndentByParagraphIndentChange;
					break;

				case WP6_TAB_GROUP_BACK_TAB: // converted as hanging indent
					if (tabPosition >= (float)((double)0xFFFE/(double)WPX_NUM_WPUS_PER_INCH))
						// fall-back solution if we are not able to read the tabPosition
						m_ps->m_textIndentByTabs -= 0.5f;
					else
						m_ps->m_textIndentByTabs = tabPosition - m_ps->m_paragraphMarginLeft
							- m_ps->m_pageMarginLeft - m_ps->m_textIndentByParagraphIndentChange;
					break;

				case WP6_TAB_GROUP_LEFT_INDENT:  // converted as left paragraph margin offset
					if (tabPosition >= (float)((double)0xFFFE/(double)WPX_NUM_WPUS_PER_INCH))
						// fall-back solution if we are not able to read the tabPosition
						m_ps->m_leftMarginByTabs += 0.5f;
					else
						m_ps->m_leftMarginByTabs = tabPosition - m_ps->m_pageMarginLeft
							- m_ps->m_leftMarginByPageMarginChange - m_ps->m_leftMarginByParagraphMarginChange;
					if (m_ps->m_paragraphTextIndent != 0.0f)
						m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;
					break;

				case WP6_TAB_GROUP_LEFT_RIGHT_INDENT: // converted as left and right paragraph margin offset
					if (tabPosition >= (float)((double)0xFFFE/(double)WPX_NUM_WPUS_PER_INCH))
						// fall-back solution if we are not able to read the tabPosition
						m_ps->m_leftMarginByTabs += 0.5f;
					else
						m_ps->m_leftMarginByTabs = tabPosition - m_ps->m_pageMarginLeft
							- m_ps->m_leftMarginByPageMarginChange - m_ps->m_leftMarginByParagraphMarginChange;
					// L/R Indent is symetrical from the effective paragraph margins and position indicates only
					// the distance from the left edge
					m_ps->m_rightMarginByTabs = m_ps->m_leftMarginByTabs;
					if (m_ps->m_paragraphTextIndent != 0.0f)
						m_ps->m_textIndentByTabs -= m_ps->m_paragraphTextIndent;
					break;	
				
				default:
					break;
				}
				m_ps->m_paragraphTextIndent = m_ps->m_textIndentByParagraphIndentChange
					+ m_ps->m_textIndentByTabs;
				m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
					+ m_ps->m_leftMarginByParagraphMarginChange + m_ps->m_leftMarginByTabs;
				m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
					+ m_ps->m_rightMarginByParagraphMarginChange + m_ps->m_rightMarginByTabs;

			}
			else
			{
				// Fridrich Strba -- HACK: we force each tab to be in its own span in order
				// to prevent previous span's properties to apply to the tab also if they changed.
				_flushText(true);
				switch ((tabType & 0xF8) >> 3)
				{
				case WP6_TAB_GROUP_TABLE_TAB:
				case WP6_TAB_GROUP_LEFT_TAB:
				case WP6_TAB_GROUP_LEFT_INDENT:
				case WP6_TAB_GROUP_LEFT_RIGHT_INDENT:
				case WP6_TAB_GROUP_CENTER_ON_MARGINS:
				case WP6_TAB_GROUP_CENTER_ON_CURRENT_POSITION:
				case WP6_TAB_GROUP_CENTER_TAB:
				case WP6_TAB_GROUP_FLUSH_RIGHT:
				case WP6_TAB_GROUP_RIGHT_TAB:
				case WP6_TAB_GROUP_DECIMAL_TAB:
					m_listenerImpl->insertTab();
					break;
				case WP6_TAB_GROUP_BAR_TAB:
					m_listenerImpl->insertTab();
					insertCharacter('|');  // We emulate the bar tab
					break;
				
				default:
					break;
				}
				_flushText(true);
				m_ps->m_textAttributesChanged = true;
			}
		}
	}
}

void WP6HLContentListener::handleLineBreak()
{
	if(!isUndoOn())
	{
		_flushText();
		// Fridrich Strba -- HACK: we force each tab to be in its own span in order
		// to prevent previous span's properties to apply to the tab also if they changed.
		m_ps->m_textAttributesChanged = true;

		if (m_ps->m_sectionAttributesChanged && !m_ps->m_isTableOpened)
		{
			_openSection();
			m_ps->m_sectionAttributesChanged = false;
		}

		if (m_parseState->m_styleStateSequence.getCurrentState() == STYLE_BODY ||
		    m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
		{
			if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
				if (m_parseState->m_currentListLevel == 0)
					_openParagraph();
				else
					_openListElement();
			_flushText(true);
			m_listenerImpl->insertLineBreak();
			_flushText(true);
			m_ps->m_textAttributesChanged = true;
		}
	}
}

void WP6HLContentListener::insertEOL()
{
	if (!isUndoOn())
	{
		if (m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
			_flushText();
		m_ps->m_numDeferredParagraphBreaks++;
	}

}

void WP6HLContentListener::characterColorChange(const uint8_t red, const uint8_t green, const uint8_t blue)
{
	if (!isUndoOn())
	{
		// flush everything which came before this change
		_flushText();
		m_ps->m_fontColor->m_r = red;
		m_ps->m_fontColor->m_g = green;
		m_ps->m_fontColor->m_b = blue;
		m_ps->m_textAttributesChanged = true;
 	}
}

void WP6HLContentListener::characterShadingChange(const uint8_t shading)
{
	if (!isUndoOn())
	{
		// flush everything which came before this change
		_flushText();
		m_ps->m_fontColor->m_s = shading;
		m_ps->m_textAttributesChanged = true;
	}
}

void WP6HLContentListener::highlightChange(const bool isOn, const RGBSColor color)
{
	if (!isUndoOn())
	{
		//flush everything which came before this change
		_flushText();
		if (isOn)
			m_ps->m_highlightColor = new RGBSColor(color.m_r, color.m_g, color.m_b, color.m_s);
		else
			DELETEP(m_ps->m_highlightColor);
		m_ps->m_textAttributesChanged = true;
	}
}

void WP6HLContentListener::fontChange(const uint16_t matchedFontPointSize, const uint16_t fontPID)
{
	if (!isUndoOn())
	{
		// flush everything which came before this change
		_flushText();

		if (matchedFontPointSize)
		{
			m_ps->m_fontSize = rint((double)((((float)matchedFontPointSize)/100.0f)*2.0f));
			// We compute the real space after paragraph in inches using the size of the font and relative spacing.
			// We have to recompute this every change of fontSize.
			m_ps->m_paragraphMarginBottom =
				(float)(((m_parseState->m_paragraphMarginBottomRelative - 1.0f)*m_ps->m_fontSize)/72.0f) +
				m_parseState->m_paragraphMarginBottomAbsolute;
		}
		if (fontPID)
		{
			const WP6FontDescriptorPacket *fontDescriptorPacket = NULL;
			if (fontDescriptorPacket = dynamic_cast<const WP6FontDescriptorPacket *>(WP6LLListener::getPrefixDataPacket(fontPID))) {
				m_ps->m_fontName->sprintf("%s", fontDescriptorPacket->getFontName());
			}
		}
		m_ps->m_textAttributesChanged = true;
	}
}

void WP6HLContentListener::attributeChange(const bool isOn, const uint8_t attribute)
{
	if (!isUndoOn())
	{
		// flush everything which came before this change
		_flushText();

		uint32_t textAttributeBit = 0;

		// FIXME: handle all the possible attribute bits
		switch (attribute)
		{
		case WP6_ATTRIBUTE_EXTRA_LARGE:
			textAttributeBit = WPX_EXTRA_LARGE_BIT;
			break;
		case WP6_ATTRIBUTE_VERY_LARGE:
			textAttributeBit = WPX_VERY_LARGE_BIT;
			break;
		case WP6_ATTRIBUTE_LARGE:
			textAttributeBit = WPX_LARGE_BIT;
			break;
		case WP6_ATTRIBUTE_SMALL_PRINT:
			textAttributeBit = WPX_SMALL_PRINT_BIT;
			break;
		case WP6_ATTRIBUTE_FINE_PRINT:
			textAttributeBit = WPX_FINE_PRINT_BIT;
			break;		
		case WP6_ATTRIBUTE_SUBSCRIPT:
			textAttributeBit = WPX_SUBSCRIPT_BIT;
			break;
		case WP6_ATTRIBUTE_SUPERSCRIPT:
			textAttributeBit = WPX_SUPERSCRIPT_BIT;
			break;
		case WP6_ATTRIBUTE_OUTLINE:
			textAttributeBit = WPX_OUTLINE_BIT;
			break;
		case WP6_ATTRIBUTE_ITALICS:
			textAttributeBit = WPX_ITALICS_BIT;
			break;
		case WP6_ATTRIBUTE_SHADOW:
			textAttributeBit = WPX_SHADOW_BIT;
			break;
		case WP6_ATTRIBUTE_REDLINE:
			textAttributeBit = WPX_REDLINE_BIT;
			break;
		case WP6_ATTRIBUTE_DOUBLE_UNDERLINE:
			textAttributeBit = WPX_DOUBLE_UNDERLINE_BIT;
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
		case WP6_ATTRIBUTE_SMALL_CAPS:
			textAttributeBit = WPX_SMALL_CAPS_BIT;
			break;
		case WP6_ATTRIBUTE_BLINK:
			textAttributeBit = WPX_BLINK_BIT;
			break;
		}

		if (isOn)
			m_ps->m_textAttributeBits |= textAttributeBit;
		else
			m_ps->m_textAttributeBits ^= textAttributeBit;

		m_ps->m_textAttributesChanged = true;
	}
}

void WP6HLContentListener::spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute)
{
	if (!isUndoOn())
	{
		m_parseState->m_paragraphMarginBottomRelative = spacingRelative;
		m_parseState->m_paragraphMarginBottomAbsolute = spacingAbsolute;
		// We compute the real space after paragraph in inches using the size of the font and relative spacing.
		// We have to recompute this every change of fontSize. That is why we keep the two components in
		// m_parsingState and the following formula is to be found in the fontChange(...) function as well.
		m_ps->m_paragraphMarginBottom =
			(float)(((m_parseState->m_paragraphMarginBottomRelative - 1.0f)*m_ps->m_fontSize)/72.0f) +
			m_parseState->m_paragraphMarginBottomAbsolute;
		// Variable spacingAfterParagraphRelative already contains the height of the space in inches
	}
}

void WP6HLContentListener::marginChange(uint8_t side, uint16_t margin)
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

void WP6HLContentListener::paragraphMarginChange(uint8_t side, int16_t margin)
{
	if (!isUndoOn())
	{
		float marginInch = (float)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
		case WPX_LEFT:
			// This is necessary in case we have Margin Set and Left or LeftRight indentation
			// by Tabs in the same time. The Left or LeftRight indentation applies to the
			// current paragraph only. Margin Set applies untill an new Margin Set code.
			m_ps->m_leftMarginByParagraphMarginChange = marginInch;
			// Add this margin to the column margin set by "marginChange" function.
			m_ps->m_paragraphMarginLeft = m_ps->m_leftMarginByPageMarginChange
						+ m_ps->m_leftMarginByParagraphMarginChange
						+ m_ps->m_leftMarginByTabs;
			break;
		case WPX_RIGHT:
			m_ps->m_rightMarginByParagraphMarginChange = marginInch;
			m_ps->m_paragraphMarginRight = m_ps->m_rightMarginByPageMarginChange
						+ m_ps->m_rightMarginByParagraphMarginChange
						+ m_ps->m_rightMarginByTabs;
			break;
		default:
			break;
		}
	}
}

void WP6HLContentListener::indentFirstLineChange(int16_t offset)
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

void WP6HLContentListener::columnChange(const WPXTextColumnType columnType, const uint8_t numColumns, const std::vector<float> &columnWidth,
		const std::vector<bool> &isFixedWidth)
{
	if (!isUndoOn())
	{
		// In WP, the last column ends with a hard column break code.
		// In this case, we do not really want to insert any column break
		if (m_ps->m_isParagraphPageBreak)
		{
			m_ps->m_isParagraphPageBreak = false;
			m_ps->m_isTextColumnWithoutParagraph = false;
		}
		_flushText();
		float remainingSpace = m_ps->m_pageFormWidth - m_ps->m_pageMarginLeft - m_ps->m_pageMarginRight
						- m_ps->m_leftMarginByPageMarginChange - m_ps->m_rightMarginByPageMarginChange;
		// determine the space that is to be divided between columns whose width is expressed in percentage of remaining space
		std::vector<WPXColumnDefinition> tmpColumnDefinition;
		tmpColumnDefinition.clear();
		if (numColumns > 1)
		{
			int i;
			for (i=0; i<columnWidth.size(); i++)
			{
				if (isFixedWidth[i])
					remainingSpace -= columnWidth[i];
			}
			WPXColumnDefinition tmpColumn;
			for (i=0; i<numColumns; i++)
			{
				if (i == 0)
					tmpColumn.m_leftGutter = 0.0f;
				else if (isFixedWidth[2*i-1])
					tmpColumn.m_leftGutter = 0.5f * columnWidth[2*i-1];
				else
					tmpColumn.m_leftGutter = 0.5f * remainingSpace * columnWidth[2*i-1];
				
				if (i >= (numColumns - 1))
					tmpColumn.m_rightGutter = 0.0f;
				else if (isFixedWidth[2*i+1])
					tmpColumn.m_rightGutter = 0.5f * columnWidth[2*i+1];
				else
					tmpColumn.m_rightGutter = 0.5f * remainingSpace * columnWidth[2*i+1];

				if (isFixedWidth[2*i])
					tmpColumn.m_width = columnWidth[2*i];
				else
					tmpColumn.m_width = remainingSpace * columnWidth[2*i];
				
				tmpColumn.m_width += tmpColumn.m_leftGutter + tmpColumn.m_rightGutter;
				
				tmpColumnDefinition.push_back(tmpColumn);
			}
		}

		m_ps->m_sectionAttributesChanged = true;
		m_ps->m_numColumns = numColumns;
		m_ps->m_textColumns = tmpColumnDefinition;
		m_ps->m_isTextColumnWithoutParagraph = true;
	}
}

void WP6HLContentListener::updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const uint16_t outlineHash,
					    const uint8_t *numberingMethods, const uint8_t tabBehaviourFlag)
{
	WP6OutlineDefinition *tempListDefinition = NULL;
	WPD_DEBUG_MSG(("WordPerfect: Updating OutlineHash %i\n", outlineHash));

	WP6OutlineDefinition *tempOutlineDefinition;
	if (m_outlineDefineHash.find(outlineHash) != m_outlineDefineHash.end())
	{
		tempOutlineDefinition = (m_outlineDefineHash.find(outlineHash))->second;
		tempOutlineDefinition->update(numberingMethods, tabBehaviourFlag);
	}
	else
	{
		tempOutlineDefinition = new WP6OutlineDefinition(outlineLocation, numberingMethods, tabBehaviourFlag);
		m_outlineDefineHash[outlineHash] = tempOutlineDefinition;
	}
}

void WP6HLContentListener::paragraphNumberOn(const uint16_t outlineHash, const uint8_t level, const uint8_t flag)
{
	if (!isUndoOn())
	{
		_paragraphNumberOn(outlineHash, (level + 1));
	}
}

void WP6HLContentListener::paragraphNumberOff()
{
	if (!isUndoOn())
	{
//		m_parseState->m_styleStateSequence.setCurrentState(BEGIN_AFTER_NUMBERING);
		m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
	}
}

void WP6HLContentListener::displayNumberReferenceGroupOn(const uint8_t subGroup, const uint8_t level)
{
	if (!isUndoOn())
	{
		switch (subGroup)
		{
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_PARAGRAPH_NUMBER_ON:
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_USER_DEFINED_ON:
			// HACKISH: if we are in a paragraph style sequence we will pretend that paragraph numbering was
			// just turned on even though it didn't happen
			if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_BEFORE_NUMBERING)
			{
				WPD_DEBUG_MSG(("WordPerfect: Virtual paragraph numbering used since no paragraph number\n"));
				(m_parseState->m_currentListLevel == 0) ? _paragraphNumberOn(0, 1) :
					_paragraphNumberOn(0, m_parseState->m_currentListLevel);
			}
			m_parseState->m_styleStateSequence.setCurrentState(DISPLAY_REFERENCING);
			// HACK: this is the >1st element in a sequence of display reference numbers (e.g.: we could have
			// 1.1.1), pretend it was the first and remove all memory of what came before in the style sequence
			if (m_parseState->m_putativeListElementHasDisplayReferenceNumber) {
				m_parseState->m_numberText.clear();
				m_parseState->m_textAfterDisplayReference.clear();
			}
			m_parseState->m_putativeListElementHasDisplayReferenceNumber = true;
			break;
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_FOOTNOTE_NUMBER_ON:
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_ENDNOTE_NUMBER_ON:
			m_parseState->m_styleStateSequence.setCurrentState(DISPLAY_REFERENCING);
			break;
		}
	}
}

void WP6HLContentListener::displayNumberReferenceGroupOff(const uint8_t subGroup)
{
	if (!isUndoOn())
	{
		switch (subGroup)
		{
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_PARAGRAPH_NUMBER_OFF:
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_USER_DEFINED_OFF:
			if (m_parseState->m_styleStateSequence.getPreviousState() == BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING)
				m_parseState->m_styleStateSequence.setCurrentState(BEGIN_NUMBERING_AFTER_DISPLAY_REFERENCING);
			else {
				m_parseState->m_styleStateSequence.setCurrentState(m_parseState->m_styleStateSequence.getPreviousState());
				// dump all our information into the before numbering block, if the display reference
				// wasn't for a list
				if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_BEFORE_NUMBERING) {
					m_parseState->m_textBeforeNumber.append(m_parseState->m_numberText);
					m_parseState->m_textBeforeNumber.clear();
				}

			}
			break;
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_FOOTNOTE_NUMBER_OFF:
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_ENDNOTE_NUMBER_OFF:
			m_parseState->m_styleStateSequence.setCurrentState(m_parseState->m_styleStateSequence.getPreviousState());
			break;
		}
	}
}

void WP6HLContentListener::styleGroupOn(const uint8_t subGroup)
{
	if (!isUndoOn())
	{
		switch (subGroup)
		{
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_ON_PART1:
			WPD_DEBUG_MSG(("WordPerfect: Handling para style begin 1 (ON)\n"));
 			//_flushText();
			_closeListElement(); _closeParagraph();

			m_parseState->m_styleStateSequence.setCurrentState(BEGIN_BEFORE_NUMBERING);
			m_parseState->m_putativeListElementHasParagraphNumber = false;
			m_parseState->m_putativeListElementHasDisplayReferenceNumber = false;
			break;
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_ON_PART2:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style begin 2 (ON)\n"));
			if (m_ps->m_numDeferredParagraphBreaks > 0) {
				m_ps->m_numDeferredParagraphBreaks--; // very complicated: we are substituting other blocks for paragraph breaks, essentially
				m_parseState->m_numRemovedParagraphBreaks = 1; // set it to 1, rather than incrementing, in case we have a leftover
			}
			_flushText();

			break;
		case WP6_STYLE_GROUP_PARASTYLE_END_ON:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style end (ON)\n"));
			m_parseState->m_styleStateSequence.setCurrentState(STYLE_END);
			_flushText(); // flush the item (list or otherwise) text
			break;
		}
	}
}

void WP6HLContentListener::styleGroupOff(const uint8_t subGroup)
{
	if (!isUndoOn())
	{

		switch (subGroup)
		{
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_OFF_PART1:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style begin 1 (OFF)\n"));
			break;
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_OFF_PART2:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style begin 2 (OFF)\n"));
			m_parseState->m_styleStateSequence.setCurrentState(STYLE_BODY);
			if (m_parseState->m_putativeListElementHasParagraphNumber)
			{
				if (m_ps->m_sectionAttributesChanged && !m_ps->m_isTableOpened)
				{
					_openSection();
					m_ps->m_sectionAttributesChanged = false;
				}

				_handleListChange(m_parseState->m_currentOutlineHash);
			}
			else 
			{
				m_ps->m_numDeferredParagraphBreaks+=m_parseState->m_numRemovedParagraphBreaks;
				m_parseState->m_numRemovedParagraphBreaks = 0;
				_flushText();
			}
			break;
		case WP6_STYLE_GROUP_PARASTYLE_END_OFF:
			WPD_DEBUG_MSG(("WordPerfect: Handling a parastyle end (OFF)\n"));
			m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
			break;
		}
	}
}

void WP6HLContentListener::globalOn(const uint8_t systemStyle)
{
	if (!isUndoOn())
	{
		if (systemStyle == WP6_SYSTEM_STYLE_FOOTNOTE || systemStyle == WP6_SYSTEM_STYLE_ENDNOTE)
			m_parseState->m_styleStateSequence.setCurrentState(DOCUMENT_NOTE_GLOBAL);
	}
}

void WP6HLContentListener::globalOff()
{
	if (!isUndoOn())
	{
		// FIXME: this needs to be verified to be correct in all cases
		m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
	}
}

void WP6HLContentListener::noteOn(const uint16_t textPID)
{
	if (!isUndoOn())
	{
		_flushText();
		m_parseState->m_styleStateSequence.setCurrentState(DOCUMENT_NOTE);
		// save a reference to the text PID, we want to parse
		// the packet after we're through with the footnote ref.
		m_parseState->m_noteTextPID = textPID;
	}
}

void WP6HLContentListener::noteOff(const WPXNoteType noteType)
{
	if (!isUndoOn())
	{
		m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
		WPXNumberingType numberingType = _extractWPXNumberingTypeFromBuf(m_parseState->m_numberText, ARABIC);
		int number = _extractDisplayReferenceNumberFromBuf(m_parseState->m_numberText, numberingType);
		m_parseState->m_numberText.clear(); // we do not need the text version of the number anymore

		WPXPropertyList propList;
		propList.insert("libwpd:number", number);

		if (noteType == FOOTNOTE)
			m_listenerImpl->openFootnote(propList);
		else
			m_listenerImpl->openEndnote(propList);

		uint16_t textPID = m_parseState->m_noteTextPID;
		handleSubDocument(textPID, false, m_parseState->m_tableList, m_parseState->m_nextTableIndice);

		if (noteType == FOOTNOTE)
			m_listenerImpl->closeFootnote();
		else
			m_listenerImpl->closeEndnote();
	}
}

void WP6HLContentListener::endDocument()
{
	// corner case: document ends in a list element
	if (m_parseState->m_styleStateSequence.getCurrentState() != NORMAL)
	{
		// maybe one could simply use the "_flushList()" ????
		_flushText(); // flush the list text
		m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
		_flushText(true); // flush the list exterior (forcing a line break, to make _flushText think we've exited a list)

	}
	// corner case: document contains no end of lines
	else if (!m_ps->m_isParagraphOpened && !m_ps->m_isParagraphClosed)
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
	_closePageSpan();
	m_listenerImpl->endDocument();
}

void WP6HLContentListener::defineTable(uint8_t position, uint16_t leftOffset)
{
	if (!isUndoOn())
	{
		switch (position & 0x07)
		{
		case 0:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN;
			break;
		case 1:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN;
			break;
		case 2:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_CENTER_BETWEEN_MARGINS;
			break;
		case 3:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_FULL;
			break;
		case 4:
			m_ps->m_tableDefinition.m_positionBits = WPX_TABLE_POSITION_ABSOLUTE_FROM_LEFT_MARGIN;
			break;
		default:
			// should not happen
			break;
		}
		// Note: WordPerfect has an offset from the left edge of the page. We translate it to the offset from the left margin
		m_ps->m_tableDefinition.m_leftOffset = (float)((double)leftOffset / (double)WPX_NUM_WPUS_PER_INCH) - m_ps->m_paragraphMarginLeft;

		// remove all the old column information
		m_ps->m_tableDefinition.columns.clear();

		// pull a table definition off of our stack
		m_parseState->m_currentTable = m_parseState->m_tableList[m_parseState->m_nextTableIndice++];
		m_parseState->m_currentTable->makeBordersConsistent();
	}
}

void WP6HLContentListener::addTableColumnDefinition(uint32_t width, uint32_t leftGutter, uint32_t rightGutter)
{
	if (!isUndoOn())
	{
		// define the new column
		WPXColumnDefinition colDef;
		colDef.m_width = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_leftGutter = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		colDef.m_rightGutter = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);

		// add the new column definition to our table definition
		m_ps->m_tableDefinition.columns.push_back(colDef);
	}
}

void WP6HLContentListener::startTable()
{
	if (!isUndoOn())
	{
		//_handleLineBreakElementBegin();

		// save the justification information. We will need it after the table ends.
		m_ps->m_paragraphJustificationBeforeTable = m_ps->m_paragraphJustification;
		// handle corner case where we have a new section, but immediately start with a table
		// FIXME: this isn't a very satisfying solution, and might need to be generalized
		// as we add more table-like structures into the document
		if (m_ps->m_sectionAttributesChanged && !m_ps->m_isTableOpened)
		// !m_ps->m_isTableOpened condition seems impossible for the time being, since
		// there are no nested tables in WP6+ file format, but with "more table-like structures"
		// it is safer to introduce it though. Does not hurt.
		{
			_openSection();
			m_ps->m_sectionAttributesChanged = false;
		}
		_openTable();
	}
}

void WP6HLContentListener::insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow)
{
	if (!isUndoOn())
	{
		_flushText();
		float rowHeightInch = (float)((double) rowHeight / (double)WPX_NUM_WPUS_PER_INCH);
		_openTableRow(rowHeightInch, isMinimumHeight, isHeaderRow);
	}
}

void WP6HLContentListener::insertCell(const uint8_t colSpan, const uint8_t rowSpan, const bool boundFromLeft, const bool boundFromAbove,
					const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
					const RGBSColor * cellBorderColor, 
					const WPXVerticalAlignment cellVerticalAlignment, const uint32_t cellAttributes)
{
	if (!isUndoOn())
	{
		if (m_ps->m_currentTableRow < 0) // cell without a row, invalid
			throw ParseException();
		_flushText();
		_openTableCell(colSpan, rowSpan, boundFromLeft, boundFromAbove,
			       m_parseState->m_currentTable->getCell(m_ps->m_currentTableRow, m_ps->m_currentTableCol)->m_borderBits,       
			       cellFgColor, cellBgColor, cellBorderColor, cellVerticalAlignment);
		m_ps->m_isCellWithoutParagraph = true;
		m_ps->m_cellAttributeBits = cellAttributes;
	}
}

void WP6HLContentListener::endTable()
{
	if (!isUndoOn())
	{
		_flushText();
		_closeTable();
		// restore the justification that was there before the table.
		m_ps->m_paragraphJustification = m_ps->m_paragraphJustificationBeforeTable;
	}
}

// _handleSubDocument: Creates an empty set of document state (saving the old one on a "stack")
// if textPID>0: Parses a wordperfect text packet (e.g.: a footnote or a header), and naively
// sends its text to the hll implementation and naively inserts it into the document
// if textPID=0: Simply creates a blank paragraph
// once finished, restores document state to what it was before
void WP6HLContentListener::_handleSubDocument(uint16_t textPID, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice)
{
	// save our old parsing state on our "stack"
	WP6ParsingState *oldParseState = m_parseState;
	
	m_parseState = new WP6ParsingState(tableList, nextTableIndice);

	if (isHeaderFooter)
	{
		// is it is Header or Footer, assume that the initial page margins are of 1 inch.
		// This is a behaviour that I observed with WP10 -- Fridrich
		marginChange(WP6_COLUMN_GROUP_LEFT_MARGIN_SET, WPX_NUM_WPUS_PER_INCH);
		marginChange(WP6_COLUMN_GROUP_RIGHT_MARGIN_SET, WPX_NUM_WPUS_PER_INCH);
	}

	if (textPID)
		WP6LLListener::getPrefixDataPacket(textPID)->parse(this);
	else
		_openParagraph();
	_flushText();
	_closeSection();

	// restore our old parsing state
	delete m_parseState;
	m_parseState = oldParseState;
	m_parseState->m_noteTextPID = 0;
}

// _handleLineBreakElementBegin: flush everything which came before this change
// eliminating one paragraph break which is now implicit in this change --
// UNLESS the paragraph break represents something else than its name suggests,
// such as a paragraph or column break OR the paragraph break exists by itself
// NB: I know this method is ugly. Sorry kids, the translation between WordPerfect
// and an XMLish format is rather ugly by definition.
// void WP6HLContentListener::_handleLineBreakElementBegin()
// {
// 	if (!m_ps->m_sectionAttributesChanged &&
// 	    m_ps->m_numDeferredParagraphBreaks > 0 &&
// 	    !m_ps->m_isParagraphColumnBreak && !m_ps->m_isParagraphPageBreak)
// 		m_ps->m_numDeferredParagraphBreaks--;
// 	_flushText();
// }

void WP6HLContentListener::_paragraphNumberOn(const uint16_t outlineHash, const uint8_t level)
{
	m_parseState->m_styleStateSequence.setCurrentState(BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING);
	m_parseState->m_putativeListElementHasParagraphNumber = true;
	m_parseState->m_currentOutlineHash = outlineHash;
	m_parseState->m_currentListLevel = level;
}

// _flushText: Flushes text and any section, paragraph, or span properties prior to the text
// paramaters: fakeText. Pretend there is text, even if there isn't any (useful for tabs)
// FIXME: we need to declare a set of preconditions that must be met when this function is called
//
void WP6HLContentListener::_flushText(const bool fakeText)
{

	// take us out of the list, if we definitely have text out of the list (or we have forced a break,
	// which assumes the same condition)
	if (m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
	{
		if (m_parseState->m_currentListLevel > 0 && 
		    (m_ps->m_numDeferredParagraphBreaks > 0 || m_parseState->m_bodyText.len() > 0 || fakeText) &&
		    m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
		{
			m_parseState->m_currentListLevel = 0;
			_handleListChange(m_parseState->m_currentOutlineHash);
			m_ps->m_numDeferredParagraphBreaks--; // we have an implicit break here, when we close the list
			m_ps->m_isParagraphOpened = false;
			m_ps->m_isListElementOpened = false;
		}
	}

	// create a new section, and a new paragraph, if our section attributes have changed and we have inserted
	// something into the document (or we have forced a break, which assumes the same condition)
	if (m_ps->m_sectionAttributesChanged && (m_parseState->m_bodyText.len() > 0 || m_ps->m_numDeferredParagraphBreaks > 0 || fakeText))
	{
		if (!m_ps->m_isTableOpened)
			_openSection();
		if (fakeText)
			_openParagraph();
	}

	if (m_ps->m_numDeferredParagraphBreaks > 0 && (m_parseState->m_styleStateSequence.getCurrentState() == NORMAL ||
						 ((m_parseState->m_styleStateSequence.getCurrentState() == STYLE_BODY ||
						   m_parseState->m_styleStateSequence.getCurrentState() == STYLE_END) &&
						  !m_parseState->m_putativeListElementHasParagraphNumber)))
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened &&
			!(m_ps->m_isTableOpened && !m_ps->m_isTableCellOpened) // don't allow paragraphs to be opened when we have already opened a table, but no cell yet. - MARCM (is it really correct, or should this be fixed elsewhere??)
		)
			m_ps->m_numDeferredParagraphBreaks++;

		while (m_ps->m_numDeferredParagraphBreaks > 1)
			_openParagraph();
		_closeParagraph();
		m_ps->m_numDeferredParagraphBreaks = 0; // compensate for this by requiring a paragraph to be opened
	}
	else if (m_ps->m_textAttributesChanged && (m_parseState->m_bodyText.len() > 0 || fakeText) && (m_ps->m_isParagraphOpened || m_ps->m_isListElementOpened))
	{
		_openSpan();
	}

	if (m_parseState->m_bodyText.len() || (m_parseState->m_textBeforeNumber.len() &&
						  !m_parseState->m_putativeListElementHasParagraphNumber))
	{
		if (!m_ps->m_isParagraphOpened && !m_ps->m_isListElementOpened)
		{
			if (m_parseState->m_currentListLevel == 0)
				_openParagraph();
			else
				_openListElement();
			_openSpan();
		}

		if (m_parseState->m_textBeforeNumber.len() &&
		    !m_parseState->m_putativeListElementHasParagraphNumber)
		{
			m_listenerImpl->insertText(m_parseState->m_textBeforeNumber);
			m_parseState->m_textBeforeNumber.clear();
		}
		if (m_parseState->m_bodyText.len())
		{
			m_listenerImpl->insertText(m_parseState->m_bodyText);
			m_parseState->m_bodyText.clear();
		}
	}

	m_ps->m_textAttributesChanged = false;
}

// FIXME: This code mostly works, but was created more or less by trial and error and does not derive from
// a good mental model of how lists actually work. Developing such a model is complicated (given that the
// WordPerfect developers never had one) but we should at least try.
void WP6HLContentListener::_handleListChange(const uint16_t outlineHash)
{
	WP6OutlineDefinition *outlineDefinition;
	if (m_outlineDefineHash.find(outlineHash) == m_outlineDefineHash.end())
	{
		// handle odd case where an outline define hash is not defined prior to being referenced by
		// a list
		outlineDefinition = new WP6OutlineDefinition();
		m_outlineDefineHash[outlineHash] = outlineDefinition;
	}
	else
		outlineDefinition = m_outlineDefineHash.find(outlineHash)->second;

	int oldListLevel;
	(m_parseState->m_listLevelStack.empty()) ? oldListLevel = 0 : oldListLevel = m_parseState->m_listLevelStack.top();
	if (oldListLevel == 0)
	{
		_closeParagraph();
	}


	if (m_parseState->m_currentListLevel > oldListLevel)
	{
		WPXPropertyList propList;
		propList.insert("libwpd:id", m_parseState->m_currentOutlineHash);
		propList.insert("libwpd:level", m_parseState->m_currentListLevel);

		if (m_parseState->m_putativeListElementHasDisplayReferenceNumber) 
		{
			WPXNumberingType listType = _extractWPXNumberingTypeFromBuf(m_parseState->m_numberText,
										    outlineDefinition->getListType((m_parseState->m_currentListLevel-1)));			
			int number = _extractDisplayReferenceNumberFromBuf(m_parseState->m_numberText, listType);

			propList.insert("style:num-prefix", m_parseState->m_textBeforeDisplayReference);
			propList.insert("style:num-format", _numberingTypeToString(listType));
			propList.insert("style:num-suffix", m_parseState->m_textAfterDisplayReference);
			propList.insert("text:start-value", number);
			propList.insert("text:space-before", (m_parseState->m_currentListLevel * WPX_DEFAULT_LIST_INDENT));

			m_listenerImpl->defineOrderedListLevel(propList);
		}
		else
		{
			propList.insert("text:bullet-char", m_parseState->m_textBeforeDisplayReference);
			propList.insert("text:space-before", (m_parseState->m_currentListLevel * WPX_DEFAULT_LIST_INDENT));
			m_listenerImpl->defineUnorderedListLevel(propList);
		}
		for (int i=(oldListLevel+1); i<=m_parseState->m_currentListLevel; i++) {
  			// Fridrich: we should not define list sublevels _inside_ of list elements;
			// so we just close the element to prevent call graph failure
 			_closeListElement();
			m_parseState->m_listLevelStack.push(i);
 			WPD_DEBUG_MSG(("Pushed level %i onto the list level stack\n", i));
			
			WPXPropertyList propList2;
			propList2.insert("libwpd:id", m_parseState->m_currentOutlineHash);

			if (m_parseState->m_putativeListElementHasDisplayReferenceNumber)
				m_listenerImpl->openOrderedListLevel(propList2);
			else
				m_listenerImpl->openUnorderedListLevel(propList2);
		}
	}
	else if (m_parseState->m_currentListLevel < oldListLevel)
	{
		_closeListElement(); // close the current element, which must exist
		// now keep on closing levels until we reach the current list level, or the list
		// level stack is empty (signalling that we are out of a list)
		while (!m_parseState->m_listLevelStack.empty() && m_parseState->m_listLevelStack.top() > m_parseState->m_currentListLevel)
		{
			int tempListLevel = m_parseState->m_listLevelStack.top();
			m_parseState->m_listLevelStack.pop();
 			WPD_DEBUG_MSG(("Popped level %i off the list level stack\n", tempListLevel));

			// we are assuming that whether or not the current element has a paragraph
			// number or not is representative of the entire list. I think this
			// assumption holds for all wordperfect files, but it's not correct
			// a priori and I hate writing lame excuses like this, so we might want to
			// change this at some point
			if (!m_parseState->m_putativeListElementHasDisplayReferenceNumber)
				m_listenerImpl->closeUnorderedListLevel();
			else
				m_listenerImpl->closeOrderedListLevel();

			// if we are in a sub-level (beyond 1), and we still haven't reached the current list level, 
			// then that implies that we opened an element that needs to be closed..
			if (!m_parseState->m_listLevelStack.empty())
				_closeListElement();
		}
	}
	else if (m_parseState->m_currentListLevel == oldListLevel)
	{
		// keep the last element on the stack, as it's replaced by this element
		// (a NULL operation)
		_closeListElement(); // but close it
	}

	m_parseState->m_textBeforeNumber.clear();
	m_parseState->m_textBeforeDisplayReference.clear();
	m_parseState->m_numberText.clear();
	m_parseState->m_textAfterDisplayReference.clear();
	m_parseState->m_textAfterNumber.clear();

	// open a new list element, if we're still in the list
	if (m_parseState->m_currentListLevel > 0)
	{
		_openListElement();
	}
}

void WP6HLContentListener::_flushList()
{
	_closeListElement();
	m_parseState->m_currentListLevel = 0;
	_handleListChange(m_parseState->m_currentOutlineHash);
}
