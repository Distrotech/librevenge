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

#include <stdio.h>

#include <librevenge-generators/librevenge-generators.h>

#include "RVNGRawGeneratorBase.h"

namespace librevenge
{

namespace
{

enum RVNGRawTextGeneratorCallback
{
    LC_START_DOCUMENT = 0,
    LC_OPEN_PAGE_SPAN,
    LC_OPEN_HEADER_FOOTER,
    LC_OPEN_PARAGRAPH,
    LC_OPEN_SPAN,
    LC_OPEN_SECTION,
    LC_OPEN_ORDERED_LIST_LEVEL,
    LC_OPEN_UNORDERED_LIST_LEVEL,
    LC_OPEN_LIST_ELEMENT,
    LC_OPEN_FOOTNOTE,
    LC_OPEN_ENDNOTE,
    LC_OPEN_TABLE,
    LC_OPEN_TABLE_ROW,
    LC_OPEN_TABLE_CELL,
    LC_OPEN_COMMENT,
    LC_OPEN_TEXT_BOX,
    LC_OPEN_FRAME
};

}

struct RVNGRawTextGeneratorImpl : RVNGRawGeneratorBase
{
	explicit RVNGRawTextGeneratorImpl(bool printCallgraphScore);
};

RVNGRawTextGeneratorImpl::RVNGRawTextGeneratorImpl(const bool printCallgraphScore)
	: RVNGRawGeneratorBase(printCallgraphScore)
{
}

RVNGRawTextGenerator::RVNGRawTextGenerator(bool printCallgraphScore) :
	m_impl(new RVNGRawTextGeneratorImpl(printCallgraphScore))
{
}

RVNGRawTextGenerator::~RVNGRawTextGenerator()
{
	if (m_impl->m_printCallgraphScore)
		printf("%d\n", m_impl->m_atLeastOneCallback ? (int)m_impl->m_callStack.size() + m_impl->m_callbackMisses : -1);

	delete m_impl;
}

void RVNGRawTextGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setDocumentMetaData(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::startDocument()
{
	RVNG_CALLGRAPH_ENTER(("startDocument()\n"), LC_START_DOCUMENT);
}

void RVNGRawTextGenerator::endDocument()
{
	RVNG_CALLGRAPH_LEAVE(("endDocument()\n"), LC_START_DOCUMENT);
}

void RVNGRawTextGenerator::definePageStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("definePageStyle(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::openPageSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openPageSpan(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_PAGE_SPAN);
}

void RVNGRawTextGenerator::closePageSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closePageSpan()\n"),
	                     LC_OPEN_PAGE_SPAN);
}

void RVNGRawTextGenerator::openHeader(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openHeader(%s)\n",
	                      getPropString(propList).cstr()),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::closeHeader()
{
	RVNG_CALLGRAPH_LEAVE(("closeHeader()\n"),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::openFooter(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFooter(%s)\n",
	                      getPropString(propList).cstr()),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::closeFooter()
{
	RVNG_CALLGRAPH_LEAVE(("closeFooter()\n"),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::defineParagraphStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	m_impl->iprintf("defineParagraphStyle(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr());
}

void RVNGRawTextGenerator::openParagraph(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	RVNG_CALLGRAPH_ENTER(("openParagraph(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()),
	                     LC_OPEN_PARAGRAPH);
}

void RVNGRawTextGenerator::closeParagraph()
{
	RVNG_CALLGRAPH_LEAVE(("closeParagraph()\n"), LC_OPEN_PARAGRAPH);
}

void RVNGRawTextGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineCharacterStyle(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSpan(%s)\n", getPropString(propList).cstr()), LC_OPEN_SPAN);
}

void RVNGRawTextGenerator::closeSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closeSpan()\n"), LC_OPEN_SPAN);
}

void RVNGRawTextGenerator::defineSectionStyle(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns)
{
	m_impl->iprintf("defineSectionStyle(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr());
}

void RVNGRawTextGenerator::openSection(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns)
{
	RVNG_CALLGRAPH_ENTER(("openSection(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_SECTION);
}

void RVNGRawTextGenerator::closeSection()
{
	RVNG_CALLGRAPH_LEAVE(("closeSection()\n"), LC_OPEN_SECTION);
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
	RVNG_CALLGRAPH_ENTER(("openOrderedListLevel(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openUnorderedListLevel(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::closeOrderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeOrderedListLevel()\n"),
	                     LC_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::closeUnorderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeUnorderedListLevel()\n"), LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::openListElement(const RVNGPropertyList &propList, const RVNGPropertyListVector &tabStops)
{
	RVNG_CALLGRAPH_ENTER(("openListElement(%s, tab-stops: %s)\n", getPropString(propList).cstr(), getPropString(tabStops).cstr()),
	                     LC_OPEN_LIST_ELEMENT);
}

void RVNGRawTextGenerator::closeListElement()
{
	RVNG_CALLGRAPH_LEAVE(("closeListElement()\n"), LC_OPEN_LIST_ELEMENT);
}

void RVNGRawTextGenerator::openFootnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFootnote(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_FOOTNOTE);
}

void RVNGRawTextGenerator::closeFootnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeFootnote()\n"), LC_OPEN_FOOTNOTE);
}

void RVNGRawTextGenerator::openEndnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openEndnote(number: %s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_ENDNOTE);
}

void RVNGRawTextGenerator::closeEndnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeEndnote()\n"), LC_OPEN_ENDNOTE);
}

void RVNGRawTextGenerator::openComment(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openComment(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_COMMENT);
}

void RVNGRawTextGenerator::closeComment()
{
	RVNG_CALLGRAPH_LEAVE(("closeComment()\n"), LC_OPEN_COMMENT);
}

void RVNGRawTextGenerator::openTextBox(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTextBox(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_TEXT_BOX);
}

void RVNGRawTextGenerator::closeTextBox()
{
	RVNG_CALLGRAPH_LEAVE(("closeTextBox()\n"), LC_OPEN_TEXT_BOX);
}

void RVNGRawTextGenerator::openTable(const RVNGPropertyList &propList, const RVNGPropertyListVector &columns)
{
	RVNG_CALLGRAPH_ENTER(("openTable(%s, columns: %s)\n", getPropString(propList).cstr(), getPropString(columns).cstr()), LC_OPEN_TABLE);
}

void RVNGRawTextGenerator::openTableRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableRow(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_TABLE_ROW);
}

void RVNGRawTextGenerator::closeTableRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableRow()\n"), LC_OPEN_TABLE_ROW);
}

void RVNGRawTextGenerator::openTableCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableCell(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_TABLE_CELL);
}

void RVNGRawTextGenerator::closeTableCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableCell()\n"), LC_OPEN_TABLE_CELL);
}

void RVNGRawTextGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertCoveredTableCell(%s)\n", getPropString(propList).cstr());
}

void RVNGRawTextGenerator::closeTable()
{
	RVNG_CALLGRAPH_LEAVE(("closeTable()\n"), LC_OPEN_TABLE);
}

void RVNGRawTextGenerator::openFrame(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFrame(%s)\n", getPropString(propList).cstr()),
	                     LC_OPEN_FRAME);
}

void RVNGRawTextGenerator::closeFrame()
{
	RVNG_CALLGRAPH_LEAVE(("closeFrame()\n"), LC_OPEN_FRAME);
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
