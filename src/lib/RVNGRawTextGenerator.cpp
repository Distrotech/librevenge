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

	m_impl->iprintf("setDocumentMetaData(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::startDocument(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startDocument()\n", propList.getPropString().cstr()), CALLBACK_START_DOCUMENT);
}

void RVNGRawTextGenerator::endDocument()
{
	RVNG_CALLGRAPH_LEAVE(("endDocument()\n"), CALLBACK_START_DOCUMENT);
}

void RVNGRawTextGenerator::definePageStyle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("definePageStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::defineEmbeddedFont(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineEmbeddedFont(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::openPageSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openPageSpan(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_PAGE_SPAN);
}

void RVNGRawTextGenerator::closePageSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closePageSpan()\n"),
	                     CALLBACK_OPEN_PAGE_SPAN);
}

void RVNGRawTextGenerator::openHeader(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openHeader(%s)\n",
	                      propList.getPropString().cstr()),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::closeHeader()
{
	RVNG_CALLGRAPH_LEAVE(("closeHeader()\n"),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::openFooter(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFooter(%s)\n",
	                      propList.getPropString().cstr()),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::closeFooter()
{
	RVNG_CALLGRAPH_LEAVE(("closeFooter()\n"),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawTextGenerator::defineParagraphStyle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineParagraphStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::openParagraph(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openParagraph(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawTextGenerator::closeParagraph()
{
	RVNG_CALLGRAPH_LEAVE(("closeParagraph()\n"), CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawTextGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineCharacterStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSpan(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_SPAN);
}

void RVNGRawTextGenerator::closeSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closeSpan()\n"), CALLBACK_OPEN_SPAN);
}

void RVNGRawTextGenerator::openLink(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openLink(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_LINK);
}

void RVNGRawTextGenerator::closeLink()
{
	RVNG_CALLGRAPH_LEAVE(("closeLink()\n"), CALLBACK_OPEN_LINK);
}

void RVNGRawTextGenerator::defineSectionStyle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineSectionStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::openSection(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSection(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_SECTION);
}

void RVNGRawTextGenerator::closeSection()
{
	RVNG_CALLGRAPH_LEAVE(("closeSection()\n"), CALLBACK_OPEN_SECTION);
}

void RVNGRawTextGenerator::insertTab()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertTab()\n");
}

void RVNGRawTextGenerator::insertSpace()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertSpace()\n");
}

void RVNGRawTextGenerator::insertText(const RVNGString &text)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertText(text: %s)\n", text.cstr());
}

void RVNGRawTextGenerator::insertLineBreak()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertLineBreak()\n");
}

void RVNGRawTextGenerator::insertField(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertField(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openOrderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openUnorderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::closeOrderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeOrderedListLevel()\n"),
	                     CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::closeUnorderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeUnorderedListLevel()\n"), CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawTextGenerator::openListElement(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openListElement(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawTextGenerator::closeListElement()
{
	RVNG_CALLGRAPH_LEAVE(("closeListElement()\n"), CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawTextGenerator::openFootnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFootnote(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_FOOTNOTE);
}

void RVNGRawTextGenerator::closeFootnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeFootnote()\n"), CALLBACK_OPEN_FOOTNOTE);
}

void RVNGRawTextGenerator::openEndnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openEndnote(number: %s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ENDNOTE);
}

void RVNGRawTextGenerator::closeEndnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeEndnote()\n"), CALLBACK_OPEN_ENDNOTE);
}

void RVNGRawTextGenerator::openComment(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openComment(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_COMMENT);
}

void RVNGRawTextGenerator::closeComment()
{
	RVNG_CALLGRAPH_LEAVE(("closeComment()\n"), CALLBACK_OPEN_COMMENT);
}

void RVNGRawTextGenerator::openTextBox(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTextBox(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TEXT_BOX);
}

void RVNGRawTextGenerator::closeTextBox()
{
	RVNG_CALLGRAPH_LEAVE(("closeTextBox()\n"), CALLBACK_OPEN_TEXT_BOX);
}

void RVNGRawTextGenerator::openTable(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTable(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_TABLE);
}

void RVNGRawTextGenerator::openTableRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableRow(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawTextGenerator::closeTableRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableRow()\n"), CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawTextGenerator::openTableCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableCell(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawTextGenerator::closeTableCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableCell()\n"), CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawTextGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertCoveredTableCell(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::closeTable()
{
	RVNG_CALLGRAPH_LEAVE(("closeTable()\n"), CALLBACK_OPEN_TABLE);
}

void RVNGRawTextGenerator::openFrame(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFrame(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_FRAME);
}

void RVNGRawTextGenerator::closeFrame()
{
	RVNG_CALLGRAPH_LEAVE(("closeFrame()\n"), CALLBACK_OPEN_FRAME);
}

void RVNGRawTextGenerator::openGroup(const librevenge::RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openGroup(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_GROUP);
}

void RVNGRawTextGenerator::closeGroup()
{
	RVNG_CALLGRAPH_LEAVE(("closeGroup()\n"), CALLBACK_OPEN_GROUP);
}

void RVNGRawTextGenerator::defineGraphicStyle(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineGraphicStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::drawRectangle(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawRectangle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::drawEllipse(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawEllipse(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::drawPolygon(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolygon(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::drawPolyline(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolyline(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::drawPath(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPath(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::drawConnector(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawConnector(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::insertBinaryObject(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertBinaryObject(%s)\n", propList.getPropString().cstr());
}

void RVNGRawTextGenerator::insertEquation(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertEquation(%s)\n", propList.getPropString().cstr());
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
