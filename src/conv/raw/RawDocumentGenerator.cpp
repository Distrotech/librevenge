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
#include "RawDocumentGenerator.h"

#ifdef _U
#undef _U
#endif

#define _U(M, L) \
	m_atLeastOneCallback = true; \
	if (!m_printCallgraphScore) \
			__iuprintf M; \
	else \
		m_callStack.push(L);

#ifdef _D
#undef _D
#endif

#define _D(M, L) \
	m_atLeastOneCallback = true; \
	if (!m_printCallgraphScore) \
			__idprintf M; \
	else \
	{ \
		ListenerCallback lc = m_callStack.top(); \
		if (lc != L) \
			m_callbackMisses++; \
		m_callStack.pop(); \
	}	
	
RawDocumentGenerator::RawDocumentGenerator(bool printCallgraphScore) :
	m_indent(0),
	m_callbackMisses(0),
	m_atLeastOneCallback(false),
	m_printCallgraphScore(printCallgraphScore),
	m_callStack()
{
}

RawDocumentGenerator::~RawDocumentGenerator()
{
	if (m_printCallgraphScore)
		printf("%d\n", m_atLeastOneCallback ? (int)(m_callStack.size() + m_callbackMisses) : -1);
}

void RawDocumentGenerator::__iprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
	if (m_printCallgraphScore) return;
	
	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

void RawDocumentGenerator::__iuprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	__indentUp();
	va_end(args);
}

void RawDocumentGenerator::__idprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
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

void RawDocumentGenerator::setDocumentMetaData(const WPXPropertyList &propList)
{
	if (m_printCallgraphScore)
		return;

	__iprintf("setDocumentMetaData(%s)\n", getPropString(propList).cstr());
}

void RawDocumentGenerator::startDocument()
{
	_U(("startDocument()\n"), LC_START_DOCUMENT);
}

void RawDocumentGenerator::endDocument()
{
	_D(("endDocument()\n"), LC_START_DOCUMENT);
}

void RawDocumentGenerator::definePageStyle(const WPXPropertyList &propList)
{
	__iprintf("definePageStyle(%s)\n", getPropString(propList).cstr());
}

void RawDocumentGenerator::openPageSpan(const WPXPropertyList &propList)
{
	_U(("openPageSpan(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_PAGE_SPAN);
}

void RawDocumentGenerator::closePageSpan()
{
	_D(("closePageSpan()\n"),
		LC_OPEN_PAGE_SPAN);
}

void RawDocumentGenerator::openHeader(const WPXPropertyList &propList)
{
	_U(("openHeader(%s)\n",
	    getPropString(propList).cstr()),
	   LC_OPEN_HEADER_FOOTER);
}

void RawDocumentGenerator::closeHeader()
{
	_D(("closeHeader()\n"),
	   LC_OPEN_HEADER_FOOTER);
}

void RawDocumentGenerator::openFooter(const WPXPropertyList &propList)
{
	_U(("openFooter(%s)\n",
	    getPropString(propList).cstr()),
	   LC_OPEN_HEADER_FOOTER);
}

void RawDocumentGenerator::closeFooter()
{
	_D(("closeFooter()\n"),
	   LC_OPEN_HEADER_FOOTER);
}

void RawDocumentGenerator::defineParagraphStyle(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops)
{
	__iprintf("defineParagraphStyle(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr());
}

void RawDocumentGenerator::openParagraph(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops)
{
	_U(("openParagraph(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()),
	   LC_OPEN_PARAGRAPH);
}

void RawDocumentGenerator::closeParagraph()
{
	_D(("closeParagraph()\n"), LC_OPEN_PARAGRAPH);
}

void RawDocumentGenerator::defineCharacterStyle(const WPXPropertyList &propList)
{
	__iprintf("defineCharacterStyle(%s)\n", getPropString(propList).cstr());
}

void RawDocumentGenerator::openSpan(const WPXPropertyList &propList)
{
	_U(("openSpan(%s)\n", getPropString(propList).cstr()), LC_OPEN_SPAN);
}

void RawDocumentGenerator::closeSpan()
{
	_D(("closeSpan()\n"), LC_OPEN_SPAN);
}

void RawDocumentGenerator::defineSectionStyle(const WPXPropertyList &propList, const WPXPropertyListVector &columns)
{
	__iprintf("defineSectionStyle(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr());
}

void RawDocumentGenerator::openSection(const WPXPropertyList &propList, const WPXPropertyListVector &columns)
{
	_U(("openSection(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_SECTION);
}

void RawDocumentGenerator::closeSection()
{
	_D(("closeSection()\n"), LC_OPEN_SECTION);
}

void RawDocumentGenerator::insertTab()
{
	__iprintf("insertTab()\n");
}

void RawDocumentGenerator::insertSpace()
{
	__iprintf("insertSpace()\n");
}

void RawDocumentGenerator::insertText(const WPXString &text)
{
	__iprintf("insertText(text: %s)\n", text.cstr());
}

void RawDocumentGenerator::insertLineBreak()
{
	__iprintf("insertLineBreak()\n");
}

void RawDocumentGenerator::insertField(const WPXString &type, const WPXPropertyList &propList)
{
	__iprintf("insertField(type: %s, %s)\n", type.cstr(), getPropString(propList).cstr());
}

void RawDocumentGenerator::defineOrderedListLevel(const WPXPropertyList &propList)
{
	__iprintf("defineOrderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RawDocumentGenerator::defineUnorderedListLevel(const WPXPropertyList &propList)
{
	__iprintf("defineUnorderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RawDocumentGenerator::openOrderedListLevel(const WPXPropertyList &propList)
{
	_U(("openOrderedListLevel(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_ORDERED_LIST_LEVEL);
}

void RawDocumentGenerator::openUnorderedListLevel(const WPXPropertyList &propList)
{
	_U(("openUnorderedListLevel(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RawDocumentGenerator::closeOrderedListLevel()
{
	_D(("closeOrderedListLevel()\n"),
	   LC_OPEN_ORDERED_LIST_LEVEL);
}

void RawDocumentGenerator::closeUnorderedListLevel()
{
	_D(("closeUnorderedListLevel()\n"), LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RawDocumentGenerator::openListElement(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops)
{
	_U(("openListElement(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()), 
	   LC_OPEN_LIST_ELEMENT);
}

void RawDocumentGenerator::closeListElement()
{
	_D(("closeListElement()\n"), LC_OPEN_LIST_ELEMENT);
}

void RawDocumentGenerator::openFootnote(const WPXPropertyList &propList)
{
	_U(("openFootnote(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_FOOTNOTE);
}

void RawDocumentGenerator::closeFootnote()
{
	_D(("closeFootnote()\n"), LC_OPEN_FOOTNOTE);
}

void RawDocumentGenerator::openEndnote(const WPXPropertyList &propList)
{
	_U(("openEndnote(number: %s)\n", getPropString(propList).cstr()),
	   LC_OPEN_ENDNOTE);
}

void RawDocumentGenerator::closeEndnote()
{
	_D(("closeEndnote()\n"), LC_OPEN_ENDNOTE);
}

void RawDocumentGenerator::openComment(const WPXPropertyList &propList)
{
	_U(("openComment(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_COMMENT);
}

void RawDocumentGenerator::closeComment()
{
	_D(("closeComment()\n"), LC_OPEN_COMMENT);
}

void RawDocumentGenerator::openTextBox(const WPXPropertyList &propList)
{
	_U(("openTextBox(%s)\n", getPropString(propList).cstr()),
	  LC_OPEN_TEXT_BOX);
}

void RawDocumentGenerator::closeTextBox()
{
	_D(("closeTextBox()\n"), LC_OPEN_TEXT_BOX);
}
	
void RawDocumentGenerator::openTable(const WPXPropertyList &propList, const WPXPropertyListVector &columns)
{
	_U(("openTable(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_TABLE);
}

void RawDocumentGenerator::openTableRow(const WPXPropertyList &propList)
{
	_U(("openTableRow(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_TABLE_ROW);
}

void RawDocumentGenerator::closeTableRow()
{
	_D(("closeTableRow()\n"), LC_OPEN_TABLE_ROW);
}

void RawDocumentGenerator::openTableCell(const WPXPropertyList &propList)
{
	_U(("openTableCell(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_TABLE_CELL);
}

void RawDocumentGenerator::closeTableCell()
{
	_D(("closeTableCell()\n"), LC_OPEN_TABLE_CELL);
}

void RawDocumentGenerator::insertCoveredTableCell(const WPXPropertyList &propList)
{
	__iprintf("insertCoveredTableCell(%s)\n", getPropString(propList).cstr());
}

void RawDocumentGenerator::closeTable()
{
	_D(("closeTable()\n"), LC_OPEN_TABLE);
}

void RawDocumentGenerator::openFrame(const WPXPropertyList &propList)
{
	_U(("openFrame(%s)\n", getPropString(propList).cstr()),
	  LC_OPEN_FRAME);
}

void RawDocumentGenerator::closeFrame()
{
	_D(("closeFrame()\n"), LC_OPEN_FRAME);
}
	
void RawDocumentGenerator::insertBinaryObject(const WPXPropertyList &propList, const WPXBinaryData & /* data */)
{
	__iprintf("insertBinaryObject(%s)\n", getPropString(propList).cstr());
}
	
void RawDocumentGenerator::insertEquation(const WPXPropertyList &propList, const WPXString &data)
{
	__iprintf("insertEquation(%s, text: %s)\n", getPropString(propList).cstr(), data.cstr());
}
