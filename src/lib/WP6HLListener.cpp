/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
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

#include "WP6PrefixData.h"

#define WP6_DEFAULT_FONT_SIZE 12.0f
#define WP6_DEFAULT_FONT_NAME "Times New Roman"

// HACK: this function is really cheesey
int _extractNumericValueFromRoman(const gchar romanChar)
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
gint _extractDisplayReferenceNumberFromBuf(const UCSString &buf, const NumberingType listType)
{
	if (listType == LOWERCASE_ROMAN || listType == UPPERCASE_ROMAN)
	{
		gint currentSum = 0;
		gint lastMark = 0;
		gint currentMark = 0;
		for (int i=0; i<buf.getLen(); i++)
		{
			gint currentMark = _extractNumericValueFromRoman(buf.getUCS4()[i]);
			if (lastMark < currentMark) {
				currentSum = currentMark - lastMark;
			}
			else
				currentSum+=currentMark;
			lastMark = currentMark;
		}
	} 
	else if (listType == LOWERCASE || listType == UPPERCASE)
	{
		// FIXME: what happens to a lettered list that goes past z? ah
		// the sweet mysteries of life
		if (buf.getLen()==0)
			throw ParseException();
		guint32 c = buf.getUCS4()[0];
		if (listType==LOWERCASE)
			c = toupper(c);
		return (c - 64);
	}
	else if (listType == ARABIC)
	{
		gint currentSum = 0;
		for (int i=0; i<buf.getLen(); i++)
		{
			currentSum *= 10;
			currentSum+=(buf.getUCS4()[i]-48);
		}
		return currentSum;
	}
	
	return 1;
}

NumberingType _extractNumberingTypeFromBuf(const UCSString &buf, const NumberingType putativeNumberingType)
{

	for (int i=0; i<buf.getLen(); i++)
	{
		if ((buf.getUCS4()[i] == 'I' || buf.getUCS4()[i] == 'V' || buf.getUCS4()[i] == 'X') && 
		    (putativeNumberingType == LOWERCASE_ROMAN || putativeNumberingType == UPPERCASE_ROMAN))
			return UPPERCASE_ROMAN;
		else if ((buf.getUCS4()[i] == 'i' || buf.getUCS4()[i] == 'v' || buf.getUCS4()[i] == 'x') && 
		    (putativeNumberingType == LOWERCASE_ROMAN || putativeNumberingType == UPPERCASE_ROMAN))
			return LOWERCASE_ROMAN;
		else if (buf.getUCS4()[i] >= 'A' && buf.getUCS4()[i] <= 'Z')
			return UPPERCASE;
		else if (buf.getUCS4()[i] >= 'a' && buf.getUCS4()[i] <= 'z')
			return LOWERCASE;		
	}

	return ARABIC;
}

WP6OutlineDefinition::WP6OutlineDefinition(const WP6OutlineLocation outlineLocation, const guint8 *numberingMethods, const guint8 tabBehaviourFlag)
{
	_updateNumberingMethods(outlineLocation, numberingMethods);
}

WP6OutlineDefinition::WP6OutlineDefinition()
{	
	guint8 numberingMethods[WP6_NUM_LIST_LEVELS];
	for (int i=0; i<WP6_NUM_LIST_LEVELS; i++)
		numberingMethods[i] = WP6_INDEX_HEADER_OUTLINE_STYLE_ARABIC_NUMBERING;

	_updateNumberingMethods(paragraphGroup, numberingMethods);
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

_WP6ParsingState::_WP6ParsingState() :
	m_textAttributeBits(0),
	m_textAttributesChanged(FALSE),
	m_fontSize(WP6_DEFAULT_FONT_SIZE),
	m_fontName(g_string_new(WP6_DEFAULT_FONT_NAME)),
	
	m_isParagraphColumnBreak(FALSE),
	m_isParagraphPageBreak(FALSE),
	m_paragraphLineSpacing(1.0f),
	m_paragraphJustification(WPX_PARAGRAPH_JUSTIFICATION_LEFT),
	m_paragraphJustificationChanged(FALSE),

	m_isSectionOpened(FALSE),

	m_isParagraphOpened(FALSE),
	m_isParagraphClosed(FALSE),
	m_isSpanOpened(FALSE),
	m_numDeferredParagraphBreaks(0),
	m_numRemovedParagraphBreaks(0),

	m_isTableOpened(FALSE),
	m_isTableRowOpened(FALSE),
	m_isTableCellOpened(FALSE),

	m_sectionAttributesChanged(TRUE),
	m_numColumns(1),
	m_marginLeft(1.0f),
	m_marginRight(1.0f),
	m_currentRow(-1),
	m_currentColumn(-1),
	
	m_currentListLevel(0),
	m_putativeListElementHasParagraphNumber(FALSE),
	m_putativeListElementHasDisplayReferenceNumber(FALSE),

	m_noteTextPID(0),
	m_inSubDocument(FALSE),

	m_isUndoOn(FALSE)
{
}

WP6HLListener::WP6HLListener(WPXHLListenerImpl *listenerImpl) :
	WP6LLListener(), 
	m_listenerImpl(listenerImpl),
	m_parseState(new WP6ParsingState)
{
}

WP6HLListener::~WP6HLListener()
{
	g_string_free(m_parseState->m_fontName, TRUE);
	typedef map<int, WP6OutlineDefinition *>::iterator Iter;
	for (Iter outline = m_outlineDefineHash.begin(); outline != m_outlineDefineHash.end(); outline++) {
		delete(outline->second);
	}
	delete m_parseState;
}

void WP6HLListener::setExtendedInformation(const guint16 type, const UCSString &data)
{
	switch (type)
	{		
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_AUTHOR):
		m_metaData.m_author.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_SUBJECT):
		m_metaData.m_subject.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_PUBLISHER):
		m_metaData.m_publisher.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_CATEGORY):
		m_metaData.m_category.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_KEYWORDS):
		m_metaData.m_keywords.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_LANGUAGE):
		m_metaData.m_language.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_ABSTRACT):
		m_metaData.m_abstract.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DESCRIPTIVE_NAME):
		m_metaData.m_descriptiveName.append(data);
		break;
	case (WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DESCRIPTIVE_TYPE):
		m_metaData.m_descriptiveType.append(data);
		break;
	}
}

void WP6HLListener::insertCharacter(const guint16 character)
{
	if (!m_parseState->m_isUndoOn)
	{
		if (m_parseState->m_styleStateSequence.getCurrentState() == STYLE_BODY || 
		    m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
			m_parseState->m_bodyText.append(character);
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_BEFORE_NUMBERING)
		{
			m_parseState->m_textBeforeNumber.append(character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING)
		{
			// left delimeter (or the bullet if there is no display referencing)
			m_parseState->m_textBeforeDisplayReference.append(character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == DISPLAY_REFERENCING)
		{
			// the actual paragraph number (in varying forms)
			m_parseState->m_numberText.append(character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_NUMBERING_AFTER_DISPLAY_REFERENCING)
		{
			// right delimeter (if there was a display no. ref. group)
			m_parseState->m_textAfterDisplayReference.append(character);
		}
		else if (m_parseState->m_styleStateSequence.getCurrentState() == BEGIN_AFTER_NUMBERING)
		{
			m_parseState->m_textAfterNumber.append(character);
		}
	}
}

void WP6HLListener::insertTab()
{
	if (!m_parseState->m_isUndoOn)
	{
		if (m_parseState->m_styleStateSequence.getCurrentState() == STYLE_BODY || 
		    m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
		{
			// TRUE == force an initial paragraph break, because we are inserting some data, _flushText
			// just doesn't know about it
			_flushText(TRUE); 
			m_listenerImpl->insertTab();
		}
	}
}

void WP6HLListener::insertEOL()
{
	if (!m_parseState->m_isUndoOn)
	{
		if (m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
			_flushText();		
		m_parseState->m_numDeferredParagraphBreaks++; 
	}
	
}

void WP6HLListener::insertBreak(const guint8 breakType)
{
	if (!m_parseState->m_isUndoOn)
	{	
		_flushText();
		m_parseState->m_numDeferredParagraphBreaks++;
		switch (breakType) 
		{
		case WPX_COLUMN_BREAK:
			m_parseState->m_isParagraphColumnBreak = TRUE;
			break;
		case WPX_PAGE_BREAK:
			m_parseState->m_isParagraphPageBreak = TRUE;
			break;
			// TODO: (.. line break?)
		}
		//m_listenerImpl->insertBreak(breakType);
	}
}

void WP6HLListener::startDocument()
{
	m_listenerImpl->startHeader();
	m_listenerImpl->setDocumentMetaData(m_metaData.m_author, m_metaData.m_subject,
					    m_metaData.m_publisher, m_metaData.m_category,
					    m_metaData.m_keywords, m_metaData.m_language,
					    m_metaData.m_abstract, m_metaData.m_descriptiveName,
					    m_metaData.m_descriptiveType);
	m_listenerImpl->endHeader();

	m_listenerImpl->startDocument();
}

void WP6HLListener::undoChange(const guint8 undoType, const guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START)
		m_parseState->m_isUndoOn = TRUE;
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END)
		m_parseState->m_isUndoOn = FALSE;		
}

void WP6HLListener::fontChange(const guint16 matchedFontPointSize, const guint16 fontPID)
{
	if (!m_parseState->m_isUndoOn)
	{
		// flush everything which came before this change
		_flushText();

		m_parseState->m_fontSize = rint((double)((((float)matchedFontPointSize)/100.0f)*2.0f));
		const WP6FontDescriptorPacket *fontDescriptorPacket = NULL;
		if (fontDescriptorPacket = dynamic_cast<const WP6FontDescriptorPacket *>(_getPrefixDataPacket(fontPID))) {
			g_string_printf(m_parseState->m_fontName, "%s", fontDescriptorPacket->getFontName());
		}
		m_parseState->m_textAttributesChanged = TRUE;
	}
}

void WP6HLListener::attributeChange(const gboolean isOn, const guint8 attribute)
{
	if (!m_parseState->m_isUndoOn)
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
			m_parseState->m_textAttributeBits |= textAttributeBit;
		else
			m_parseState->m_textAttributeBits ^= textAttributeBit;
		
		m_parseState->m_textAttributesChanged = TRUE;
	}
}

void WP6HLListener::lineSpacingChange(const float lineSpacing)
{
	if (!m_parseState->m_isUndoOn)
	{
		m_parseState->m_paragraphLineSpacing = lineSpacing;
	}
}

void WP6HLListener::justificationChange(const guint8 justification)
{
	if (!m_parseState->m_isUndoOn)
	{
		switch (justification)
		{
		case WP6_PARAGRAPH_JUSTIFICATION_LEFT:
		case WP6_PARAGRAPH_JUSTIFICATION_FULL:
			m_parseState->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_LEFT;
			break;
		case WP6_PARAGRAPH_JUSTIFICATION_CENTER:
			m_parseState->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_CENTER;
			break;
		case WP6_PARAGRAPH_JUSTIFICATION_RIGHT:
			m_parseState->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RIGHT;
			break;
		case WP6_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES:
			m_parseState->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES;
			break;
		case WP6_PARAGRAPH_JUSTIFICATION_RESERVED:
			m_parseState->m_paragraphJustification = WPX_PARAGRAPH_JUSTIFICATION_RESERVED;
			break;
		}
		
		m_parseState->m_paragraphJustificationChanged = TRUE;
	}
}

void WP6HLListener::marginChange(guint8 side, guint16 margin)
{
	if (!m_parseState->m_isUndoOn)
	{
		_handleLineBreakElementBegin();
		
		float marginInch = (float)(((double)margin + (double)WP6_NUM_EXTRA_WPU) / (double)WPX_NUM_WPUS_PER_INCH);

		switch(side)
		{
		case WP6_COLUMN_GROUP_LEFT_MARGIN_SET:
			m_parseState->m_marginLeft = marginInch;
			break;
		case WP6_COLUMN_GROUP_RIGHT_MARGIN_SET:
			m_parseState->m_marginRight = marginInch;
			break;
		}

		m_parseState->m_sectionAttributesChanged = TRUE;
	}	
}

void WP6HLListener::columnChange(guint8 numColumns)
{
	if (!m_parseState->m_isUndoOn)
	{
		_handleLineBreakElementBegin();
		
		_flushText();
		
		m_parseState->m_numColumns = numColumns;
		m_parseState->m_sectionAttributesChanged = TRUE;
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
	if (!m_parseState->m_isUndoOn)
	{
		m_parseState->m_styleStateSequence.setCurrentState(BEGIN_NUMBERING_BEFORE_DISPLAY_REFERENCING);
		m_parseState->m_putativeListElementHasParagraphNumber = TRUE;
		m_parseState->m_currentOutlineHash = outlineHash;
		m_parseState->m_currentListLevel = (level + 1);
	}
}

void WP6HLListener::paragraphNumberOff()
{
	if (!m_parseState->m_isUndoOn)
	{		
		m_parseState->m_styleStateSequence.setCurrentState(BEGIN_AFTER_NUMBERING);
	}
}

void WP6HLListener::displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level)
{
	if (!m_parseState->m_isUndoOn)
	{
		switch (subGroup)
		{
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_PARAGRAPH_NUMBER_ON:
			// ..
			m_parseState->m_styleStateSequence.setCurrentState(DISPLAY_REFERENCING);
			// HACK: this is the >1st element in a sequence of display reference numbers, pretend it was
			// the first and remove all memory of what came before in the style sequence
			if (m_parseState->m_putativeListElementHasDisplayReferenceNumber) {
				m_parseState->m_numberText.clear();
				m_parseState->m_textAfterDisplayReference.clear();	
			}
			m_parseState->m_putativeListElementHasDisplayReferenceNumber = TRUE;
			break;
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_FOOTNOTE_NUMBER_ON:
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_ENDNOTE_NUMBER_ON:
			m_parseState->m_styleStateSequence.setCurrentState(DISPLAY_REFERENCING);
			break;
		}
	}
}

void WP6HLListener::displayNumberReferenceGroupOff(const guint8 subGroup)
{
	if (!m_parseState->m_isUndoOn)
	{
		switch (subGroup)
		{
		case WP6_DISPLAY_NUMBER_REFERENCE_GROUP_PARAGRAPH_NUMBER_OFF:
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

void WP6HLListener::styleGroupOn(const guint8 subGroup)
{
	if (!m_parseState->m_isUndoOn)
	{
		switch (subGroup)
		{
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_ON_PART1:
			WPD_DEBUG_MSG(("WordPerfect: Handling para style begin 1 (ON)\n"));
			//_flushText();

			m_parseState->m_styleStateSequence.setCurrentState(BEGIN_BEFORE_NUMBERING);
			m_parseState->m_putativeListElementHasParagraphNumber = FALSE;
			m_parseState->m_putativeListElementHasDisplayReferenceNumber = FALSE;
			break;
		case WP6_STYLE_GROUP_PARASTYLE_BEGIN_ON_PART2:
			WPD_DEBUG_MSG(("WordPerfect: Handling a para style begin 2 (ON)\n"));
			if (m_parseState->m_numDeferredParagraphBreaks > 0) {
				m_parseState->m_numDeferredParagraphBreaks--; // very complicated: we are substituting other blocks for paragraph breaks, essentially
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

void WP6HLListener::styleGroupOff(const guint8 subGroup)
{
	if (!m_parseState->m_isUndoOn)
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
				if (m_parseState->m_sectionAttributesChanged) 
				{
					_openSection();
					m_parseState->m_sectionAttributesChanged = FALSE;
				}
				
				_handleListChange(m_parseState->m_currentOutlineHash);
			}
			else {
				m_parseState->m_numDeferredParagraphBreaks+=m_parseState->m_numRemovedParagraphBreaks;
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

void WP6HLListener::noteOn(const guint16 textPID)
{
	if (!m_parseState->m_isUndoOn)
	{
		_flushText();
		m_parseState->m_styleStateSequence.setCurrentState(DOCUMENT_NOTE);
		// save a reference to the text PID, we want to parse 
		// the packet after we're through with the footnote ref.
		m_parseState->m_noteTextPID = textPID;
	}
}

void WP6HLListener::noteOff(const NoteType noteType)
{
	if (!m_parseState->m_isUndoOn)
	{
		m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
		NumberingType numberingType = _extractNumberingTypeFromBuf(m_parseState->m_numberText, ARABIC);
		int number = _extractDisplayReferenceNumberFromBuf(m_parseState->m_numberText, numberingType);
		if (noteType == FOOTNOTE)
			m_listenerImpl->openFootnote(number);
		else
			m_listenerImpl->openEndnote(number);
		// save our old parsing state on our "stack"
		WP6ParsingState *oldParseState = m_parseState;
		int textPID = oldParseState->m_noteTextPID;
		m_parseState = new WP6ParsingState;

		_getPrefixDataPacket(textPID)->parse(this);	
		_flushText();
		_closeSection();
		if (noteType == FOOTNOTE)
			m_listenerImpl->closeFootnote();		
		else
			m_listenerImpl->closeEndnote();		

		// restore our old parsing state
		delete m_parseState;
		m_parseState = oldParseState;		
		m_parseState->m_noteTextPID = 0;
	}
}

void WP6HLListener::endDocument()
{
	// corner case: document ends in a list element
	if (m_parseState->m_styleStateSequence.getCurrentState() != NORMAL)
	{
		_flushText();
		m_parseState->m_currentListLevel = 0;
		if (m_parseState->m_putativeListElementHasParagraphNumber) 
			_handleListChange(m_parseState->m_currentOutlineHash);
		m_parseState->m_styleStateSequence.setCurrentState(NORMAL);
		_flushText();

	}
	// corner case: document contains no end of lines
	else if (!m_parseState->m_isParagraphOpened && !m_parseState->m_isParagraphClosed)
	{
		_flushText();       
	}
	// NORMAL(ish) case document ends either inside a paragraph or outside of one,
	// but not inside an object
	else if (!m_parseState->m_isParagraphClosed || !m_parseState->m_isParagraphOpened)
	{
		_flushText();
	}
	
	// the only other possibility is a logical contradiction: a paragraph
	// may not be opened and closed at the same time

	// close the document nice and tight
	_closeSection();
	m_listenerImpl->endDocument();
}

void WP6HLListener::startTable()
{
	if (!m_parseState->m_isUndoOn) 
	{		
		_handleLineBreakElementBegin();

		// handle corner case where we have a new section, but immediately start with a table
		// FIXME: this isn't a very satisfying solution, and might need to be generalized
		// as we add more table-like structures into the document
		if (m_parseState->m_sectionAttributesChanged) 
		{
			_openSection();
			m_parseState->m_sectionAttributesChanged = FALSE;
		}
		
		_openTable();
	}
}

void WP6HLListener::insertRow()
{
	if (!m_parseState->m_isUndoOn) 
	{			
		_flushText();
		_openTableRow();
	}
}

void WP6HLListener::insertCell(const guint8 colSpan, const guint8 rowSpan, const gboolean boundFromLeft, const gboolean boundFromAbove, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	if (!m_parseState->m_isUndoOn) 
	{			
		_flushText();
		_openTableCell(colSpan, rowSpan, boundFromLeft, boundFromAbove, cellFgColor, cellBgColor);
	}
}

void WP6HLListener::endTable()
{
	if (!m_parseState->m_isUndoOn) 
	{			
		_flushText();
		_closeTable();
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
	if (!m_parseState->m_sectionAttributesChanged && 
	    m_parseState->m_numDeferredParagraphBreaks > 0 &&
	    !m_parseState->m_isParagraphColumnBreak && !m_parseState->m_isParagraphPageBreak) 
		m_parseState->m_numDeferredParagraphBreaks--;					
	_flushText();
}

// _flushText
// FIXME: we need to declare a set of preconditions that must be met when this function is called
// 
void WP6HLListener::_flushText(const gboolean forceInitialParagraph)
{		

	// take us out of the list, if we definitely have text out of the list (or we have forced a break,
	// which assumes the same condition)
	if (m_parseState->m_styleStateSequence.getCurrentState() == NORMAL) 
	{
		if (m_parseState->m_currentListLevel > 0 && (m_parseState->m_numDeferredParagraphBreaks > 0 || m_parseState->m_bodyText.getLen() > 0 || forceInitialParagraph) && 
		    m_parseState->m_styleStateSequence.getCurrentState() == NORMAL)
		{
			m_parseState->m_currentListLevel = 0;
			_handleListChange(m_parseState->m_currentOutlineHash);
		}
	}

	// create a new section, and a new paragraph, if our section attributes have changed and we have inserted
	// something into the document (or we have forced a break, which assumes the same condition)
	if (m_parseState->m_sectionAttributesChanged && (m_parseState->m_bodyText.getLen() > 0 || m_parseState->m_numDeferredParagraphBreaks > 0 || forceInitialParagraph ))
	{
		_openSection();
		_openParagraph();
	}

	if (m_parseState->m_numDeferredParagraphBreaks > 0 && (m_parseState->m_styleStateSequence.getCurrentState() == NORMAL || 
						 ((m_parseState->m_styleStateSequence.getCurrentState() == STYLE_BODY || 
						   m_parseState->m_styleStateSequence.getCurrentState() == STYLE_END) &&
						  !m_parseState->m_putativeListElementHasParagraphNumber)))
	{
		while (m_parseState->m_numDeferredParagraphBreaks > 0)
		{
			_openParagraph();
		}
		m_parseState->m_isParagraphOpened = TRUE;
	}
	else if (m_parseState->m_textAttributesChanged && m_parseState->m_bodyText.getLen() > 0) 
	{
		_openSpan();
	}
	if (!m_parseState->m_putativeListElementHasParagraphNumber && !m_parseState->m_sectionAttributesChanged) // conditional to handle section flush coming just before a style definition

	{		
		m_listenerImpl->insertText(m_parseState->m_textBeforeNumber);
		m_parseState->m_textBeforeNumber.clear();	
	}

	if (m_parseState->m_bodyText.getLen()) {
		m_listenerImpl->insertText(m_parseState->m_bodyText);
		m_parseState->m_bodyText.clear();
	}

	m_parseState->m_textAttributesChanged = FALSE;
	m_parseState->m_paragraphJustificationChanged = FALSE;
}

void WP6HLListener::_handleListChange(const guint16 outlineHash)
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

	if (m_parseState->m_currentListLevel > oldListLevel)
	{
		if (m_parseState->m_putativeListElementHasDisplayReferenceNumber) {
			NumberingType listType = _extractNumberingTypeFromBuf(m_parseState->m_numberText, 
									      outlineDefinition->getListType((m_parseState->m_currentListLevel-1)));
			gint number = _extractDisplayReferenceNumberFromBuf(m_parseState->m_numberText, listType);
			m_listenerImpl->defineOrderedListLevel(m_parseState->m_currentOutlineHash, 
							       m_parseState->m_currentListLevel, listType, 
							       m_parseState->m_textBeforeDisplayReference, 
							       m_parseState->m_textAfterDisplayReference,
							       number);
		}
		else
			m_listenerImpl->defineUnorderedListLevel(m_parseState->m_currentOutlineHash, 
								 m_parseState->m_currentListLevel, 
								 m_parseState->m_textBeforeDisplayReference);

		for (gint i=(oldListLevel+1); i<=m_parseState->m_currentListLevel; i++) {
			m_parseState->m_listLevelStack.push(i);
 			WPD_DEBUG_MSG(("Pushed level %i onto the list level stack\n", i));
			_closeParagraph();
			if (m_parseState->m_putativeListElementHasDisplayReferenceNumber) 			
				m_listenerImpl->openOrderedListLevel(m_parseState->m_currentOutlineHash);
			else 
				m_listenerImpl->openUnorderedListLevel(m_parseState->m_currentOutlineHash);
		}
	}
	else if (m_parseState->m_currentListLevel < oldListLevel)
	{
		_closeSpan(); // close any span which was opened in this list element
		m_listenerImpl->closeListElement(); // close the current element, which must exist
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

			if (tempListLevel > 1)
				m_listenerImpl->closeListElement();
		}
	}
	else if (m_parseState->m_currentListLevel == oldListLevel)
	{
		// keep the last element on the stack, as it's replaced by this element
		// (a NULL operation)
		_closeSpan();
		m_listenerImpl->closeListElement(); // but close it
	}

	m_parseState->m_textBeforeNumber.clear();
	m_parseState->m_textBeforeDisplayReference.clear();	
	m_parseState->m_numberText.clear();	
	m_parseState->m_textAfterDisplayReference.clear();	
	m_parseState->m_textAfterNumber.clear();	

	// open a new list element, if we're still in the list
	if (m_parseState->m_currentListLevel > 0)
	{
		m_listenerImpl->openListElement(m_parseState->m_paragraphJustification, m_parseState->m_textAttributeBits,
						m_parseState->m_fontName->str, m_parseState->m_fontSize, 
						m_parseState->m_paragraphLineSpacing);
	}	
}

void WP6HLListener::_openSection()
{
	_closeSection();
	m_listenerImpl->openSection(m_parseState->m_numColumns, m_parseState->m_marginLeft, m_parseState->m_marginRight);	
	m_parseState->m_sectionAttributesChanged = FALSE;
	m_parseState->m_isSectionOpened = TRUE;
}

void WP6HLListener::_closeSection()
{
	_closeParagraph();
	if (m_parseState->m_isSectionOpened)
		m_listenerImpl->closeSection();

	m_parseState->m_isSectionOpened = FALSE;
}

void WP6HLListener::_openTable()
{
	_closeTable();
	
	m_listenerImpl->openTable();
	m_parseState->m_isTableOpened = TRUE;
}

void WP6HLListener::_closeTable()
{
	_closeTableRow();

	if (m_parseState->m_isTableOpened)
	{ 
		m_listenerImpl->closeTable();
		m_parseState->m_currentRow = 0;
		m_parseState->m_currentColumn = 0;
		m_parseState->m_numDeferredParagraphBreaks++;
	}
	m_parseState->m_isTableOpened = FALSE;
}

void WP6HLListener::_openTableRow()
{
	_closeTableRow();
	m_parseState->m_currentRow++;
	m_parseState->m_currentColumn = -1;
	m_listenerImpl->openTableRow();
	m_parseState->m_isTableRowOpened = TRUE;
}

void WP6HLListener::_closeTableRow()
{
	_closeTableCell();

	if (m_parseState->m_isTableRowOpened) 
		m_listenerImpl->closeTableRow();
	m_parseState->m_isTableRowOpened = FALSE;
}

void WP6HLListener::_openTableCell(const guint8 colSpan, const guint8 rowSpan, const gboolean boundFromLeft, const gboolean boundFromAbove, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
		m_parseState->m_currentColumn++;
		if (!boundFromLeft && !boundFromAbove) 
		{
			_closeTableCell();
			m_parseState->m_numDeferredParagraphBreaks++;
			m_listenerImpl->openTableCell(m_parseState->m_currentColumn, m_parseState->m_currentRow, 
						      colSpan, rowSpan, cellFgColor, cellBgColor);
			m_parseState->m_isTableCellOpened = TRUE;
		}
}

void WP6HLListener::_closeTableCell()
{
	_closeParagraph();
	if (m_parseState->m_isTableCellOpened)
		m_listenerImpl->closeTableCell();

	m_parseState->m_isTableCellOpened = FALSE;
}

void WP6HLListener::_openParagraph()
{
	_closeParagraph();

	m_listenerImpl->openParagraph(m_parseState->m_paragraphJustification, m_parseState->m_textAttributeBits,
				      m_parseState->m_fontName->str, m_parseState->m_fontSize, 
				      m_parseState->m_paragraphLineSpacing, 
				      m_parseState->m_isParagraphColumnBreak, m_parseState->m_isParagraphPageBreak);
	if (m_parseState->m_numDeferredParagraphBreaks > 0) 
		m_parseState->m_numDeferredParagraphBreaks--;

	m_parseState->m_isParagraphColumnBreak = FALSE; 
	m_parseState->m_isParagraphPageBreak = FALSE;
	m_parseState->m_isParagraphOpened = TRUE;
}

void WP6HLListener::_closeParagraph()
{
	_closeSpan();
	if (m_parseState->m_isParagraphOpened)
		m_listenerImpl->closeParagraph();
	

	m_parseState->m_isParagraphOpened = FALSE;
}

void WP6HLListener::_openSpan()
{
	_closeSpan();
	m_listenerImpl->openSpan(m_parseState->m_textAttributeBits, 
				 m_parseState->m_fontName->str, 
				 m_parseState->m_fontSize);	

	m_parseState->m_isSpanOpened = TRUE;
}

void WP6HLListener::_closeSpan()
{
	if (m_parseState->m_isSpanOpened)
		m_listenerImpl->closeSpan();

	m_parseState->m_isSpanOpened = FALSE;
}
