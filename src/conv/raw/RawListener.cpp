/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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

RawListenerImpl::RawListenerImpl(bool printIndentLevel) :
	m_indent(0),
	m_actualIndentLevel(0),
	m_printIndentLevel(printIndentLevel)
{
}

RawListenerImpl::~RawListenerImpl()
{
	if (m_printIndentLevel)
		printf("%d\n", m_actualIndentLevel);
}

void RawListenerImpl::__iprintf(const char *format, ...)
{
	if (m_printIndentLevel) return;

	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

void RawListenerImpl::__iuprintf(const char *format, ...)
{
	if (m_printIndentLevel) return;

	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	__indentUp();
	va_end(args);

	m_actualIndentLevel++;
}

void RawListenerImpl::__idprintf(const char *format, ...)
{
	if (m_printIndentLevel) return;

	va_list args;
	va_start(args, format);
	__indentDown();
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);

	m_actualIndentLevel--;
}

void RawListenerImpl::setDocumentMetaData(const UCSString &author, const UCSString &subject,
					 const UCSString &publisher, const UCSString &category,
					 const UCSString &keywords, const UCSString &language,
					 const UCSString &abstract, const UCSString &descriptiveName,
					 const UCSString &descriptiveType)
{
	UTF8String authorUTF8(author);
	UTF8String subjectUTF8(subject);
	UTF8String publisherUTF8(publisher);
	UTF8String categoryUTF8(category);
	UTF8String keywordsUTF8(keywords);
	UTF8String languageUTF8(language);
	UTF8String abstractUTF8(abstract);
	UTF8String descriptiveNameUTF8(descriptiveName);
	UTF8String descriptiveTypeUTF8(descriptiveType);
	__iprintf("setDocumentMetaData(author: %s, subject: %s, publisher: %s, category: %s, keywords: %s, language: %s, abstract: %s, descriptiveName: %s, descriptiveType: %s)\n",
		authorUTF8.getUTF8(), subjectUTF8.getUTF8(),
		publisherUTF8.getUTF8(), categoryUTF8.getUTF8(),
		keywordsUTF8.getUTF8(), languageUTF8.getUTF8(),
		abstractUTF8.getUTF8(), descriptiveNameUTF8.getUTF8(),
		descriptiveTypeUTF8.getUTF8()
	);
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
				  const float formLength, const float formWidth, const WPXFormOrientation orientation,
				  const float marginLeft, const float marginRight,
				  const float marginTop, const float marginBottom)
{
	__iuprintf("openPageSpan(span: %d, isLastPageSpan: %s, formLength: %.4f, formWidth: %.4f, Orientation: %s, marginLeft: %.4f, marginRight: %.4f, marginTop: %.4f, marginBottom: %.4f\n",
				span, (isLastPageSpan ? "true" : "false"), formLength, formWidth, ((orientation==LANDSCAPE) ? "landscape" : "portrait"), marginLeft, marginRight, marginTop, marginBottom);
}

void RawListenerImpl::closePageSpan()
{
	__idprintf("closePageSpan()\n");
}

void RawListenerImpl::openHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	__iuprintf("openHeaderFooter(headerFooterType: %d, headerFooterOccurence: %d)\n",
		headerFooterType, headerFooterOccurence
	);
}

void RawListenerImpl::closeHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	__idprintf("closeHeaderFooter(headerFooterType: %d, headerFooterOccurence: %d)\n",
		headerFooterType, headerFooterOccurence
	);
}

void RawListenerImpl::openParagraph(const guint8 paragraphJustification, const guint32 textAttributeBits,
				   const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				   const gchar *fontName, const float fontSize, const RGBSColor *fontColor,
				   const RGBSColor *highlightColor, const float lineSpacing,
				   const bool isColumnBreak, const bool isPageBreak)
{
	UTF8String fontNameUTF8(fontName);
	__iuprintf("openParagraph(paragraphJustification: %d, textAttributeBits: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, textIndent: %.4f, fontName: %s, fontSize: %.4f,  fontColor: #%02x%02x%02x s:%02x, highlightColor: #%02x%02x%02x s:%02x, lineSpacing: %.4f, isColumnBreak: %s, isPageBreak: %s)\n",
		paragraphJustification, textAttributeBits,
		marginLeftOffset, marginRightOffset, textIndent,
		fontNameUTF8.getUTF8(), fontSize, (fontColor?fontColor->m_r:0xff), (fontColor?fontColor->m_g:0xff),
		(fontColor?fontColor->m_b:0xff), (fontColor?fontColor->m_s:0xff), (highlightColor?highlightColor->m_r:0xff),
		(highlightColor?highlightColor->m_g:0xff), (highlightColor?highlightColor->m_b:0xff),
		(highlightColor?highlightColor->m_s:0xff), // saturation cannot be ever 0xff; if it is, the pointer is NULL.
		lineSpacing, (isColumnBreak ? "true" : "false"), (isPageBreak ? "true" : "false")
	);
}

void RawListenerImpl::closeParagraph()
{
	__idprintf("closeParagraph()\n");
}

void RawListenerImpl::openSpan(const guint32 textAttributeBits, const gchar *fontName, const float fontSize,
					const RGBSColor *fontColor, const RGBSColor *highlightColor)
{
	__iuprintf("openSpan(textAttributeBits: %u, fontName: %s, fontSize: %.4f,  fontColor: #%02x%02x%02x s:%02x, highlightColor: #%02x%02x%02x s:%02x)\n",
			   textAttributeBits, fontName, fontSize, (fontColor?fontColor->m_r:0xff), (fontColor?fontColor->m_g:0xff),
			   (fontColor?fontColor->m_b:0xff), (fontColor?fontColor->m_s:0xff), (highlightColor?highlightColor->m_r:0xff),
			   (highlightColor?highlightColor->m_g:0xff), (highlightColor?highlightColor->m_b:0xff),
			   (highlightColor?highlightColor->m_s:0xff) // saturation cannot be ever 0xff; if it is, the pointer is NULL
	);
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
	UTF8String textUTF8(text);
	__iprintf("insertText(text: %s)\n", textUTF8.getUTF8());
}

void RawListenerImpl::insertLineBreak()
{
	__iprintf("insertLineBreak()\n");
}

void RawListenerImpl::defineOrderedListLevel(const int listID, const guint16 listLevel, const WPXNumberingType listType,
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const int startingNumber)
{
	UTF8String textBeforeNumberUTF8(textBeforeNumber);
	UTF8String textAfterNumberUTF8(textAfterNumber);
	__iprintf("defineOrderedListLevel(listID: %d, listLevel: %d, listType: %d, textBeforeNumber: %s, textAfterNumber: %s, startingNumber: %d)\n",
		listID, listLevel, listType,
		textBeforeNumberUTF8.getUTF8(), textAfterNumberUTF8.getUTF8(),
		startingNumber
	);
}

void RawListenerImpl::defineUnorderedListLevel(const int listID, const guint16 listLevel, const UCSString &bullet)
{
	UTF8String bulletUTF8(bullet);
	__iprintf("defineUnorderedListLevel(listID: %d, listLevel: %d, bullet: %s)\n", listID, listLevel, bulletUTF8.getUTF8());
}

void RawListenerImpl::openOrderedListLevel(const int listID)
{
	__iuprintf("openOrderedListLevel(listID: %d)\n", listID);
}

void RawListenerImpl::openUnorderedListLevel(const int listID)
{
	__iuprintf("openUnorderedListLevel(listID: %d)\n", listID);
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
				   const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				   const gchar *fontName, const float fontSize, const RGBSColor *fontColor,
				   const RGBSColor *highlightColor, const float lineSpacing)
{
	UTF8String fontNameUTF8(fontName);
	__iuprintf("openListElement(paragraphJustification: %d, textAttributeBits: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, textIndent: %.4f, fontName: %s, fontSize: %.4f, fontColor: #%02x%02x%02x s:%02x, highlightColor: #%02x%02x%02x s:%02x, lineSpacing: %.4f)\n",
		paragraphJustification, textAttributeBits,
		marginLeftOffset, marginRightOffset, textIndent,
		fontNameUTF8.getUTF8(), fontSize, (fontColor?fontColor->m_r:0xff), (fontColor?fontColor->m_g:0xff),
		(fontColor?fontColor->m_b:0xff), (fontColor?fontColor->m_s:0xff), (highlightColor?highlightColor->m_r:0xff),
		(highlightColor?highlightColor->m_g:0xff), (highlightColor?highlightColor->m_b:0xff),
	    (highlightColor?highlightColor->m_s:0xff), // saturation cannot be ever 0xff; if it is, the pointer is NULL.
	    lineSpacing
	);
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
	__iuprintf("openTableCell(col: %d, row: %d, colSpan: %d, rowSpan: %d, borderBits: %d, cellFgColor: #%02x%02x%02x s:%02x, cellBgColor: #%02x%02x%02x s:%02x)\n",
		col, row, colSpan, rowSpan,
		borderBits,
		// The saturation cannot ever be more that 0x64. It it is, cellFgColor or cellBgColor is NULL
		(cellFgColor?cellFgColor->m_r:0xff), (cellFgColor?cellFgColor->m_g:0xff), (cellFgColor?cellFgColor->m_b:0xff), (cellFgColor?cellFgColor->m_s:0xff),
		(cellBgColor?cellBgColor->m_r:0xff), (cellBgColor?cellBgColor->m_g:0xff), (cellBgColor?cellBgColor->m_b:0xff), (cellBgColor?cellBgColor->m_s:0xff)
	);
}

void RawListenerImpl::closeTableCell()
{
	__idprintf("closeTableCell()\n");
}

void RawListenerImpl::insertCoveredTableCell(const guint32 col, const guint32 row)
{
	__iprintf("insertCoveredTableCell(col: %d, row: %d)\n", col, row);
}

void RawListenerImpl::closeTable()
{
	__idprintf("closeTable()\n");
}
