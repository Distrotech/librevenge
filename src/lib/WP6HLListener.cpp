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

#include <math.h>
#include <ctype.h>
#include "WP6HLListener.h"
#include "WPXHLListenerImpl.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"
#include "libwpd_internal.h"

#define WP6_DEFAULT_FONT_SIZE 12.0f
#define WP6_DEFAULT_FONT_NAME "Times New Roman"

void destroyOutlineDefinitionNotify(gpointer data)
{
	WP6OutlineDefinition *temp = (WP6OutlineDefinition *)data;
	delete(temp);
}

void destroyOutlineKeyNotify(gpointer data)
{
	int *temp = (int *)data;
	delete(temp);
}

static int _extractNumericValueFromRoman(char romanChar)
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

// _extractDisplayReferenceNumberFromBuf: given a number string in UCS2 represented
// as letters, numbers, or roman numerals.. return an integer value representing its number
// HACK: this function is really cheesey
// NOTE: if the input is not valid, the output is unspecified
static int _extractDisplayReferenceNumberFromBuf(const UCSString &buf, const WPXListType listType)
{
	if (listType == lowercaseRoman || listType == uppercaseRoman)
	{
		int currentSum = 0;
		int lastMark = 0;
		int currentMark = 0;
		for (int i=0; i<buf.getLen(); i++)
		{
			int currentMark = _extractNumericValueFromRoman(buf.getUCS4()[i]);
			if (lastMark < currentMark) {
				currentSum = currentMark - lastMark;
			}
			else
				currentSum+=currentMark;
			lastMark = currentMark;
		}
	} 
	else if (listType == lowercase || listType == uppercase)
	{
		// FIXME: what happens to a lettered list that goes past z? ah
		// the sweet mysteries of life
		if (buf.getLen()==0)
			throw ParseException();
		int c = buf.getUCS4()[0];
		if (listType==lowercase)
			c = toupper(c);
		return (c - 64);
	}
	else if (listType == arabic)
	{
		int currentSum = 0;
		for (int i=0; i<buf.getLen(); i++)
		{
			currentSum *= 10;
			currentSum+=(buf.getUCS4()[i]-48);
		}
		return currentSum;
	}
	
	return 1;
}

static WPXListType _extractListTypeFromBuf(const UCSString &buf, const WPXListType putativeListType)
{
	for (int i=0; i<buf.getLen(); i++)
	{
		if ((buf.getUCS4()[i] == 'I' || buf.getUCS4()[i] == 'V' || buf.getUCS4()[i] == 'X') && 
		    (putativeListType == lowercaseRoman || putativeListType == uppercaseRoman))
			return uppercaseRoman;
		else if ((buf.getUCS4()[i] == 'i' || buf.getUCS4()[i] == 'v' || buf.getUCS4()[i] == 'x') && 
		    (putativeListType == lowercaseRoman || putativeListType == uppercaseRoman))
			return lowercaseRoman;
		else if (buf.getUCS4()[i] >= 'A' && buf.getUCS4()[i] <= 'Z')
			return uppercase;
		else if (buf.getUCS4()[i] >= 'a' && buf.getUCS4()[i] <= 'z')
			return lowercase;		
	}

	return arabic;
}

WP6OutlineDefinition::WP6OutlineDefinition(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods, const guint8 tabBehaviourFlag)
{
	_updateNumberingMethods(outlineLocation, numberingMethods);
}

// update: updates a partially made list definition (usual case where this is used: an
// outline style is defined in a prefix packet, then you are given more information later
// in the document) 
// FIXME: make sure this is in the right place
void WP6OutlineDefinition::update(const guint8 *numberingMethods, const guint8 tabBehaviourFlag)
{
	_updateNumberingMethods(paragraphGroup, numberingMethods);
}

void WP6OutlineDefinition::_updateNumberingMethods(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods)
{
	for (int i=0; i<WP6_NUM_LIST_LEVELS; i++) 
	{
		switch (numberingMethods[i])
		{
		case WP6_INDEX_HEADER_OUTLINE_STYLE_ARABIC_NUMBERING:
			m_listTypes[i] = arabic; 
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_LOWERCASE_NUMBERING:
			m_listTypes[i] = lowercase; 
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_UPPERCASE_NUMBERING:
			m_listTypes[i] = uppercase; 
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_LOWERCASE_ROMAN_NUMBERING:
			m_listTypes[i] = lowercaseRoman;
			break;
		case WP6_INDEX_HEADER_OUTLINE_STYLE_UPPERCASE_ROMAN_NUMBERING:
			m_listTypes[i] = uppercaseRoman;
			break;
			//case WP6_INDEX_HEADER_OUTLINE_STYLE_LEADING_ZERO_ARABIC_NUMBERING:
			//break;
		default:
			m_listTypes[i] = arabic;
		}
	}
	WPD_DEBUG_MSG(("WordPerfect: Updated List Types: (%i %i %i %i %i %i %i %i)\n", 
		       m_listTypes[0], m_listTypes[1], m_listTypes[2], m_listTypes[3],
		       m_listTypes[4], m_listTypes[5], m_listTypes[6], m_listTypes[7]));

}

WP6HLListener::WP6HLListener(WPXHLListenerImpl *listenerImpl) :
	WP6LLListener(), 
	m_listenerImpl(listenerImpl),
	m_textAttributeBits(0),
	m_textAttributesChanged(FALSE),
	m_currentFontSize(WP6_DEFAULT_FONT_SIZE),
	m_currentFontName(g_string_new(WP6_DEFAULT_FONT_NAME)),
	
	m_isParagraphColumnBreak(FALSE),
	m_isParagraphPageBreak(FALSE),
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
	m_paragraphJustificationChanged(FALSE),
	m_isParagraphOpened(FALSE),
	m_isParagraphClosed(FALSE),
	m_numDeferredParagraphBreaks(0),
	m_numRemovedParagraphBreaks(0),

	m_isTableOpened(FALSE),
	
	m_sectionAttributesChanged(TRUE),
	m_numColumns(1),
	m_marginLeft(1.0f),
	m_marginRight(1.0f),
	m_currentRow(-1),
	m_currentColumn(-1),
	
	m_currentListLevel(0),
	m_isPutativeListElementHasParagraphNumber(FALSE),
	m_isPutativeListElementHasDisplayReferenceNumber(FALSE),

	m_isUndoOn(FALSE)
{
}

WP6HLListener::~WP6HLListener()
{
	g_string_free(m_currentFontName, TRUE);
	typedef map<int, WP6OutlineDefinition *>::iterator Iter;
	for (Iter outline = m_outlineDefineHash.begin(); outline != m_outlineDefineHash.end(); outline++) {
		delete(outline->second);
	}
}

void WP6HLListener::insertCharacter(guint16 character)
{
	if (!m_isUndoOn)
	{
		if (m_paragraphStyleStateSequence.getCurrentState() == styleBody || m_paragraphStyleStateSequence.getCurrentState() == notInStyle)
			m_bodyText.append(character);
		else if (m_paragraphStyleStateSequence.getCurrentState() == beginBeforeNumbering)
		{
			m_textBeforeNumber.append(character);
		}
		else if (m_paragraphStyleStateSequence.getCurrentState() == beginNumberingBeforeDisplayReferencing)
		{
			// left delimeter (or the bullet if there is no display referencing)
			m_textBeforeDisplayReference.append(character);
		}
		else if (m_paragraphStyleStateSequence.getCurrentState() == beginDisplayReferencing)
		{
			// the actual paragraph number (in varying forms)
			m_numberText.append(character);
		}
		else if (m_paragraphStyleStateSequence.getCurrentState() == beginNumberingAfterDisplayReferencing)
		{
			// right delimeter (if there was a display no. ref. group)
			m_textAfterDisplayReference.append(character);
		}
		else if (m_paragraphStyleStateSequence.getCurrentState() == beginAfterNumbering)
		{
			m_textAfterNumber.append(character);
		}
	}
}

void WP6HLListener::insertEOL()
{
	if (!m_isUndoOn)
	{
		if (m_paragraphStyleStateSequence.getCurrentState() == notInStyle)
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

	const WP6DefaultInitialFontPacket * defaultInitialFontPacket = NULL;
	if (defaultInitialFontPacket = _getDefaultInitialFontPacket()) {		
		fontChange(defaultInitialFontPacket->getPointSize(), defaultInitialFontPacket->getInitialFontDescriptorPID());	
	}
}

void WP6HLListener::undoChange(const guint8 undoType, const guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START)
		m_isUndoOn = TRUE;
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END)
		m_isUndoOn = FALSE;		
}

void WP6HLListener::fontChange(const guint16 matchedFontPointSize, const guint16 fontPID)
{
	if (!m_isUndoOn)
	{
		// flush everything which came before this change
		_flushText();

		m_currentFontSize = rint((double)((((float)matchedFontPointSize)/100.0f)*2.0f));
		const WP6FontDescriptorPacket *fontDescriptorPacket = NULL;
		if (fontDescriptorPacket = dynamic_cast<const WP6FontDescriptorPacket *>(_getPrefixDataPacket(fontPID))) {
			g_string_printf(m_currentFontName, "%s", fontDescriptorPacket->getFontName());
		}
		m_textAttributesChanged = TRUE;
	}
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
		_handleLineBreakElementBegin();
		
		float marginInch = (float)(((double)margin + (double)WP6_NUM_EXTRA_WPU) / (double)WPX_NUM_WPUS_PER_INCH);

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
		_handleLineBreakElementBegin();
		
		_flushText();
		
		m_numColumns = numColumns;
		m_sectionAttributesChanged = TRUE;
	}
}

void WP6HLListener::updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash, 
					    const guint8 *numberingMethods, const guint8 tabBehaviourFlag)
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

void WP6HLListener::paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag)
{
	if (!m_isUndoOn)
	{
		m_paragraphStyleStateSequence.setCurrentState(beginNumberingBeforeDisplayReferencing);
		m_isPutativeListElementHasParagraphNumber = TRUE;
		m_currentOutlineHash = outlineHash;
		m_currentListLevel = (level + 1);
	}
}

void WP6HLListener::paragraphNumberOff()
{
	if (!m_isUndoOn)
	{		
		m_paragraphStyleStateSequence.setCurrentState(beginAfterNumbering);
	}
}

void WP6HLListener::displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level)
{
	if (!m_isUndoOn)
	{
		switch (subGroup)
		{
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_PARAGRAPH_NUMBER_ON:
			// ..
			m_paragraphStyleStateSequence.setCurrentState(beginDisplayReferencing);
			// HACK: this is the >1st element in a sequence of display reference numbers, pretend it was
			// the first and remove all memory of what came before in the style sequence
			if (m_isPutativeListElementHasDisplayReferenceNumber) {
				m_numberText.clear();
				m_textAfterDisplayReference.clear();	
			}
			m_isPutativeListElementHasDisplayReferenceNumber = TRUE;
			break;
		}
	}
}

void WP6HLListener::displayNumberReferenceGroupOff(const guint8 subGroup)
{
	if (!m_isUndoOn)
	{
		switch (subGroup)
		{
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_PARAGRAPH_NUMBER_OFF:
			if (m_paragraphStyleStateSequence.getPreviousState() == beginNumberingBeforeDisplayReferencing)
			    m_paragraphStyleStateSequence.setCurrentState(beginNumberingAfterDisplayReferencing);
			else {
				m_paragraphStyleStateSequence.setCurrentState(m_paragraphStyleStateSequence.getPreviousState());
				
				// dump all our information into the before numbering block, if the display reference
				// wasn't for a list
				if (m_paragraphStyleStateSequence.getCurrentState() == beginBeforeNumbering) {
					m_textBeforeNumber.append(m_numberText);
					m_textBeforeNumber.clear();	
				}
				
			}
		}
	}
}

void WP6HLListener::styleGroupOn(const guint8 subGroup)
{
	if (!m_isUndoOn)
	{
		switch (subGroup)
		{
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_ON_PART1:
			WPD_DEBUG_MSG(("WordPerfect: Handling para style begin 1 (ON)\n"));
			//_flushText();

			m_paragraphStyleStateSequence.setCurrentState(beginBeforeNumbering);
			//m_paragraphStyleMarginLeft = WP6_PARAGRAPH_STYLE_MARGIN_LEFT_INCREMENT; 
			//m_paragraphStyleTextIndent = WP6_PARAGRAPH_STYLE_TEXT_INDENT_DECREMENT; // WL: this is abi's default (not wordperfect's) -- I think it is better to use abi's
			m_isPutativeListElementHasParagraphNumber = FALSE;
			m_isPutativeListElementHasDisplayReferenceNumber = FALSE;
			break;
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_ON_PART2:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style begin 2 (ON)\n"));
			if (m_numDeferredParagraphBreaks > 0) {
				m_numDeferredParagraphBreaks--; // very complicated: we are substituting other blocks for paragraph breaks, essentially
				m_numRemovedParagraphBreaks = 1; // set it to 1, rather than incrementing, in case we have a leftover
			}
			_flushText();

			break;
		case WP6_STYLE_GROUP_PARASTYLE_END_ON:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style end (ON)\n"));
			m_paragraphStyleStateSequence.setCurrentState(end);
			_flushText(); // flush the item (list or otherwise) text
			break;
		}
	}
}

void WP6HLListener::styleGroupOff(const guint8 subGroup)
{
	if (!m_isUndoOn)
	{

		switch (subGroup)
		{
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_OFF_PART1:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style begin 1 (OFF)\n"));
			break;
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_OFF_PART2:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style begin 2 (OFF)\n"));
			m_paragraphStyleStateSequence.setCurrentState(styleBody);      
			if (m_isPutativeListElementHasParagraphNumber) 
			{
				if (m_sectionAttributesChanged) 
				{
					m_listenerImpl->openSection(m_numColumns, m_marginLeft, m_marginRight);
					m_sectionAttributesChanged = FALSE;
				}
				_handleListChange(m_currentOutlineHash);
				m_listenerImpl->openListElement(m_currentOutlineHash);			
			}
			else {
				m_numDeferredParagraphBreaks+=m_numRemovedParagraphBreaks;
				m_numRemovedParagraphBreaks = 0;
				_flushText();
			}
			break;
		case WP6_STYLE_GROUP_PARASTYLE_END_OFF:
			WPD_DEBUG_MSG(("WordPerfect: Handling a parastyle end (OFF)\n"));		
			m_paragraphStyleStateSequence.setCurrentState(notInStyle);
			break;		
		}
	}
}

void WP6HLListener::endDocument()
{
	// corner case: document ends in a list element
	if (m_paragraphStyleStateSequence.getCurrentState() != notInStyle)
	{
		_flushText();
		m_currentListLevel = 0;
		if (m_isPutativeListElementHasParagraphNumber) 
			_handleListChange(m_currentOutlineHash);
		m_paragraphStyleStateSequence.setCurrentState(notInStyle);
		_flushText();

	}
	// corner case: document contains no end of lines
	else if (!m_isParagraphOpened && !m_isParagraphClosed)
	{
		_flushText();       
	}
	// normal(ish) case document ends either inside a paragraph or outside of one,
	// but not inside an object
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
		_handleLineBreakElementBegin();

		// HACK: handle corner case of tables without an end element by starting
		// a new table?
		if (m_isTableOpened)
			m_listenerImpl->closeTable();

		// handle corner case where we have a new section, but immediately start with a table
		// FIXME: this isn't a very satisfying solution, and might need to be generalized
		// as we add more table-like structures into the document
		if (m_sectionAttributesChanged) 
		{
			m_listenerImpl->openSection(m_numColumns, m_marginLeft, m_marginRight);
			m_sectionAttributesChanged = FALSE;
		}
				
		m_listenerImpl->openTable();
		m_isTableOpened = TRUE;
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
		m_isTableOpened = FALSE;
	}
}

// _handleLineBreakElementBegin: flush everything which came before this change
// eliminating one paragraph break which is now implicit in this change -- 
// UNLESS the paragraph break represents something else than its name suggests, 
// such as a paragraph or column break 
// NB: I know this method is ugly. Sorry kids, the translation between WordPerfect
// and an XMLish format is rather ugly by definition.
void WP6HLListener::_handleLineBreakElementBegin() 
{
	if (!m_sectionAttributesChanged && m_numDeferredParagraphBreaks > 0 &&
	    !m_isParagraphColumnBreak && !m_isParagraphPageBreak)
		m_numDeferredParagraphBreaks--;					
	_flushText();
}

// _flushText
// FIXME: we need to declare a set of preconditions that must be met when this function is called
// 
void WP6HLListener::_flushText()
{		
	if (m_paragraphStyleStateSequence.getCurrentState() == notInStyle) 
	{
		if (m_currentListLevel > 0 && (m_numDeferredParagraphBreaks > 0 || m_bodyText.getLen() > 0) && 
		    m_paragraphStyleStateSequence.getCurrentState() == notInStyle)
		{
			m_currentListLevel = 0;
			_handleListChange(m_currentOutlineHash);
		}
	}

	if (m_sectionAttributesChanged && (m_bodyText.getLen() > 0 || m_numDeferredParagraphBreaks > 0))
	{
		m_listenerImpl->openSection(m_numColumns, m_marginLeft, m_marginRight);
		m_sectionAttributesChanged = FALSE;

		m_listenerImpl->openParagraph(m_paragraphJustification, m_textAttributeBits,
					      m_currentFontName->str, m_currentFontSize, 
					      m_isParagraphColumnBreak, m_isParagraphPageBreak);
		m_isParagraphColumnBreak = FALSE; m_isParagraphPageBreak = FALSE;
		m_isParagraphOpened = TRUE;
		if (m_numDeferredParagraphBreaks > 0)
			m_numDeferredParagraphBreaks--;
	}

	if (m_numDeferredParagraphBreaks > 0 && (m_paragraphStyleStateSequence.getCurrentState() == notInStyle || 
						 ((m_paragraphStyleStateSequence.getCurrentState() == styleBody || 
						   m_paragraphStyleStateSequence.getCurrentState() == end) &&
						  !m_isPutativeListElementHasParagraphNumber)))
	{
		while (m_numDeferredParagraphBreaks > 0)
		{
			m_listenerImpl->openParagraph(m_paragraphJustification, m_textAttributeBits, 
						      m_currentFontName->str, m_currentFontSize,
						      m_isParagraphColumnBreak, m_isParagraphPageBreak);
			m_isParagraphColumnBreak = FALSE; m_isParagraphPageBreak = FALSE;
			m_numDeferredParagraphBreaks--;
		}
		m_isParagraphOpened = TRUE;
	}
	else if (m_textAttributesChanged && m_bodyText.getLen() > 0)
		m_listenerImpl->openSpan(m_textAttributeBits, m_currentFontName->str, m_currentFontSize);

	if (!m_isPutativeListElementHasParagraphNumber && !m_sectionAttributesChanged) // conditional to handle section flush coming just before a style definition

	{		
		m_listenerImpl->insertText(m_textBeforeNumber);
		m_textBeforeNumber.clear();	
	}

	m_listenerImpl->insertText(m_bodyText);
	m_bodyText.clear();

	m_textAttributesChanged = FALSE;
	m_paragraphJustificationChanged = FALSE;
}

void WP6HLListener::_handleListChange(const guint16 outlineHash)
{
	if (m_outlineDefineHash.find(outlineHash) == m_outlineDefineHash.end())
		throw ParseException();
	WP6OutlineDefinition *listDefinition = m_outlineDefineHash.find(outlineHash)->second;
	
	int oldListLevel;
	(m_listLevelStack.empty()) ? oldListLevel = 0 : oldListLevel = m_listLevelStack.top();

	if (m_currentListLevel > oldListLevel)
	{
		if (m_isPutativeListElementHasDisplayReferenceNumber) {
			WPXListType listType = _extractListTypeFromBuf(m_numberText, listDefinition->getListType((m_currentListLevel-1)));
			int startNum = _extractDisplayReferenceNumberFromBuf(m_numberText, listType);

			m_listenerImpl->defineOrderedListLevel(m_currentOutlineHash, m_currentListLevel, listType, 
							       m_textBeforeDisplayReference, m_textAfterDisplayReference,
							       startNum);
		}
		else
			m_listenerImpl->defineUnorderedListLevel(m_currentOutlineHash, m_currentListLevel, m_textBeforeDisplayReference);

		for (gint i=(oldListLevel+1); i<=m_currentListLevel; i++) {
			m_listLevelStack.push(i);
 			WPD_DEBUG_MSG(("Pushed level %i onto the list level stack\n", i));
			if (m_isPutativeListElementHasDisplayReferenceNumber) 
				m_listenerImpl->openOrderedListLevel(m_currentOutlineHash);
			else 
				m_listenerImpl->openUnorderedListLevel(m_currentOutlineHash);
		}
	}
	else if (m_currentListLevel < oldListLevel)
	{
		m_listenerImpl->closeListElement(); // close the current element, which must exist
		// now keep on closing levels until we reach the current list level, or the list
		// level stack is empty (signalling that we are out of a list)
		while (!m_listLevelStack.empty() && m_listLevelStack.top() > m_currentListLevel)
		{
			int tempListLevel = m_listLevelStack.top(); m_listLevelStack.pop();
 			WPD_DEBUG_MSG(("Popped level %i off the list level stack\n", tempListLevel));
			// we are assuming that whether or not the current element has a paragraph
			// number or not is representative of the entire list. I think this
			// assumption holds for all wordperfect files, but it's not correct
			// a priori and I hate writing lame excuses like this, so we might want to
			// change this at some point
			if (!m_isPutativeListElementHasDisplayReferenceNumber)
				m_listenerImpl->closeUnorderedListLevel();
			else
				m_listenerImpl->closeOrderedListLevel();

			if (tempListLevel > 1)
				m_listenerImpl->closeListElement();
		}
	}
	else if (m_currentListLevel == oldListLevel)
	{
		// keep the last element on the stack, as it's replaced by this element
		// (a NULL operation)
		m_listenerImpl->closeListElement(); // but close it
	}

	m_textBeforeNumber.clear();
	m_textBeforeDisplayReference.clear();	
	m_numberText.clear();	
	m_textAfterDisplayReference.clear();	
	m_textAfterNumber.clear();	
	
}
