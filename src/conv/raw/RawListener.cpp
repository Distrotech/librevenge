/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002,2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#include <stdio.h>
#include "RawListener.h"

RawListenerImpl::RawListenerImpl() : 
	m_indent(0)
{
}

void RawListenerImpl::__iprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format); 	
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

void RawListenerImpl::__iuprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	__indentUp();
	va_end(args);
}

void RawListenerImpl::__idprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	__indentDown();
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

void RawListenerImpl::setDocumentMetaData(const UCSString &author, const UCSString &subject,
					 const UCSString &publisher, const UCSString &category,
					 const UCSString &keywords, const UCSString &language,
					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType)
{
}

void RawListenerImpl::startDocument()
{
	__iuprintf("startDocument()\n");
}

void RawListenerImpl::endDocument()
{
	__idprintf("endDocument()\n");
}

void RawListenerImpl::openPageSpan(const int span, const bool isLastPageSpan,
				  const float marginLeft, const float marginRight,
				  const float marginTop, const float marginBottom)
{
	__iuprintf("openPageSpan(TODO: add args)\n");
}
						 
void RawListenerImpl::closePageSpan()
{
	__idprintf("closePageSpan()\n");
}
	
void RawListenerImpl::openHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	__iuprintf("openHeaderFooter(TODO: add args)\n");
}

void RawListenerImpl::closeHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	__idprintf("closeHeaderFooter(TODO: add args)\n");
}

void RawListenerImpl::openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits,
				   const float marginLeftOffset, const float marginRightOffset,
				   const gchar *fontName, const float fontSize, 
				   const float lineSpacing, 
				   const bool isColumnBreak, const bool isPageBreak)
{
	__iuprintf("openParagraph(TODO: add args)\n");
}
	
void RawListenerImpl::closeParagraph()
{
	__idprintf("closeParagraph()\n");
}
	
void RawListenerImpl::openSpan(const guint32 textAttributeBits, const gchar *fontName, const float fontSize)
{
	__iuprintf("openSpan(textAttributeBits: %u, fontName: %s, fontSize: %.4f)\n", textAttributeBits, fontName, fontSize);
}
	
void RawListenerImpl::closeSpan()
{
	__idprintf("closeSpan()\n");
}
	
void RawListenerImpl::openSection(const unsigned int numColumns, const float spaceAfter)
{
	__iuprintf("openSection(numColumns: %u, spaceAfter: %.4f)\n", numColumns, spaceAfter);
}
	
void RawListenerImpl::closeSection()
{
	__idprintf("closeSection()\n");
}

void RawListenerImpl::insertTab()
{
	__iprintf("insertTab()\n");
}

void RawListenerImpl::insertText(const UCSString &text)
{
}
	
void RawListenerImpl::insertLineBreak()
{
	__iprintf("insertLineBreak()\n");
}

void RawListenerImpl::defineOrderedListLevel(const int listID, const guint16 listLevel, const WPXNumberingType listType, 
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const int startingNumber)
{
}
						
void RawListenerImpl::defineUnorderedListLevel(const int listID, const guint16 listLevel, const UCSString &bullet)
{
}

void RawListenerImpl::openOrderedListLevel(const int listID)
{
}

void RawListenerImpl::openUnorderedListLevel(const int listID)
{
}

void RawListenerImpl::closeOrderedListLevel()
{
}

void RawListenerImpl::closeUnorderedListLevel()
{
}

void RawListenerImpl::openListElement(const guint8 paragraphJustification, const guint32 textAttributeBits,
				     const float marginLeftOffset, const float marginRightOffset,
				     const gchar *fontName, const float fontSize, 
				     const float lineSpacing)
{
}
					 
void RawListenerImpl::closeListElement()
{
}
	
void RawListenerImpl::openFootnote(int number)
{
	__iuprintf("openFootnote(number: %d)\n", number);
}

void RawListenerImpl::closeFootnote()
{
	__idprintf("closeFootnote()\n");
}

void RawListenerImpl::openEndnote(int number)
{
	__iuprintf("openEndnote(number: %d)\n", number);
}

void RawListenerImpl::closeEndnote()
{
	__idprintf("closeEndnote()\n");
}

void RawListenerImpl::openTable(const guint8 tablePositionBits, 
			       const float marginLeftOffset, const float marginRightOffset,
			       const float leftOffset, const vector < WPXColumnDefinition > &columns)
{
}
				   
void RawListenerImpl::openTableRow()
{
}

void RawListenerImpl::closeTableRow()
{
}

void RawListenerImpl::openTableCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan, 
				   const guint8 borderBits, 
				   const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
}

void RawListenerImpl::closeTableCell()
{
}

void RawListenerImpl::insertCoveredTableCell(const guint32 col, const guint32 row)
{
}
	
void RawListenerImpl::closeTable()
{
}

/*
void RawListener::insertCharacter(const guint16 character)
{
	if (!m_isUndoOn) {
		// first convert from ucs2 to ucs4
		gunichar characterUCS4 = (gunichar) character;
		
		// then convert from ucs4 to utf8 and write it
		gchar *characterUTF8 = g_ucs4_to_utf8(&characterUCS4, 1, NULL, NULL, NULL); // TODO: handle errors
		printf("%s", characterUTF8);
		
		// clean up our mess
		g_free(characterUTF8);
	}
}

void RawListener::undoChange(const guint8 undoType, const guint16 undoLevel)
{
	if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_START) {
		//printf("<UNDO ON>");
		m_isUndoOn = true;
	}
	else if (undoType == WP6_UNDO_GROUP_INVALID_TEXT_END) {
		//printf("<UNDO OFF>");
		m_isUndoOn = false;
	}
}

void RawListener::lineSpacingChange(const float lineSpacing)
{
	printf("<line-spacing:%i>", lineSpacing);

}

void RawListener::justificationChange(const guint8 justification)
{
	printf("<justification:%i>", justification);

}

void RawListener::insertTab(const guint8 tabType)
{
	if (!m_isUndoOn) {
		printf("<TAB: %i>", tabType);
	}
}

void RawListener::insertEOL()
{
	if (!m_isUndoOn) {
		printf("<EOL>\n");
	}
}

void RawListener::insertBreak(const guint8 breakType)
{
	switch(breakType)
	{
	case WPX_PAGE_BREAK:
		printf("\n<HARD PAGE BREAK>\n");
		break;
	case WPX_SOFT_PAGE_BREAK:
		printf("\n<SOFT PAGE BREAK>\n");
		break;
	case WPX_COLUMN_BREAK:
		printf("\n<COLUMN BREAK>\n");
		break;
	}
}

void RawListener::setDate(const guint16 year, const guint8 month, const guint8 day, 
			  const guint8 hour, const guint8 minute, const guint8 second,
			  const guint8 dayOfWeek, const guint8 timeZone, const guint8 unused)
{
	printf("<DATE SET year: %i month: %i day: %i hour: %i minute: %i second: %i dayOfWeek: %i timeZone: %i unused: %i>\n");
}
void RawListener::setExtendedInformation(const guint16 type, const UCSString &data)
{
	UTF8String tempUTF8(data);
	printf("<EXTENDED INFO type: %i data: %s>\n", type, tempUTF8.getUTF8());
	
}
void RawListener::updateOutlineDefinition(const WP6OutlineLocation outlineLocation, const guint16 outlineHash, 
					  const guint8 *numberingMethods, const guint8 tabBehaviourFlag)
{
	if (!m_isUndoOn) {
		printf("<UPDATE OUTLINE DEFINITION (outlineHash:%i, numbering-methods:.., tab-behaviour-flag:%i)>\n", 
		       outlineHash, tabBehaviourFlag);
	}	
}

void RawListener::paragraphNumberOn(const guint16 outlineHash, const guint8 level, const guint8 flag)
{
	if (!m_isUndoOn) {
		printf("<PARAGRAPH NUMBER ON (outlineHash:%i, level:%i, flag:%i)>", 
		       outlineHash, level, flag);
	}	
}

void RawListener::paragraphNumberOff()
{
	if (!m_isUndoOn) {
		printf("<PARAGRAPH NUMBER OFF>");
	}	
}

void RawListener::displayNumberReferenceGroupOn(const guint8 subGroup, const guint8 level)
{
	if (!m_isUndoOn) {
		printf("<DISPLAY NUMBER REF ON (subgroup:%i, level:%i)>", subGroup, level);
	}	
}

void RawListener::displayNumberReferenceGroupOff(const guint8 subGroup)
{
	if (!m_isUndoOn) {
		printf("<DISPLAY NUMBER REF OFF (subgroup:%i)>", subGroup);
	}	
}

void RawListener::styleGroupOn(const guint8 subGroup)
{
	if (!m_isUndoOn) {
		printf("<STYLE GROUP ON (subgroup:%i)>", subGroup);
	}	
}

void RawListener::styleGroupOff(const guint8 subGroup)
{
	if (!m_isUndoOn) {
		printf("<STYLE GROUP OFF (subgroup:%i)>", subGroup);
	}	
}

void RawListener::suppressPageCharacteristics(const guint8 suppressCode)
{
	if (!m_isUndoOn) {
		printf("<SUPPRESS PAGE CHARACTERISTICS (suppressCode: %i)>", suppressCode);
	}	
}

void RawListener::insertCell(const guint8 colSpan, const guint8 rowSpan, const bool boundFromLeft, const bool boundFromAbove, 
			     const guint8 borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	if (!m_isUndoOn) {
		printf("<INSERT CELL (colSpan: %i, rowSpan: %i, borderBits: %x)>", colSpan, rowSpan, borderBits);
	}		
}
*/
