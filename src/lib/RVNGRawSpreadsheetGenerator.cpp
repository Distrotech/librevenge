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

enum RVNGRawSpreadsheetGeneratorCallback
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
	LC_OPEN_TEXT_BOX,
	LC_OPEN_TABLE,
	LC_OPEN_TABLE_ROW,
	LC_OPEN_TABLE_CELL,
	LC_OPEN_COMMENT,
	LC_OPEN_FRAME,
	LC_OPEN_SHEET,
	LC_OPEN_SHEET_ROW,
	LC_OPEN_SHEET_CELL,
	LC_OPEN_CHART,
	LC_OPEN_GRAPHIC,
	LC_OPEN_GRAPHIC_PAGE,
	LC_OPEN_GRAPHIC_LAYER
};

}

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

void RVNGRawSpreadsheetGenerator::startDocument()
{
	RVNG_CALLGRAPH_ENTER(("startDocument()\n"), LC_START_DOCUMENT);
}

void RVNGRawSpreadsheetGenerator::endDocument()
{
	RVNG_CALLGRAPH_LEAVE(("endDocument()\n"), LC_START_DOCUMENT);
}

//
// Sheet
//
void RVNGRawSpreadsheetGenerator::insertSheetConditionInNumberingStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertSheetConditionInNumberingStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::defineSheetNumberingStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineSheetNumberingStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openSheet(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSheet(%s)\n", propList.getPropString().cstr()), LC_OPEN_SHEET);
}

void RVNGRawSpreadsheetGenerator::closeSheet()
{
	RVNG_CALLGRAPH_LEAVE(("closeSheet()\n"), LC_OPEN_SHEET);
}

void RVNGRawSpreadsheetGenerator::openSheetRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSheetRow(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_SHEET_ROW);
}

void RVNGRawSpreadsheetGenerator::closeSheetRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeSheetRow()\n"), LC_OPEN_SHEET_ROW);
}

void RVNGRawSpreadsheetGenerator::openSheetCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSheetCell(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_SHEET_CELL);
}

void RVNGRawSpreadsheetGenerator::closeSheetCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeSheetCell()\n"), LC_OPEN_SHEET_CELL);
}

//
// Chart
//
void RVNGRawSpreadsheetGenerator::openChart(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openChart(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_CHART);
}

void RVNGRawSpreadsheetGenerator::closeChart()
{
	RVNG_CALLGRAPH_LEAVE(("closeChart()\n"), LC_OPEN_CHART);
}

void RVNGRawSpreadsheetGenerator::insertChartSerie(const RVNGPropertyList &serie)
{
	m_impl->iprintf("insertChartSerie(%s)\n", serie.getPropString().cstr());
}

//
// Graphic
//
void RVNGRawSpreadsheetGenerator::startGraphic(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startGraphic(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_GRAPHIC);
}

void RVNGRawSpreadsheetGenerator::endGraphic()
{
	RVNG_CALLGRAPH_LEAVE(("endGraphic()\n"), LC_OPEN_GRAPHIC);
}

void RVNGRawSpreadsheetGenerator::startGraphicPage(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startGraphicPage(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_GRAPHIC_PAGE);
}

void RVNGRawSpreadsheetGenerator::endGraphicPage()
{
	RVNG_CALLGRAPH_LEAVE(("endGraphicPage()\n"), LC_OPEN_GRAPHIC_PAGE);
}

void RVNGRawSpreadsheetGenerator::startGraphicLayer(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("startGraphicLayer(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_GRAPHIC_LAYER);
}

void RVNGRawSpreadsheetGenerator::endGraphicLayer()
{
	RVNG_CALLGRAPH_LEAVE(("endGraphicLayer()\n"), LC_OPEN_GRAPHIC_LAYER);
}

void RVNGRawSpreadsheetGenerator::setGraphicStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("setGraphicStyle(%s)\n", propList.getPropString().cstr());
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
	                     LC_OPEN_PAGE_SPAN);
}

void RVNGRawSpreadsheetGenerator::closePageSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closePageSpan()\n"),
	                     LC_OPEN_PAGE_SPAN);
}

void RVNGRawSpreadsheetGenerator::openHeader(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openHeader(%s)\n",
	                      propList.getPropString().cstr()),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::closeHeader()
{
	RVNG_CALLGRAPH_LEAVE(("closeHeader()\n"),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::openFooter(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFooter(%s)\n",
	                      propList.getPropString().cstr()),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::closeFooter()
{
	RVNG_CALLGRAPH_LEAVE(("closeFooter()\n"),
	                     LC_OPEN_HEADER_FOOTER);
}

void RVNGRawSpreadsheetGenerator::defineParagraphStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineParagraphStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openParagraph(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openParagraph(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_PARAGRAPH);
}

void RVNGRawSpreadsheetGenerator::closeParagraph()
{
	RVNG_CALLGRAPH_LEAVE(("closeParagraph()\n"), LC_OPEN_PARAGRAPH);
}

void RVNGRawSpreadsheetGenerator::defineCharacterStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineCharacterStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openSpan(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSpan(%s)\n", propList.getPropString().cstr()), LC_OPEN_SPAN);
}

void RVNGRawSpreadsheetGenerator::closeSpan()
{
	RVNG_CALLGRAPH_LEAVE(("closeSpan()\n"), LC_OPEN_SPAN);
}

void RVNGRawSpreadsheetGenerator::defineSectionStyle(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineSectionStyle(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openSection(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openSection(%s)\n", propList.getPropString().cstr()), LC_OPEN_SECTION);
}

void RVNGRawSpreadsheetGenerator::closeSection()
{
	RVNG_CALLGRAPH_LEAVE(("closeSection()\n"), LC_OPEN_SECTION);
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

void RVNGRawSpreadsheetGenerator::defineOrderedListLevel(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineOrderedListLevel(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::defineUnorderedListLevel(const RVNGPropertyList &propList)
{
	m_impl->iprintf("defineUnorderedListLevel(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::openOrderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openOrderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::openUnorderedListLevel(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openUnorderedListLevel(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::closeOrderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeOrderedListLevel()\n"),
	                     LC_OPEN_ORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::closeUnorderedListLevel()
{
	RVNG_CALLGRAPH_LEAVE(("closeUnorderedListLevel()\n"), LC_OPEN_UNORDERED_LIST_LEVEL);
}

void RVNGRawSpreadsheetGenerator::openListElement(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openListElement(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_LIST_ELEMENT);
}

void RVNGRawSpreadsheetGenerator::closeListElement()
{
	RVNG_CALLGRAPH_LEAVE(("closeListElement()\n"), LC_OPEN_LIST_ELEMENT);
}

void RVNGRawSpreadsheetGenerator::openFootnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFootnote(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_FOOTNOTE);
}

void RVNGRawSpreadsheetGenerator::closeFootnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeFootnote()\n"), LC_OPEN_FOOTNOTE);
}

void RVNGRawSpreadsheetGenerator::openEndnote(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openEndnote(number: %s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_ENDNOTE);
}

void RVNGRawSpreadsheetGenerator::closeEndnote()
{
	RVNG_CALLGRAPH_LEAVE(("closeEndnote()\n"), LC_OPEN_ENDNOTE);
}

void RVNGRawSpreadsheetGenerator::openComment(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openComment(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_COMMENT);
}

void RVNGRawSpreadsheetGenerator::closeComment()
{
	RVNG_CALLGRAPH_LEAVE(("closeComment()\n"), LC_OPEN_COMMENT);
}

void RVNGRawSpreadsheetGenerator::openTextBox(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTextBox(%s)\n", propList.getPropString().cstr()), LC_OPEN_TEXT_BOX);
}

void RVNGRawSpreadsheetGenerator::closeTextBox()
{
	RVNG_CALLGRAPH_LEAVE(("closeTextBox()\n"), LC_OPEN_TEXT_BOX);
}

void RVNGRawSpreadsheetGenerator::openTable(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTable(%s)\n", propList.getPropString().cstr()), LC_OPEN_TABLE);
}

void RVNGRawSpreadsheetGenerator::openTableRow(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableRow(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_TABLE_ROW);
}

void RVNGRawSpreadsheetGenerator::closeTableRow()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableRow()\n"), LC_OPEN_TABLE_ROW);
}

void RVNGRawSpreadsheetGenerator::openTableCell(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openTableCell(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_TABLE_CELL);
}

void RVNGRawSpreadsheetGenerator::closeTableCell()
{
	RVNG_CALLGRAPH_LEAVE(("closeTableCell()\n"), LC_OPEN_TABLE_CELL);
}

void RVNGRawSpreadsheetGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	m_impl->iprintf("insertCoveredTableCell(%s)\n", propList.getPropString().cstr());
}

void RVNGRawSpreadsheetGenerator::closeTable()
{
	RVNG_CALLGRAPH_LEAVE(("closeTable()\n"), LC_OPEN_TABLE);
}

void RVNGRawSpreadsheetGenerator::openFrame(const RVNGPropertyList &propList)
{
	RVNG_CALLGRAPH_ENTER(("openFrame(%s)\n", propList.getPropString().cstr()),
	                     LC_OPEN_FRAME);
}

void RVNGRawSpreadsheetGenerator::closeFrame()
{
	RVNG_CALLGRAPH_LEAVE(("closeFrame()\n"), LC_OPEN_FRAME);
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
