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
	__iprintf("setDocumentMetaData(TODO: add args)\n");
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
	UTF8String tempUTF8(text);
	__iprintf("insertText(%s)\n", tempUTF8.getUTF8());
}
	
void RawListenerImpl::insertLineBreak()
{
	__iprintf("insertLineBreak()\n");
}

void RawListenerImpl::defineOrderedListLevel(const int listID, const guint16 listLevel, const WPXNumberingType listType, 
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const int startingNumber)
{
	__iprintf("defineOrderedListLevel(TODO: add args)\n");
}
						
void RawListenerImpl::defineUnorderedListLevel(const int listID, const guint16 listLevel, const UCSString &bullet)
{
	__iprintf("defineUnorderedListLevel(TODO: add args)\n");
}

void RawListenerImpl::openOrderedListLevel(const int listID)
{
	__iuprintf("openOrderedListLevel(TODO: add args)\n");
}

void RawListenerImpl::openUnorderedListLevel(const int listID)
{
	__iuprintf("openUnorderedListLevel(TODO: add args)\n");
}

void RawListenerImpl::closeOrderedListLevel()
{
	__idprintf("closeOrderedListLevel()\n");
}

void RawListenerImpl::closeUnorderedListLevel()
{
	__idprintf("closeUnorderedListLevel()\n");
}

void RawListenerImpl::openListElement(const guint8 paragraphJustification, const guint32 textAttributeBits,
				     const float marginLeftOffset, const float marginRightOffset,
				     const gchar *fontName, const float fontSize, 
				     const float lineSpacing)
{
	__iuprintf("openListElement(TODO: add args)\n");
}
					 
void RawListenerImpl::closeListElement()
{
	__idprintf("closeListElement()\n");
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
	__iuprintf("openTable(tablePositionBits: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, leftOffset: %.4f, TODO: columns defs.)\n",
		tablePositionBits, marginLeftOffset, marginRightOffset, leftOffset);	
}
				   
void RawListenerImpl::openTableRow()
{
	__iuprintf("openTableRow()\n");
}

void RawListenerImpl::closeTableRow()
{
	__idprintf("closeTableRow()\n");
}

void RawListenerImpl::openTableCell(const guint32 col, const guint32 row, const guint32 colSpan, const guint32 rowSpan, 
				   const guint8 borderBits, 
				   const RGBSColor * cellFgColor, const RGBSColor * cellBgColor)
{
	__iuprintf("openTableCell(TODO: add args)\n");
}

void RawListenerImpl::closeTableCell()
{
	__idprintf("closeTableCell()\n");
}

void RawListenerImpl::insertCoveredTableCell(const guint32 col, const guint32 row)
{
}
	
void RawListenerImpl::closeTable()
{
	__idprintf("closeTable()\n");
}
