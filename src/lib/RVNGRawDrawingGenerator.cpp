/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2004 Marc Oude Kotte (marc@solcon.nl)
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

struct RVNGRawDrawingGeneratorImpl : RVNGRawGeneratorBase
{
	explicit RVNGRawDrawingGeneratorImpl(bool printCallgraphScore);
};

RVNGRawDrawingGeneratorImpl::RVNGRawDrawingGeneratorImpl(const bool printCallgraphScore)
	: RVNGRawGeneratorBase(printCallgraphScore)
{
}

RVNGRawDrawingGenerator::RVNGRawDrawingGenerator(bool printCallgraphScore):
	RVNGDrawingInterface(),
	m_impl(new RVNGRawDrawingGeneratorImpl(printCallgraphScore))
{
}

RVNGRawDrawingGenerator::~RVNGRawDrawingGenerator()
{
	if (m_impl->m_printCallgraphScore)
		printf("%d\n", (int)m_impl->m_callStack.size() + m_impl->m_callbackMisses);

	delete m_impl;
}

void RVNGRawDrawingGenerator::startDocument(const librevenge::RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startDocument(%s)\n", propList.getPropString().cstr()), CALLBACK_START_DOCUMENT);
}

void RVNGRawDrawingGenerator::endDocument()
{
	RVNG_CALLGRAPH_LEAVE(("endDocument\n"), CALLBACK_START_DOCUMENT);
}

void RVNGRawDrawingGenerator::setDocumentMetaData(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setDocumentMetaData(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::defineEmbeddedFont(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineEmbeddedFont(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::startPage(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startPage(%s)\n", propList.getPropString().cstr()), CALLBACK_START_PAGE);
}

void RVNGRawDrawingGenerator::endPage()
{
	RVNG_CALLGRAPH_LEAVE(("endPage\n"), CALLBACK_START_PAGE);
}

void RVNGRawDrawingGenerator::startMasterPage(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startMasterPage(%s)\n", propList.getPropString().cstr()), CALLBACK_START_MASTER_PAGE);
}

void RVNGRawDrawingGenerator::endMasterPage()
{
	RVNG_CALLGRAPH_LEAVE(("endMasterPage\n"), CALLBACK_START_MASTER_PAGE);
}

void RVNGRawDrawingGenerator::startLayer(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startLayer (%s)\n", propList.getPropString().cstr()), CALLBACK_START_LAYER);
}

void RVNGRawDrawingGenerator::endLayer()
{
	RVNG_CALLGRAPH_LEAVE(("endLayer\n"), CALLBACK_START_LAYER);
}

void RVNGRawDrawingGenerator::startEmbeddedGraphics(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startEmbeddedGraphics (%s)\n", propList.getPropString().cstr()), CALLBACK_START_EMBEDDED_GRAPHICS);
}

void RVNGRawDrawingGenerator::endEmbeddedGraphics()
{
	RVNG_CALLGRAPH_LEAVE(("endEmbeddedGraphics \n"), CALLBACK_START_EMBEDDED_GRAPHICS);
}

void RVNGRawDrawingGenerator::openGroup(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openGroup(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_GROUP);
}

void RVNGRawDrawingGenerator::closeGroup()
{
	RVNG_CALLGRAPH_LEAVE(("closeGroup\n"), CALLBACK_OPEN_GROUP);
}

void RVNGRawDrawingGenerator::setStyle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawRectangle (%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawEllipse (%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::drawPolyline(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolyline (%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::drawPolygon(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolygon (%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::drawPath(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPath (%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::drawGraphicObject(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawGraphicObject (%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::drawConnector(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawConnector(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::startTextObject(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startTextObject (%s)\n", propList.getPropString().cstr()), CALLBACK_START_TEXT_OBJECT);
}

void RVNGRawDrawingGenerator::endTextObject()
{
	RVNG_CALLGRAPH_LEAVE(("endTextObject\n"), CALLBACK_START_TEXT_OBJECT);
}

void RVNGRawDrawingGenerator::openOrderedListLevel(const librevenge::RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openOrderedListLevel(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawDrawingGenerator::closeOrderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeOrderedListLevel\n"), CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawDrawingGenerator::openUnorderedListLevel(const librevenge::RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openUnorderedListLevel(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawDrawingGenerator::closeUnorderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeUnorderedListLevel\n"), CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawDrawingGenerator::openListElement(const librevenge::RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openListElement(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawDrawingGenerator::closeListElement()
{
	RVNG_CALLGRAPH_LEAVE(("closeListElement\n"), CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawDrawingGenerator::defineParagraphStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineParagraphStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::openParagraph(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openParagraph (%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawDrawingGenerator::closeParagraph()
{
	RVNG_CALLGRAPH_LEAVE(("closeParagraph\n"), CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawDrawingGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineCharacterStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSpan(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_SPAN);
}

void RVNGRawDrawingGenerator::closeSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closeSpan\n"), CALLBACK_OPEN_SPAN);
}

void RVNGRawDrawingGenerator::openLink(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openLink(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_LINK);
}

void RVNGRawDrawingGenerator::closeLink()
{
	RVNG_CALLGRAPH_LEAVE(("closeLink\n"), CALLBACK_OPEN_LINK);
}

void RVNGRawDrawingGenerator::insertTab()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertTab\n");
}

void RVNGRawDrawingGenerator::insertSpace()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertSpace\n");
}

void RVNGRawDrawingGenerator::insertText(const RVNGString &str)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertText (%s)\n", str.cstr());
}

void RVNGRawDrawingGenerator::insertLineBreak()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertLineBreak\n");
}

void RVNGRawDrawingGenerator::insertField(const librevenge::RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertField(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::startTableObject(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startTableObject(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_TABLE);
}

void RVNGRawDrawingGenerator::openTableRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableRow(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawDrawingGenerator::closeTableRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableRow()\n"), CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawDrawingGenerator::openTableCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableCell(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawDrawingGenerator::closeTableCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableCell()\n"), CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawDrawingGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertCoveredTableCell(%s)\n", propList.getPropString().cstr());
}

void RVNGRawDrawingGenerator::endTableObject()
{
	RVNG_CALLGRAPH_LEAVE(("endTableObject()\n"), CALLBACK_OPEN_TABLE);
}
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
