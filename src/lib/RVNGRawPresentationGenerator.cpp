/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

struct RVNGRawPresentationGeneratorImpl : RVNGRawGeneratorBase
{
	explicit RVNGRawPresentationGeneratorImpl(bool printCallgraphScore);
};

RVNGRawPresentationGeneratorImpl::RVNGRawPresentationGeneratorImpl(bool printCallgraphScore)
	: RVNGRawGeneratorBase(printCallgraphScore)
{
}

RVNGRawPresentationGenerator::RVNGRawPresentationGenerator(const bool printCallgraphScore)
	: m_impl(new RVNGRawPresentationGeneratorImpl(printCallgraphScore))
{
}

RVNGRawPresentationGenerator::~RVNGRawPresentationGenerator()
{
	if (m_impl->m_printCallgraphScore)
		printf("%d\n", m_impl->m_atLeastOneCallback ? (int)m_impl->m_callStack.size() + m_impl->m_callbackMisses : -1);

	delete m_impl;
}

void RVNGRawPresentationGenerator::startDocument(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startDocument(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_START_DOCUMENT);
}

void RVNGRawPresentationGenerator::endDocument()
{
	RVNG_CALLGRAPH_LEAVE(("endDocument()\n"), CALLBACK_START_DOCUMENT);
}

void RVNGRawPresentationGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setDocumentMetaData(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::defineEmbeddedFont(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineEmbeddedFont(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::startSlide(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startSlide(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_START_SLIDE);
}

void RVNGRawPresentationGenerator::endSlide()
{
	RVNG_CALLGRAPH_LEAVE(("endSlide()\n"), CALLBACK_START_SLIDE);
}

void RVNGRawPresentationGenerator::startMasterSlide(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startMasterSlide(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_START_MASTER_SLIDE);
}

void RVNGRawPresentationGenerator::endMasterSlide()
{
	RVNG_CALLGRAPH_LEAVE(("endMasterSlide()\n"), CALLBACK_START_MASTER_SLIDE);
}

void RVNGRawPresentationGenerator::setSlideTransition(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setSlideTransition(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::startLayer(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startLayer(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_START_LAYER);
}

void RVNGRawPresentationGenerator::endLayer()
{
	RVNG_CALLGRAPH_LEAVE(("endLayer()\n"), CALLBACK_START_LAYER);
}

void RVNGRawPresentationGenerator::startEmbeddedGraphics(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startEmbeddedGraphics(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_START_EMBEDDED_GRAPHICS);
}

void RVNGRawPresentationGenerator::endEmbeddedGraphics()
{
	RVNG_CALLGRAPH_LEAVE(("endEmbeddedGraphics \n"), CALLBACK_START_EMBEDDED_GRAPHICS);
}

void RVNGRawPresentationGenerator::openGroup(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openGroup(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_GROUP);
}

void RVNGRawPresentationGenerator::closeGroup()
{
	RVNG_CALLGRAPH_LEAVE(("closeGroup()\n"), CALLBACK_OPEN_GROUP);
}

void RVNGRawPresentationGenerator::setStyle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawRectangle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawEllipse(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::drawPolyline(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolyline(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::drawPolygon(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPolygon(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::drawPath(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawPath(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::drawGraphicObject(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawGraphicObject(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::drawConnector(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("drawConnector(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::startTextObject(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startTextObject(%s)\n", propList.getPropString().cstr()), CALLBACK_START_TEXT_OBJECT);
}

void RVNGRawPresentationGenerator::endTextObject()
{
	RVNG_CALLGRAPH_LEAVE(("endTextObject()\n"), CALLBACK_START_TEXT_OBJECT);
}

void RVNGRawPresentationGenerator::defineParagraphStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineParagraphStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::openParagraph(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openParagraph(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawPresentationGenerator::closeParagraph()
{
	RVNG_CALLGRAPH_LEAVE(("closeParagraph()\n"), CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawPresentationGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineCharacterStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSpan(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_SPAN);
}

void RVNGRawPresentationGenerator::closeSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closeSpan()\n"), CALLBACK_OPEN_SPAN);
}

void RVNGRawPresentationGenerator::openLink(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openLink(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_LINK);
}

void RVNGRawPresentationGenerator::closeLink()
{
	RVNG_CALLGRAPH_LEAVE(("closeLink()\n"), CALLBACK_OPEN_LINK);
}

void RVNGRawPresentationGenerator::insertText(const RVNGString &str)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertText(%s)\n", str.cstr());
}

void RVNGRawPresentationGenerator::insertTab()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertTab()\n");
}

void RVNGRawPresentationGenerator::insertSpace()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertSpace()\n");
}

void RVNGRawPresentationGenerator::insertLineBreak()
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertLineBreak()\n");
}

void RVNGRawPresentationGenerator::insertField(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertField(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openOrderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawPresentationGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openUnorderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawPresentationGenerator::closeOrderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeOrderedListLevel()\n"),
	                     CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawPresentationGenerator::closeUnorderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeUnorderedListLevel()\n"),
	                     CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawPresentationGenerator::openListElement(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openListElement(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawPresentationGenerator::closeListElement()
{
	RVNG_CALLGRAPH_LEAVE(("closeListElement()\n"), CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawPresentationGenerator::startTableObject(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startTableObject(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_TABLE);
}

void RVNGRawPresentationGenerator::openTableRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableRow(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawPresentationGenerator::closeTableRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableRow()\n"), CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawPresentationGenerator::openTableCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableCell(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawPresentationGenerator::closeTableCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableCell()\n"), CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawPresentationGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertCoveredTableCell(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::endTableObject()
{
	RVNG_CALLGRAPH_LEAVE(("endTableObject()\n"), CALLBACK_OPEN_TABLE);
}

void RVNGRawPresentationGenerator::startComment(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startComment(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_START_COMMENT);
}

void RVNGRawPresentationGenerator::endComment()
{
	RVNG_CALLGRAPH_LEAVE(("endComment()\n"), CALLBACK_START_COMMENT);
}

void RVNGRawPresentationGenerator::startNotes(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startNotes(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_START_NOTES);
}

void RVNGRawPresentationGenerator::endNotes()
{
	RVNG_CALLGRAPH_LEAVE(("endNotes()\n"), CALLBACK_START_NOTES);
}

void RVNGRawPresentationGenerator::defineChartStyle(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineChartStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::openChart(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChart(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART);
}

void RVNGRawPresentationGenerator::closeChart()
{
	RVNG_CALLGRAPH_LEAVE(("closeChart()\n"), CALLBACK_OPEN_CHART);
}

void RVNGRawPresentationGenerator::openChartTextObject(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChartTextObject(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART_TEXTOBJECT);
}

void RVNGRawPresentationGenerator::closeChartTextObject()
{
	RVNG_CALLGRAPH_LEAVE(("closeChartTextObject()\n"), CALLBACK_OPEN_CHART_TEXTOBJECT);
}

void RVNGRawPresentationGenerator::openChartPlotArea(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChartPlotArea(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART_PLOTAREA);
}

void RVNGRawPresentationGenerator::closeChartPlotArea()
{
	RVNG_CALLGRAPH_LEAVE(("closeChartPlotArea()\n"), CALLBACK_OPEN_CHART_PLOTAREA);
}

void RVNGRawPresentationGenerator::insertChartAxis(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertChartAxis(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::openChartSeries(const librevenge::RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChartSeries(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART_SERIE);
}

void RVNGRawPresentationGenerator::closeChartSeries()
{
	RVNG_CALLGRAPH_LEAVE(("closeChartSeries()\n"), CALLBACK_OPEN_CHART_SERIE);
}

void RVNGRawPresentationGenerator::openAnimationSequence(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openAnimationSequence(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ANIMATION_SEQUENCE);
}

void RVNGRawPresentationGenerator::closeAnimationSequence()
{
	RVNG_CALLGRAPH_LEAVE(("closeAnimationSequence()\n"), CALLBACK_OPEN_ANIMATION_SEQUENCE);
}

void RVNGRawPresentationGenerator::openAnimationGroup(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openAnimationGroup(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ANIMATION_GROUP);
}

void RVNGRawPresentationGenerator::closeAnimationGroup()
{
	RVNG_CALLGRAPH_LEAVE(("closeAnimationGroup()\n"), CALLBACK_OPEN_ANIMATION_GROUP);
}

void RVNGRawPresentationGenerator::openAnimationIteration(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openAnimationIteration(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ANIMATION_ITERATION);
}

void RVNGRawPresentationGenerator::closeAnimationIteration()
{
	RVNG_CALLGRAPH_LEAVE(("closeAnimationIteration()\n"), CALLBACK_OPEN_ANIMATION_ITERATION);
}

void RVNGRawPresentationGenerator::insertMotionAnimation(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertMotionAnimation(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::insertColorAnimation(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertColorAnimation(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::insertAnimation(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertAnimation(%s)\n", propList.getPropString().cstr());
}

void RVNGRawPresentationGenerator::insertEffect(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("insertEffect(%s)\n", propList.getPropString().cstr());
}


}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
