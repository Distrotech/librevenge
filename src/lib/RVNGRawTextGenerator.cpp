/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <stack>
#include <stdio.h>
#include <stdarg.h>

#include <librevenge-generators/librevenge-generators.h>

#ifdef _U
#undef _U
#endif

#define _U(M, L) \
	m_impl->m_atLeastOneCallback = true; \
	if (!m_impl->m_printCallgraphScore) \
			m_impl->iuprintf M; \
	else \
		m_impl->m_callStack.push(L);

#ifdef _D
#undef _D
#endif

#define _D(M, L) \
	m_impl->m_atLeastOneCallback = true; \
	if (!m_impl->m_printCallgraphScore) \
			m_impl->idprintf M; \
	else \
	{ \
		RVNGRawTextGeneratorCallback lc = m_impl->m_callStack.top(); \
		if (lc != L) \
			m_impl->m_callbackMisses++; \
		m_impl->m_callStack.pop(); \
	}

using namespace std;

namespace librevenge
{

struct RVNGRawTextGeneratorImpl
{
	explicit RVNGRawTextGeneratorImpl(bool printCallgraphScore);

	int m_indent;
	int m_callbackMisses;
	bool m_atLeastOneCallback;
	bool m_printCallgraphScore;
	stack<RVNGRawTextGeneratorCallback> m_callStack;

	void indentUp()
	{
		m_indent++;
	}
	void indentDown()
	{
		if (m_indent > 0) m_indent--;
	}

	void iprintf(const char *format, ...);
	void iuprintf(const char *format, ...);
	void idprintf(const char *format, ...);
};

RVNGRawTextGeneratorImpl::RVNGRawTextGeneratorImpl(const bool printCallgraphScore) :
	m_indent(0),
	m_callbackMisses(0),
	m_atLeastOneCallback(false),
	m_printCallgraphScore(printCallgraphScore),
	m_callStack()
{
}

RVNGRawTextGenerator::RVNGRawTextGenerator(bool printCallgraphScore) :
	m_impl(new RVNGRawTextGeneratorImpl(printCallgraphScore))
{
}

RVNGRawTextGenerator::~RVNGRawTextGenerator()
{
	if (m_impl->m_printCallgraphScore)
		printf("%d\n", m_impl->m_atLeastOneCallback ? (int)(m_impl->m_callStack.size() + m_impl->m_callbackMisses) : -1);

	delete m_impl;
}

void RVNGRawTextGeneratorImpl::iprintf(const char *format, ...)
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

void RVNGRawTextGeneratorImpl::iuprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
	va_list args;
	va_start(args, format);
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	indentUp();
	va_end(args);
}

void RVNGRawTextGeneratorImpl::idprintf(const char *format, ...)
{
	m_atLeastOneCallback = true;
	va_list args;
	va_start(args, format);
	indentDown();
	for (int i=0; i<m_indent; i++)
		printf("  ");
	vprintf(format, args);
	va_end(args);
}

RVNGString getPropString(const RVNGPropertyList &propList)
{
	RVNGString propString;
	RVNGPropertyList::Iter i(propList);
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

RVNGString getPropString(const RVNGPropertyListVector &itemList)
{
	RVNGString propString;

	propString.append("(");
	RVNGPropertyListVector::Iter i(itemList);

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

void RVNGRawTextGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setDocumentMetaData(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::startDocument()
{
	_U(("startDocument()\n"), LC_START_DOCUMENT);
}

void RVNGRawTextGenerator::endDocument()
{
	_D(("endDocument()\n"), LC_START_DOCUMENT);
}

void RVNGRawTextGenerator::definePageStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("definePageStyle(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::openPageSpan(const RVNGPropertyList &propList)
{
	_U(("openPageSpan(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_PAGE_SPAN);
}

void RVNGRawTextGenerator::closePageSpan()
{
	_D(("closePageSpan()\n"),
	   LC_OPEN_PAGE_SPAN);
}

void RVNGRawTextGenerator::openHeader(const RVNGPropertyList &propList)
{
	_U(("openHeader(%s)\n",
	    getPropString(propList).cstr()),
	   LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::closeHeader()
{
	_D(("closeHeader()\n"),
	   LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::openFooter(const RVNGPropertyList &propList)
{
	_U(("openFooter(%s)\n",
	    getPropString(propList).cstr()),
	   LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::closeFooter()
{
	_D(("closeFooter()\n"),
	   LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::defineParagraphStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	m_impl->iprintf("defineParagraphStyle(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr());
}

void RVNGRawTextGenerator::openParagraph(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	_U(("openParagraph(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()),
	   LC_OPEN_PARAGRAPH);
}

void RVNGRawTextGenerator::closeParagraph()
{
	_D(("closeParagraph()\n"), LC_OPEN_PARAGRAPH);
}

void RVNGRawTextGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineCharacterStyle(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::openSpan(const RVNGPropertyList &propList)
{
	_U(("openSpan(%s)\n", getPropString(propList).cstr()), LC_OPEN_SPAN);
}

void RVNGRawTextGenerator::closeSpan()
{
	_D(("closeSpan()\n"), LC_OPEN_SPAN);
}

void RVNGRawTextGenerator::defineSectionStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns)
{
	m_impl->iprintf("defineSectionStyle(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr());
}

void RVNGRawTextGenerator::openSection(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns)
{
	_U(("openSection(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_SECTION);
}

void RVNGRawTextGenerator::closeSection()
{
	_D(("closeSection()\n"), LC_OPEN_SECTION);
}

void RVNGRawTextGenerator::insertTab()
{
	m_impl->iprintf("insertTab()\n");
}

void RVNGRawTextGenerator::insertSpace()
{
	m_impl->iprintf("insertSpace()\n");
}

void RVNGRawTextGenerator::insertText(const RVNGString &text)
{
	m_impl->iprintf("insertText(text: %s)\n", text.cstr());
}

void RVNGRawTextGenerator::insertLineBreak()
{
	m_impl->iprintf("insertLineBreak()\n");
}

void RVNGRawTextGenerator::insertField(const RVNGString &type, const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertField(type: %s, %s)\n", type.cstr(), getPropString(propList).cstr());
}

void RVNGRawTextGenerator::defineOrderedListLevel(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineOrderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::defineUnorderedListLevel(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineUnorderedListLevel(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	_U(("openOrderedListLevel(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	_U(("openUnorderedListLevel(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::closeOrderedListLevel()
{
	_D(("closeOrderedListLevel()\n"),
	   LC_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::closeUnorderedListLevel()
{
	_D(("closeUnorderedListLevel()\n"), LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::openListElement(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	_U(("openListElement(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()),
	   LC_OPEN_LIST_ELEMENT);
}

void RVNGRawTextGenerator::closeListElement()
{
	_D(("closeListElement()\n"), LC_OPEN_LIST_ELEMENT);
}

void RVNGRawTextGenerator::openFootnote(const RVNGPropertyList &propList)
{
	_U(("openFootnote(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_FOOTNOTE);
}

void RVNGRawTextGenerator::closeFootnote()
{
	_D(("closeFootnote()\n"), LC_OPEN_FOOTNOTE);
}

void RVNGRawTextGenerator::openEndnote(const RVNGPropertyList &propList)
{
	_U(("openEndnote(number: %s)\n", getPropString(propList).cstr()),
	   LC_OPEN_ENDNOTE);
}

void RVNGRawTextGenerator::closeEndnote()
{
	_D(("closeEndnote()\n"), LC_OPEN_ENDNOTE);
}

void RVNGRawTextGenerator::openComment(const RVNGPropertyList &propList)
{
	_U(("openComment(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_COMMENT);
}

void RVNGRawTextGenerator::closeComment()
{
	_D(("closeComment()\n"), LC_OPEN_COMMENT);
}

void RVNGRawTextGenerator::openTextBox(const RVNGPropertyList &propList)
{
	_U(("openTextBox(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_TEXT_BOX);
}

void RVNGRawTextGenerator::closeTextBox()
{
	_D(("closeTextBox()\n"), LC_OPEN_TEXT_BOX);
}

void RVNGRawTextGenerator::openTable(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns)
{
	_U(("openTable(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_TABLE);
}

void RVNGRawTextGenerator::openTableRow(const RVNGPropertyList &propList)
{
	_U(("openTableRow(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_TABLE_ROW);
}

void RVNGRawTextGenerator::closeTableRow()
{
	_D(("closeTableRow()\n"), LC_OPEN_TABLE_ROW);
}

void RVNGRawTextGenerator::openTableCell(const RVNGPropertyList &propList)
{
	_U(("openTableCell(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_TABLE_CELL);
}

void RVNGRawTextGenerator::closeTableCell()
{
	_D(("closeTableCell()\n"), LC_OPEN_TABLE_CELL);
}

void RVNGRawTextGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertCoveredTableCell(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::closeTable()
{
	_D(("closeTable()\n"), LC_OPEN_TABLE);
}

void RVNGRawTextGenerator::openFrame(const RVNGPropertyList &propList)
{
	_U(("openFrame(%s)\n", getPropString(propList).cstr()),
	   LC_OPEN_FRAME);
}

void RVNGRawTextGenerator::closeFrame()
{
	_D(("closeFrame()\n"), LC_OPEN_FRAME);
}

void RVNGRawTextGenerator::insertBinaryObject(const RVNGPropertyList &propList, const RVNGBinaryData & /* data */)
{
	m_impl->iprintf("insertBinaryObject(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::insertEquation(const RVNGPropertyList &propList, const RVNGString &data)
{
	m_impl->iprintf("insertEquation(%s, text: %s)\n", getPropString(propList).cstr(), data.cstr());
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
