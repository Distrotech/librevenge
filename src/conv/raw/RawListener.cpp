/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <stdio.h>
#include <stdarg.h>
#include "RawListener.h"

#ifdef _U
#undef _U
#endif

#define _U(M, L) \
	if (!m_printCallgraphScore) \
			__iuprintf M; \
	else \
		m_callStack.push(L);

#ifdef _D
#undef _D
#endif

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
	m_printCallgraphScore(printCallgraphScore),
	m_callStack()
{
}

RawListenerImpl::~RawListenerImpl()
{
	if (m_printCallgraphScore)
		printf("%d\n", (int)(m_callStack.size() + m_callbackMisses));
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

WPXString getPropString(const WPXPropertyList &propList)
{
	WPXString propString;
	WPXPropertyList::Iter i(propList);
	if (!i.last()) 
	{
		propString.append(i.key());
		propString.append(": ");
		propString.append(i()->getStr().cstr());
		for (; i.next(); )
		{
			propString.append(", ");
			propString.append(i.key());
			propString.append(": ");
			propString.append(i()->getStr().cstr());
		}
	}

	return propString;
}

WPXString getPropString(const WPXPropertyListVector &itemList)
{
	WPXString propString;

	propString.append("(");
	WPXPropertyListVector::Iter i(itemList);

	if (!i.last())
	{
		propString.append("(");
		propString.append(getPropString(i()));
		propString.append(")");

		for (; i.next();)
		{
			propString.append(", (");
			propString.append(getPropString(i()));
			propString.append(")");
		}

	}
	propString.append(")");

	return propString;
}

void RawListenerImpl::setDocumentMetaData(const WPXPropertyList &propList)
{
	if (m_printCallgraphScore)
		return;

	__iprintf("setDocumentMetaData(%s)\n", getPropString(propList).cstr());
}

void RawListenerImpl::startDocument()
{
	_U(("startDocument()\n"), LC_START_DOCUMENT);
}

void RawListenerImpl::endDocument()
{
	_D(("endDocument()\n"), LC_START_DOCUMENT);
}

void RawListenerImpl::openPageSpan(const WPXPropertyList &propList)
{
	_U(("openPageSpan(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_PAGE_SPAN);
}

void RawListenerImpl::closePageSpan()
{
	_D(("closePageSpan()\n"),
		LC_OPEN_PAGE_SPAN);
}

void RawListenerImpl::openHeader(const WPXPropertyList &propList)
{
	_U(("openHeader(%s)\n",
	    getPropString(propList).cstr()),
	   LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::closeHeader()
{
	_D(("closeHeader()\n"),
	   LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::openFooter(const WPXPropertyList &propList)
{
	_U(("openFooter(%s)\n",
	    getPropString(propList).cstr()),
	   LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::closeFooter()
{
	_D(("closeFooter()\n"),
	   LC_OPEN_HEADER_FOOTER);
}

void RawListenerImpl::openParagraph(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops)
{
	_U(("openParagraph(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()),
	   LC_OPEN_PARAGRAPH);
}

void RawListenerImpl::closeParagraph()
{
	_D(("closeParagraph()\n"), LC_OPEN_PARAGRAPH);
}

void RawListenerImpl::openSpan(const WPXPropertyList &propList)
{
	_U(("openSpan(%s)\n", getPropString(propList).cstr()), LC_OPEN_SPAN);
}

void RawListenerImpl::closeSpan()
{
	_D(("closeSpan()\n"), LC_OPEN_SPAN);
}

void RawListenerImpl::openSection(const WPXPropertyList &propList, const WPXPropertyListVector &columns)
{
	_U(("openSection(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_SECTION);
}

void RawListenerImpl::closeSection()
{
	_D(("closeSection()\n"), LC_OPEN_SECTION);
}

void RawListenerImpl::insertTab()
{
	__iprintf("insertTab()\n");
}

void RawListenerImpl::insertText(const WPXString &text)
{
	WPXString textUTF8(text);
	__iprintf("insertText(text: %s)\n", textUTF8.cstr());
}

void RawListenerImpl::insertLineBreak()
{
	__iprintf("insertLineBreak()\n");
}

void RawListenerImpl::defineOrderedListLevel(const WPXPropertyList &propList)
{
	__iprintf("defineOrderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RawListenerImpl::defineUnorderedListLevel(const WPXPropertyList &propList)
{
	__iprintf("defineUnorderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RawListenerImpl::openOrderedListLevel(const WPXPropertyList &propList)
{
	_U(("openOrderedListLevel(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_ORDERED_LIST_LEVEL);
}

void RawListenerImpl::openUnorderedListLevel(const WPXPropertyList &propList)
{
	_U(("openUnorderedListLevel(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RawListenerImpl::closeOrderedListLevel()
{
	_D(("closeOrderedListLevel()\n"),
	   LC_OPEN_ORDERED_LIST_LEVEL);
}

void RawListenerImpl::closeUnorderedListLevel()
{
	_D(("closeUnorderedListLevel()\n"), LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RawListenerImpl::openListElement(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops)
{
	_U(("openListElement(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()), 
	   LC_OPEN_LIST_ELEMENT);
}

void RawListenerImpl::closeListElement()
{
	_D(("closeListElement()\n"), LC_OPEN_LIST_ELEMENT);
}

void RawListenerImpl::openFootnote(const WPXPropertyList &propList)
{
	_U(("openFootnote(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_FOOTNOTE);
}

void RawListenerImpl::closeFootnote()
{
	_D(("closeFootnote()\n"), LC_OPEN_FOOTNOTE);
}

void RawListenerImpl::openEndnote(const WPXPropertyList &propList)
{
	_U(("openEndnote(number: %s)\n", getPropString(propList).cstr()),
	   LC_OPEN_ENDNOTE);
}

void RawListenerImpl::closeEndnote()
{
	_D(("closeEndnote()\n"), LC_OPEN_ENDNOTE);
}

void RawListenerImpl::openComment(const WPXPropertyList &propList)
{
	_U(("openComment(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_COMMENT);
}

void RawListenerImpl::closeComment()
{
	_D(("closeComment()\n"), LC_OPEN_COMMENT);
}

void RawListenerImpl::openTable(const WPXPropertyList &propList, const WPXPropertyListVector &columns)
{
	_U(("openTable(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_TABLE);
}

void RawListenerImpl::openTableRow(const WPXPropertyList &propList)
{
	_U(("openTableRow(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_TABLE_ROW);
}

void RawListenerImpl::closeTableRow()
{
	_D(("closeTableRow()\n"), LC_OPEN_TABLE_ROW);
}

void RawListenerImpl::openTableCell(const WPXPropertyList &propList)
{
	_U(("openTableCell(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_TABLE_CELL);
}

void RawListenerImpl::closeTableCell()
{
	_D(("closeTableCell()\n"), LC_OPEN_TABLE_CELL);
}

void RawListenerImpl::insertCoveredTableCell(const WPXPropertyList &propList)
{
	__iprintf("insertCoveredTableCell(%s)\n", getPropString(propList).cstr());
}

void RawListenerImpl::closeTable()
{
	_D(("closeTable()\n"), LC_OPEN_TABLE);
}
