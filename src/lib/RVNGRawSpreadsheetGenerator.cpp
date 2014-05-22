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

struct RVNGRawSpreadsheetGeneratorImpl : RVNGRawGeneratorBase
{
	explicit RVNGRawSpreadsheetGeneratorImpl(bool printCallgraphScore);
};

RVNGRawSpreadsheetGeneratorImpl::RVNGRawSpreadsheetGeneratorImpl(const bool printCallgraphScore)
	: RVNGRawGeneratorBase(printCallgraphScore)
{
}

RVNGRawSpreadsheetGenerator::RVNGRawSpreadsheetGenerator(bool printCallgraphScore) :
	m_impl(new RVNGRawSpreadsheetGeneratorImpl(printCallgraphScore))
{
}

RVNGRawSpreadsheetGenerator::~RVNGRawSpreadsheetGenerator()
{
	if (m_impl->m_printCallgraphScore)
		printf("%d\n", m_impl->m_atLeastOneCallback ? (int)m_impl->m_callStack.size() + m_impl->m_callbackMisses : -1);

	delete m_impl;
}

void RVNGRawSpreadsheetGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("setDocumentMetaData(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::defineEmbeddedFont(const RVNGPropertyList &propList)
{
	if (m_impl->m_printCallgraphScore)
		return;

	m_impl->iprintf("defineEmbeddedFont(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::startDocument(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startDocument(%s)\n", propList.getPropString().cstr()), CALLBACK_START_DOCUMENT);
}

void RVNGRawSpreadsheetGenerator::endDocument()
{
	RVNG_CALLGRAPH_LEAVE(("endDocument()\n"), CALLBACK_START_DOCUMENT);
}

//
// Sheet
//
void RVNGRawSpreadsheetGenerator::defineSheetNumberingStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineSheetNumberingStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openSheet(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSheet(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_SHEET);
}

void RVNGRawSpreadsheetGenerator::closeSheet()
{
	RVNG_CALLGRAPH_LEAVE(("closeSheet()\n"), CALLBACK_OPEN_SHEET);
}

void RVNGRawSpreadsheetGenerator::openSheetRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSheetRow(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_SHEET_ROW);
}

void RVNGRawSpreadsheetGenerator::closeSheetRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeSheetRow()\n"), CALLBACK_OPEN_SHEET_ROW);
}

void RVNGRawSpreadsheetGenerator::openSheetCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSheetCell(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_SHEET_CELL);
}

void RVNGRawSpreadsheetGenerator::closeSheetCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeSheetCell()\n"), CALLBACK_OPEN_SHEET_CELL);
}

//
// Chart
//
void RVNGRawSpreadsheetGenerator::defineChartStyle(const RVNGPropertyList &axis)
{
	m_impl->iprintf("defineChartStyle(%s)\n", axis.getPropString().cstr());
}
void RVNGRawSpreadsheetGenerator::openChart(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChart(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART);
}
void RVNGRawSpreadsheetGenerator::closeChart()
{
	RVNG_CALLGRAPH_LEAVE(("closeChart()\n"), CALLBACK_OPEN_CHART);
}
void RVNGRawSpreadsheetGenerator::openChartTextObject(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChartTextObject(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART_TEXTOBJECT);
}
void RVNGRawSpreadsheetGenerator::closeChartTextObject()
{
	RVNG_CALLGRAPH_LEAVE(("closeChartTextObject()\n"), CALLBACK_OPEN_CHART_TEXTOBJECT);
}
void RVNGRawSpreadsheetGenerator::openChartPlotArea(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChartPlotArea(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART_PLOTAREA);
}
void RVNGRawSpreadsheetGenerator::closeChartPlotArea()
{
	RVNG_CALLGRAPH_LEAVE(("closeChartPlotArea()\n"), CALLBACK_OPEN_CHART_PLOTAREA);
}
void RVNGRawSpreadsheetGenerator::insertChartAxis(const RVNGPropertyList &axis)
{
	m_impl->iprintf("insertChartAxis(%s)\n", axis.getPropString().cstr());
}
void RVNGRawSpreadsheetGenerator::openChartSerie(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChartSerie(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_CHART_SERIE);
}
void RVNGRawSpreadsheetGenerator::closeChartSerie()
{
	RVNG_CALLGRAPH_LEAVE(("closeChartSerie()\n"), CALLBACK_OPEN_CHART_SERIE);
}

//
// Graphic
//
void RVNGRawSpreadsheetGenerator::openGroup(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openGroup(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_GRAPHIC_LAYER);
}

void RVNGRawSpreadsheetGenerator::closeGroup()
{
	RVNG_CALLGRAPH_LEAVE(("closeGroup()\n"), CALLBACK_OPEN_GRAPHIC_LAYER);
}

void RVNGRawSpreadsheetGenerator::defineGraphicStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineGraphicStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::drawRectangle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("drawRectangle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::drawEllipse(const RVNGPropertyList &propList)
{
	m_impl->iprintf("drawEllipse(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::drawPolygon(const RVNGPropertyList &propList)
{
	m_impl->iprintf("drawPolygon(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::drawPolyline(const RVNGPropertyList &propList)
{
	m_impl->iprintf("drawPolyline(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::drawPath(const RVNGPropertyList &propList)
{
	m_impl->iprintf("drawPath(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::drawConnector(const RVNGPropertyList &propList)
{
	m_impl->iprintf("drawConnector(%s)\n", propList.getPropString().cstr());
}

//
//
//

void RVNGRawSpreadsheetGenerator::definePageStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("definePageStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openPageSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openPageSpan(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_PAGE_SPAN);
}

void RVNGRawSpreadsheetGenerator::closePageSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closePageSpan()\n"),
	                     CALLBACK_OPEN_PAGE_SPAN);
}

void RVNGRawSpreadsheetGenerator::openHeader(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openHeader(%s)\n",
	                      propList.getPropString().cstr()),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::closeHeader()
{
	RVNG_CALLGRAPH_LEAVE(("closeHeader()\n"),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::openFooter(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFooter(%s)\n",
	                      propList.getPropString().cstr()),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::closeFooter()
{
	RVNG_CALLGRAPH_LEAVE(("closeFooter()\n"),
	                     CALLBACK_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::defineParagraphStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineParagraphStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openParagraph(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openParagraph(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawSpreadsheetGenerator::closeParagraph()
{
	RVNG_CALLGRAPH_LEAVE(("closeParagraph()\n"), CALLBACK_OPEN_PARAGRAPH);
}

void RVNGRawSpreadsheetGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineCharacterStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSpan(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_SPAN);
}

void RVNGRawSpreadsheetGenerator::closeSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closeSpan()\n"), CALLBACK_OPEN_SPAN);
}

void RVNGRawSpreadsheetGenerator::openLink(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openLink(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_LINK);
}

void RVNGRawSpreadsheetGenerator::closeLink()
{
	RVNG_CALLGRAPH_LEAVE(("closeLink()\n"), CALLBACK_OPEN_LINK);
}

void RVNGRawSpreadsheetGenerator::defineSectionStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineSectionStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openSection(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSection(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_SECTION);
}

void RVNGRawSpreadsheetGenerator::closeSection()
{
	RVNG_CALLGRAPH_LEAVE(("closeSection()\n"), CALLBACK_OPEN_SECTION);
}

void RVNGRawSpreadsheetGenerator::insertTab()
{
	m_impl->iprintf("insertTab()\n");
}

void RVNGRawSpreadsheetGenerator::insertSpace()
{
	m_impl->iprintf("insertSpace()\n");
}

void RVNGRawSpreadsheetGenerator::insertText(const RVNGString &text)
{
	m_impl->iprintf("insertText(text: %s)\n", text.cstr());
}

void RVNGRawSpreadsheetGenerator::insertLineBreak()
{
	m_impl->iprintf("insertLineBreak()\n");
}

void RVNGRawSpreadsheetGenerator::insertField(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertField(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openOrderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openUnorderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::closeOrderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeOrderedListLevel()\n"),
	                     CALLBACK_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::closeUnorderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeUnorderedListLevel()\n"), CALLBACK_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::openListElement(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openListElement(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawSpreadsheetGenerator::closeListElement()
{
	RVNG_CALLGRAPH_LEAVE(("closeListElement()\n"), CALLBACK_OPEN_LIST_ELEMENT);
}

void RVNGRawSpreadsheetGenerator::openFootnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFootnote(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_FOOTNOTE);
}

void RVNGRawSpreadsheetGenerator::closeFootnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeFootnote()\n"), CALLBACK_OPEN_FOOTNOTE);
}

void RVNGRawSpreadsheetGenerator::openEndnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openEndnote(number: %s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_ENDNOTE);
}

void RVNGRawSpreadsheetGenerator::closeEndnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeEndnote()\n"), CALLBACK_OPEN_ENDNOTE);
}

void RVNGRawSpreadsheetGenerator::openComment(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openComment(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_COMMENT);
}

void RVNGRawSpreadsheetGenerator::closeComment()
{
	RVNG_CALLGRAPH_LEAVE(("closeComment()\n"), CALLBACK_OPEN_COMMENT);
}

void RVNGRawSpreadsheetGenerator::openTextBox(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTextBox(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_TEXT_BOX);
}

void RVNGRawSpreadsheetGenerator::closeTextBox()
{
	RVNG_CALLGRAPH_LEAVE(("closeTextBox()\n"), CALLBACK_OPEN_TEXT_BOX);
}

void RVNGRawSpreadsheetGenerator::openTable(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTable(%s)\n", propList.getPropString().cstr()), CALLBACK_OPEN_TABLE);
}

void RVNGRawSpreadsheetGenerator::openTableRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableRow(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawSpreadsheetGenerator::closeTableRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableRow()\n"), CALLBACK_OPEN_TABLE_ROW);
}

void RVNGRawSpreadsheetGenerator::openTableCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableCell(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawSpreadsheetGenerator::closeTableCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableCell()\n"), CALLBACK_OPEN_TABLE_CELL);
}

void RVNGRawSpreadsheetGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertCoveredTableCell(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::closeTable()
{
	RVNG_CALLGRAPH_LEAVE(("closeTable()\n"), CALLBACK_OPEN_TABLE);
}

void RVNGRawSpreadsheetGenerator::openFrame(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFrame(%s)\n", propList.getPropString().cstr()),
	                     CALLBACK_OPEN_FRAME);
}

void RVNGRawSpreadsheetGenerator::closeFrame()
{
	RVNG_CALLGRAPH_LEAVE(("closeFrame()\n"), CALLBACK_OPEN_FRAME);
}

void RVNGRawSpreadsheetGenerator::insertBinaryObject(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertBinaryObject(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::insertEquation(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertEquation(%s)\n", propList.getPropString().cstr());
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
