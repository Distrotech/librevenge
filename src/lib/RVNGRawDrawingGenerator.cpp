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

namespace
{

enum RVNGRawDrawingGeneratorCallback
{
    PC_START_GRAPHICS = 0,
    PC_START_LAYER,
    PC_START_EMBEDDED_GRAPHICS,
    PC_START_TEXT_OBJECT,
    PC_START_TEXT_LINE,
    PC_START_TEXT_SPAN
};

} // anonymous namespace

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

void RVNGRawDrawingGenerator::startDocument(const librevenge::RVNGPropertyList & /*propList*/) {}
void RVNGRawDrawingGenerator::endDocument() {}
void RVNGRawDrawingGenerator::setDocumentMetaData(const librevenge::RVNGPropertyList & /*propList*/) {}

void RVNGRawDrawingGenerator::startPage(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startPage(%s)\n", propList.getPropString().cstr()), PC_START_GRAPHICS);
}

void RVNGRawDrawingGenerator::endPage()
{
	RVNG_CALLGRAPH_LEAVE(("endPage\n"), PC_START_GRAPHICS);
}

void RVNGRawDrawingGenerator::startLayer(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startLayer (%s)\n", propList.getPropString().cstr()), PC_START_LAYER);
}

void RVNGRawDrawingGenerator::endLayer()
{
	RVNG_CALLGRAPH_LEAVE(("endLayer\n"), PC_START_LAYER);
}

void RVNGRawDrawingGenerator::startEmbeddedGraphics(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startEmbeddedGraphics (%s)\n", propList.getPropString().cstr()), PC_START_EMBEDDED_GRAPHICS);
}

void RVNGRawDrawingGenerator::endEmbeddedGraphics()
{
	RVNG_CALLGRAPH_LEAVE(("endEmbeddedGraphics \n"), PC_START_EMBEDDED_GRAPHICS);
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

void RVNGRawDrawingGenerator::startTextObject(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startTextObject (%s)\n", propList.getPropString().cstr()), PC_START_TEXT_OBJECT);
}

void RVNGRawDrawingGenerator::endTextObject()
{
	RVNG_CALLGRAPH_LEAVE(("endTextObject\n"), PC_START_TEXT_OBJECT);
}

void RVNGRawDrawingGenerator::openOrderedListLevel(const librevenge::RVNGPropertyList & /*propList*/) {}
void RVNGRawDrawingGenerator::closeOrderedListLevel() {}

void RVNGRawDrawingGenerator::openUnorderedListLevel(const librevenge::RVNGPropertyList & /*propList*/) {}
void RVNGRawDrawingGenerator::closeUnorderedListLevel() {}

void RVNGRawDrawingGenerator::openListElement(const librevenge::RVNGPropertyList & /*propList*/) {}
void RVNGRawDrawingGenerator::closeListElement() {}

void RVNGRawDrawingGenerator::openParagraph(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openParagraph (%s)\n", propList.getPropString().cstr()), PC_START_TEXT_LINE);
}

void RVNGRawDrawingGenerator::closeParagraph()
{
	RVNG_CALLGRAPH_LEAVE(("closeParagraph\n"), PC_START_TEXT_LINE);
}

void RVNGRawDrawingGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSpan (%s)\n", propList.getPropString().cstr()), PC_START_TEXT_SPAN);
}

void RVNGRawDrawingGenerator::closeSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closeSpan\n"), PC_START_TEXT_SPAN);
}

void RVNGRawDrawingGenerator::insertTab() {}
void RVNGRawDrawingGenerator::insertSpace() {}

void RVNGRawDrawingGenerator::insertText(const RVNGString &str)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertText (%s)\n", str.cstr());
}

void RVNGRawDrawingGenerator::insertLineBreak() {}
void RVNGRawDrawingGenerator::insertField(const librevenge::RVNGPropertyList & /*propList*/) {}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
