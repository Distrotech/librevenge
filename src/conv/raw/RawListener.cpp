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

void RawListenerImpl::setDocumentMetaData(const WPXPropertyList &propList)
{
	if (m_printCallgraphScore)
		return;

	__iprintf("setDocumentMetaData(");

	WPXPropertyList::Iter i(propList);
	if (!i.last()) 
	{
		printf("%s: %s", i.key().c_str(), i()->getStr().getUTF8());
		for (i; i.next(); )
		{
			printf(", %s: %s", i.key().c_str(), i()->getStr().getUTF8());
		}
	}
	printf(")\n");
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

void RawListenerImpl::openPageSpan(const WPXPropertyList &propList)
{
	_U(("openPageSpan(span: %d, isLastPageSpan: %s, formLength: %.4f, formWidth: %.4f, Orientation: %s, marginLeft: %.4f, marginRight: %.4f, marginTop: %.4f, marginBottom: %.4f\n",
	    propList["num-pages"]->getInt(), (propList["is-last-page-span"]->getInt() ? "true" : "false"), propList["page-height"]->getFloat(),
	    propList["page-width"]->getFloat(), 
	    (((WPXFormOrientation)propList["print-orientation"]->getInt()==LANDSCAPE) ? "landscape" : "portrait"), 
	    propList["margin-left"]->getFloat(), propList["margin-right"]->getFloat(), 
	    propList["margin-top"]->getFloat(), propList["margin-bottom"]->getFloat()),
	   LC_OPEN_PAGE_SPAN);
}

void RawListenerImpl::closePageSpan()
{
	_D(("closePageSpan()\n"),
		LC_OPEN_PAGE_SPAN);
}

void RawListenerImpl::openHeaderFooter(const WPXPropertyList &propList)
{
	_U(("openHeaderFooter(headerFooterType: %d, headerFooterOccurence: %d)\n",
	    propList["type"]->getInt(), propList["occurence"]->getInt()),
	   LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::closeHeaderFooter(const WPXPropertyList &propList)
{
	_D(("closeHeaderFooter(headerFooterType: %d, headerFooterOccurence: %d)\n",
	    propList["type"]->getInt(), propList["occurence"]->getInt()),
	   LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::openParagraph(const WPXPropertyList &propList, const vector<WPXTabStop> &tabStops)
{
	_U(("openParagraph(paragraphJustification: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, textIndent: %.4f, lineSpacing: %.4f, spacingBeforeParagraph: %.4f, spacingAfterParagraph: %.4f, isColumnBreak: %s, isPageBreak: %s, TODO: tab-stops.)\n",
	    propList["justification"]->getInt(), propList["margin-left"]->getFloat(), propList["margin-right"]->getFloat(),
	    propList["text-indent"]->getFloat(), propList["line-spacing"]->getFloat(), propList["margin-top"]->getFloat(),
	    propList["margin-bottom"]->getFloat(), (propList["column-break"]->getInt() ? "true" : "false"),
	    (propList["page-break"]->getInt() ? "true" : "false")),
	   LC_OPEN_PARAGRAPH);
}

void RawListenerImpl::closeParagraph()
{
	_D(("closeParagraph()\n"),
		LC_OPEN_PARAGRAPH);
}

void RawListenerImpl::openSpan(const WPXPropertyList &propList)
{
	_U(("openSpan(textAttributeBits: %u, fontName: %s, fontSize: %.4f, fontColor: %s, highlightColor: %s)\n",
	    propList["text-attribute-bits"]->getInt(), propList["font-name"]->getStr().getUTF8(), propList["font-size"]->getFloat(),
	    propList["color"]->getStr().getUTF8(), propList["text-background-color"]->getStr().getUTF8()), 
	   LC_OPEN_SPAN);
}

void RawListenerImpl::closeSpan()
{
	_D(("closeSpan()\n"),
	   LC_OPEN_SPAN);
}

void RawListenerImpl::openSection(const WPXPropertyList &propList, const vector<WPXColumnDefinition> &columns)
{
	UTF8String sColumns;
	sColumns.sprintf("");
	if (propList["num-columns"]->getInt() > 1)
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
	_U(("openSection(numColumns: %u, columns:%s, spaceAfter: %.4f)\n", propList["num-columns"]->getInt(), sColumns.getUTF8(), 
	    propList["margin-bottom"]->getFloat()),
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

void RawListenerImpl::defineOrderedListLevel(const WPXPropertyList &propList)
{
	__iprintf("defineOrderedListLevel(listID: %d, listLevel: %d, listType: %d, textBeforeNumber: %s, textAfterNumber: %s, startingNumber: %d)\n",
		  propList["id"]->getInt(), propList["level"]->getInt(), propList["type"]->getInt(), 
		  propList["text-before-number"]->getStr().getUTF8(), propList["text-after-number"]->getStr().getUTF8(),
		  propList["starting-number"]->getInt());
}

void RawListenerImpl::defineUnorderedListLevel(const WPXPropertyList &propList)
{
	__iprintf("defineUnorderedListLevel(listID: %d, listLevel: %d, bullet: %s)\n", propList["id"]->getInt(), propList["level"]->getInt(), 
		  propList["bullet"]->getStr().getUTF8());
}

void RawListenerImpl::openOrderedListLevel(const WPXPropertyList &propList)
{
	_U(("openOrderedListLevel(listID: %d)\n", propList["id"]->getInt()),
		LC_OPEN_ORDERED_LIST_LEVEL);
}

void RawListenerImpl::openUnorderedListLevel(const WPXPropertyList &propList)
{
	_U(("openUnorderedListLevel(listID: %d)\n", propList["id"]->getInt()),
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

void RawListenerImpl::openListElement(const WPXPropertyList &propList, const vector<WPXTabStop> &tabStops)
{
	_U(("openListElement(paragraphJustification: %d, marginLeftOffset: %.4f, marginRightOffset: %.4f, textIndent: %.4f, lineSpacing: %.4f, spacingBeforeParagraph: %.4f, spacingAfterParagraph: %.4f, TODO: tab-stops.)\n",
	    propList["justification"]->getInt(), propList["margin-left"]->getFloat(), propList["margin-right"]->getFloat(),
	    propList["text-indent"]->getFloat(), propList["line-spacing"]->getFloat(), propList["margin-top"]->getFloat(),
	    propList["margin-bottom"]->getFloat()),
	   LC_OPEN_LIST_ELEMENT);
}

void RawListenerImpl::closeListElement()
{
	_D(("closeListElement()\n"),
		LC_OPEN_LIST_ELEMENT);
}

void RawListenerImpl::openFootnote(const WPXPropertyList &propList)
{
	_U(("openFootnote(number: %d)\n", propList["number"]->getInt()),
		LC_OPEN_FOOTNOTE);
}

void RawListenerImpl::closeFootnote()
{
	_D(("closeFootnote()\n"),
		LC_OPEN_FOOTNOTE);
}

void RawListenerImpl::openEndnote(const WPXPropertyList &propList)
{
	_U(("openEndnote(number: %d)\n", propList["number"]->getInt()),
		LC_OPEN_ENDNOTE);
}

void RawListenerImpl::closeEndnote()
{
	_D(("closeEndnote()\n"),
		LC_OPEN_ENDNOTE);
}

void RawListenerImpl::openTable(const WPXPropertyList &propList, const vector < WPXColumnDefinition > &columns)
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
	    propList["alignment"]->getInt(), propList["margin-left"]->getFloat(), propList["margin-right"]->getFloat(), 
	    propList["left-offset"]->getFloat(), sColumns.getUTF8()),
	   LC_OPEN_TABLE);
}

void RawListenerImpl::openTableRow(const WPXPropertyList &propList)
{
	_U(("openTableRow(height: %.4f, isMinimumHeight: %s, isHeaderRow: %s)\n", propList["height"]->getFloat(),
	    (propList["is-minimum-height"]->getInt() ? "true" : "false"), 
	    (propList["is-header-row"]->getInt() ? "true" : "false")),
	   LC_OPEN_TABLE_ROW);
}

void RawListenerImpl::closeTableRow()
{
	_D(("closeTableRow()\n"),
		LC_OPEN_TABLE_ROW);
}

void RawListenerImpl::openTableCell(const WPXPropertyList &propList)
{
	UTF8String sCellVerticalAlignment;
	switch ((WPXVerticalAlignment)propList["vertical-alignment"]->getInt())
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
			
	_U(("openTableCell(col: %d, row: %d, colSpan: %d, rowSpan: %d, borderBits: %d, cellColor: %s, cellBorderColor: %s, cellVerticalAlignment %s)\n",
	    propList["col"]->getInt(), propList["row"]->getInt(), propList["col-span"]->getInt(), propList["row-span"]->getInt(), propList["border-bits"]->getInt(),
    	    propList["color"]->getStr().getUTF8(), propList["border-color"]->getStr().getUTF8(),
	    sCellVerticalAlignment.getUTF8()),
	   LC_OPEN_TABLE_CELL);
}

void RawListenerImpl::closeTableCell()
{
	_D(("closeTableCell()\n"),
	   LC_OPEN_TABLE_CELL);
}

void RawListenerImpl::insertCoveredTableCell(const WPXPropertyList &propList)
{
	__iprintf("insertCoveredTableCell(col: %d, row: %d)\n", propList["col"]->getInt(), propList["row"]->getInt());
}

void RawListenerImpl::closeTable()
{
	_D(("closeTable()\n"),
		LC_OPEN_TABLE);
}
