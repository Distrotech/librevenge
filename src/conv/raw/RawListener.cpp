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

#define _U(M, L) \
	if (!m_printCallgraphScore) \
			__iuprintf M; \
	else \
		m_callStack.push(L);

#define _D(M, L) \
	if (!m_printCallgraphScore) \
			__idprintf M; \
	else \
	{ \
		ListenerCallback lc = m_callStack.top(); \
		if (lc != L) \
			m_callbackMisses++; \
		m_callStack.pop(); \
	}	
	
RawListenerImpl::RawListenerImpl(bool printCallgraphScore) :
	m_indent(0),
	m_callbackMisses(0),
	m_printCallgraphScore(printCallgraphScore)
{
}

RawListenerImpl::~RawListenerImpl()
{
	if (m_printCallgraphScore)
		printf("%d\n", m_callStack.size() + m_callbackMisses);
}

void RawListenerImpl::__iprintf(const char *format, ...)
{
	if (m_printCallgraphScore) return;
	
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
	_U(("startDocument()\n"), LC_START_DOCUMENT);
}

void RawListenerImpl::endDocument()
{
	_D(("endDocument()\n"), 
		LC_START_DOCUMENT);
}

void RawListenerImpl::openPageSpan(const int span, const bool isLastPageSpan,
				  const float formLength, const float formWidth, const WPXFormOrientation orientation,
				  const float marginLeft, const float marginRight,
				  const float marginTop, const float marginBottom)
{
	_U(("openPageSpan(span: %d, isLastPageSpan: %s, formLength: %.4f, formWidth: %.4f, Orientation: %s, marginLeft: %.4f, marginRight: %.4f, marginTop: %.4f, marginBottom: %.4f\n",
			span, (isLastPageSpan ? "true" : "false"), formLength, formWidth, ((orientation==LANDSCAPE) ? "landscape" : "portrait"), marginLeft, marginRight, marginTop, marginBottom),
		LC_OPEN_PAGE_SPAN);
}

void RawListenerImpl::closePageSpan()
{
	_D(("closePageSpan()\n"),
		LC_OPEN_PAGE_SPAN);
}

void RawListenerImpl::openHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	_U(("openHeaderFooter(headerFooterType: %d, headerFooterOccurence: %d)\n",
		headerFooterType, headerFooterOccurence),
		LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::closeHeaderFooter(const WPXHeaderFooterType headerFooterType, const WPXHeaderFooterOccurence headerFooterOccurence)
{
	_D(("closeHeaderFooter(headerFooterType: %d, headerFooterOccurence: %d)\n",
			headerFooterType, headerFooterOccurence),
		LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::openParagraph(const uint8_t paragraphJustification, 
				    const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				    const float lineSpacing, const float spacingBeforeParagraph, const float spacingAfterParagraph,
				    const vector<WPXTabStop> &tabStops, const bool isColumnBreak, const bool isPageBreak)
{
	_U(("openParagraph(paragraphJustification: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, textIndent: %.4f, lineSpacing: %.4f, spacingBeforeParagraph: %.4f, spacingAfterParagraph: %.4f, isColumnBreak: %s, isPageBreak: %s, TODO: tab-stops.)\n",
			paragraphJustification, 
			marginLeftOffset, marginRightOffset, textIndent, lineSpacing, spacingBeforeParagraph, spacingAfterParagraph,
			(isColumnBreak ? "true" : "false"), (isPageBreak ? "true" : "false")),
		LC_OPEN_PARAGRAPH);
}

void RawListenerImpl::closeParagraph()
{
	_D(("closeParagraph()\n"),
		LC_OPEN_PARAGRAPH);
}

void RawListenerImpl::openSpan(const uint32_t textAttributeBits, const char *fontName, const float fontSize,
					const RGBSColor *fontColor, const RGBSColor *highlightColor)
{
	_U(("openSpan(textAttributeBits: %u, fontName: %s, fontSize: %.4f,  fontColor: #%02x%02x%02x s:%02x, highlightColor: #%02x%02x%02x s:%02x)\n",
			textAttributeBits, fontName, fontSize, (fontColor?fontColor->m_r:0xff), (fontColor?fontColor->m_g:0xff),
			(fontColor?fontColor->m_b:0xff), (fontColor?fontColor->m_s:0xff), (highlightColor?highlightColor->m_r:0xff),
			(highlightColor?highlightColor->m_g:0xff), (highlightColor?highlightColor->m_b:0xff),
			(highlightColor?highlightColor->m_s:0xff)), // saturation cannot be ever 0xff; if it is, the pointer is NULL
		LC_OPEN_SPAN);
}

void RawListenerImpl::closeSpan()
{
	_D(("closeSpan()\n"),
		LC_OPEN_SPAN);
}

void RawListenerImpl::openSection(const unsigned int numColumns, const vector<WPXColumnDefinition> &columns, const float spaceAfter)
{
	UTF8String sColumns;
	sColumns.sprintf("");
	if (numColumns > 1)
	{
		for (int i=0; i<columns.size(); i++)
		{
			sColumns.sprintf("%s W:%.4f|", sColumns.getUTF8(), columns[i].m_width);
			sColumns.sprintf("%sL:%.4f|", sColumns.getUTF8(), columns[i].m_leftGutter);
			sColumns.sprintf("%sR:%.4f", sColumns.getUTF8(), columns[i].m_rightGutter);
		}
	}
	else
		sColumns.sprintf(" SINGLE COLUMN");
	_U(("openSection(numColumns: %u, columns:%s, spaceAfter: %.4f)\n", numColumns, sColumns.getUTF8(), spaceAfter),
		LC_OPEN_SECTION);
}

void RawListenerImpl::closeSection()
{
	_D(("closeSection()\n"),
		LC_OPEN_SECTION);
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

void RawListenerImpl::defineOrderedListLevel(const int listID, const int listLevel, const WPXNumberingType listType,
					    const UCSString &textBeforeNumber, const UCSString &textAfterNumber,
					    const int startingNumber)
{
	UTF8String textBeforeNumberUTF8(textBeforeNumber);
	UTF8String textAfterNumberUTF8(textAfterNumber);
	__iprintf("defineOrderedListLevel(listID: %d, listLevel: %d, listType: %d, textBeforeNumber: %s, textAfterNumber: %s, startingNumber: %d)\n",
		listID, listLevel, listType,
		textBeforeNumberUTF8.getUTF8(), textAfterNumberUTF8.getUTF8(),
		startingNumber);
}

void RawListenerImpl::defineUnorderedListLevel(const int listID, const int listLevel, const UCSString &bullet)
{
	UTF8String bulletUTF8(bullet);
	__iprintf("defineUnorderedListLevel(listID: %d, listLevel: %d, bullet: %s)\n", listID, listLevel, bulletUTF8.getUTF8());
}

void RawListenerImpl::openOrderedListLevel(const int listID)
{
	_U(("openOrderedListLevel(listID: %d)\n", listID),
		LC_OPEN_ORDERED_LIST_LEVEL);
}

void RawListenerImpl::openUnorderedListLevel(const int listID)
{
	_U(("openUnorderedListLevel(listID: %d)\n", listID),
		LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RawListenerImpl::closeOrderedListLevel()
{
	_D(("closeOrderedListLevel()\n"),
		LC_OPEN_ORDERED_LIST_LEVEL);
}

void RawListenerImpl::closeUnorderedListLevel()
{
	_D(("closeUnorderedListLevel()\n"),
		LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RawListenerImpl::openListElement(const uint8_t paragraphJustification, 
				      const float marginLeftOffset, const float marginRightOffset, const float textIndent,
				      const float lineSpacing, const float spacingBeforeParagraph, const float spacingAfterParagraph, const vector<WPXTabStop> &tabStops)
{
	_U(("openListElement(paragraphJustification: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, textIndent: %.4f, lineSpacing: %.4f, spacingBeforeParagraph: %.4f, spacingAfterParagraph: %.4f, TODO: tab-stops.)\n",
			paragraphJustification, 
			marginLeftOffset, marginRightOffset, textIndent,
			lineSpacing, spacingBeforeParagraph, spacingAfterParagraph),
		LC_OPEN_LIST_ELEMENT);
}

void RawListenerImpl::closeListElement()
{
	_D(("closeListElement()\n"),
		LC_OPEN_LIST_ELEMENT);
}

void RawListenerImpl::openFootnote(int number)
{
	_U(("openFootnote(number: %d)\n", number),
		LC_OPEN_FOOTNOTE);
}

void RawListenerImpl::closeFootnote()
{
	_D(("closeFootnote()\n"),
		LC_OPEN_FOOTNOTE);
}

void RawListenerImpl::openEndnote(int number)
{
	_U(("openEndnote(number: %d)\n", number),
		LC_OPEN_ENDNOTE);
}

void RawListenerImpl::closeEndnote()
{
	_D(("closeEndnote()\n"),
		LC_OPEN_ENDNOTE);
}

void RawListenerImpl::openTable(const uint8_t tablePositionBits,
			       const float marginLeftOffset, const float marginRightOffset,
			       const float leftOffset, const vector < WPXColumnDefinition > &columns)
{
	UTF8String sColumns;
	sColumns.sprintf("");
	for (int i=0; i<columns.size(); i++)
	{
		sColumns.sprintf("%s W:%.4f|", sColumns.getUTF8(), columns[i].m_width);
		sColumns.sprintf("%sL:%.4f|", sColumns.getUTF8(), columns[i].m_leftGutter);
		sColumns.sprintf("%sR:%.4f", sColumns.getUTF8(), columns[i].m_rightGutter);
	}

	_U(("openTable(tablePositionBits: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, leftOffset: %.4f, columns:%s.)\n",
			tablePositionBits, marginLeftOffset, marginRightOffset, leftOffset, sColumns.getUTF8()),
		LC_OPEN_TABLE);
}

void RawListenerImpl::openTableRow(const float height, const bool isMinimumHeight, const bool isHeaderRow)
{
	_U(("openTableRow(height: %.4f, isMinimumHeight: %s, isHeaderRow: %s)\n", height, (isMinimumHeight ? "true" : "false"), (isHeaderRow ? "true" : "false")),
		LC_OPEN_TABLE_ROW);
}

void RawListenerImpl::closeTableRow()
{
	_D(("closeTableRow()\n"),
		LC_OPEN_TABLE_ROW);
}

void RawListenerImpl::openTableCell(const uint32_t col, const uint32_t row, const uint32_t colSpan, const uint32_t rowSpan,
				   const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				   const RGBSColor * cellBorderColor,
				   const WPXVerticalAlignment cellVerticalAlignment)
{
	UTF8String sCellVerticalAlignment;
	switch (cellVerticalAlignment)
	{
	case TOP:
		sCellVerticalAlignment.sprintf("TOP");
		break;
	case MIDDLE:
		sCellVerticalAlignment.sprintf("MIDDLE");
		break;
	case BOTTOM:
		sCellVerticalAlignment.sprintf("BOTTOM");
		break;
	case FULL:
		sCellVerticalAlignment.sprintf("FULL");
		break;
	default:
		break;
	}
			
	_U(("openTableCell(col: %d, row: %d, colSpan: %d, rowSpan: %d, borderBits: %d, cellFgColor: #%02x%02x%02x s:%02x, cellBgColor: #%02x%02x%02x s:%02x, cellBorderColor: #%02x%02x%02x s:%02x, cellVerticalAlignment %s)\n",
			col, row, colSpan, rowSpan, borderBits,
			// The saturation cannot ever be more that 0x64. It it is, cellFgColor or cellBgColor is NULL
			(cellFgColor?cellFgColor->m_r:0xff), (cellFgColor?cellFgColor->m_g:0xff), (cellFgColor?cellFgColor->m_b:0xff), (cellFgColor?cellFgColor->m_s:0xff),
			(cellBgColor?cellBgColor->m_r:0xff), (cellBgColor?cellBgColor->m_g:0xff), (cellBgColor?cellBgColor->m_b:0xff), (cellBgColor?cellBgColor->m_s:0xff),
			cellBorderColor->m_r, cellBorderColor->m_g, cellBorderColor->m_b, cellBorderColor->m_s,
			sCellVerticalAlignment.getUTF8()),
		LC_OPEN_TABLE_CELL);
}

void RawListenerImpl::closeTableCell()
{
	_D(("closeTableCell()\n"),
		LC_OPEN_TABLE_CELL);
}

void RawListenerImpl::insertCoveredTableCell(const uint32_t col, const uint32_t row)
{
	__iprintf("insertCoveredTableCell(col: %d, row: %d)\n", col, row);
}

void RawListenerImpl::closeTable()
{
	_D(("closeTable()\n"),
		LC_OPEN_TABLE);
}
