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
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <iomanip>
#include <sstream>

#include <librevenge-generators/librevenge-generators.h>

#include "librevenge_internal.h"

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009

namespace librevenge
{

struct RVNGTextSpreadsheetGeneratorImpl
{
	explicit RVNGTextSpreadsheetGeneratorImpl(RVNGStringVector &sheets, bool isInfo);

	RVNGStringVector &m_sheets;
	std::ostringstream m_stream;
	bool m_isInfo, m_isFirstCell;
	int m_sheetLevel;
};

RVNGTextSpreadsheetGeneratorImpl::RVNGTextSpreadsheetGeneratorImpl(RVNGStringVector &sheets, const bool isInfo)
	: m_sheets(sheets)
	, m_stream()
	, m_isInfo(isInfo)
	, m_isFirstCell(false)
	, m_sheetLevel(0)
{
}

RVNGTextSpreadsheetGenerator::RVNGTextSpreadsheetGenerator(RVNGStringVector &sheets, const bool isInfo) :
	m_impl(new RVNGTextSpreadsheetGeneratorImpl(sheets, isInfo))
{
}

RVNGTextSpreadsheetGenerator::~RVNGTextSpreadsheetGenerator()
{
	delete m_impl;
}

void RVNGTextSpreadsheetGenerator::setDocumentMetaData(const RVNGPropertyList &propList)
{
	if (!m_impl->m_isInfo)
		return;
	RVNGPropertyList::Iter propIter(propList);
	for (propIter.rewind(); propIter.next();)
	{
		m_impl->m_stream << propIter.key() << ' ' << propIter()->getStr().cstr() << '\n';
	}
}

void RVNGTextSpreadsheetGenerator::startDocument(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::endDocument() {}

void RVNGTextSpreadsheetGenerator::defineEmbeddedFont(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::defineSheetNumberingStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openSheet(const RVNGPropertyList &)
{
	++m_impl->m_sheetLevel;
}
void RVNGTextSpreadsheetGenerator::closeSheet()
{
	if (--m_impl->m_sheetLevel) return;
	m_impl->m_sheets.append(m_impl->m_stream.str().c_str());
	m_impl->m_stream.str("");
}
void RVNGTextSpreadsheetGenerator::openSheetRow(const RVNGPropertyList & /* propList */)
{
	m_impl->m_isFirstCell=true;
}
void RVNGTextSpreadsheetGenerator::closeSheetRow()
{
	insertLineBreak();
}
void RVNGTextSpreadsheetGenerator::openSheetCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_isFirstCell)
		m_impl->m_isFirstCell=false;
	else
		insertTab();
	if (!propList["librevenge:value-type"])
		return;
	// now we need to retrieve the value in proplist
	std::string valueType(propList["librevenge:value-type"]->getStr().cstr());
	if (valueType=="double" || valueType=="scientific") valueType="float";
	else if (valueType=="percent") valueType="percentage";

	if (propList["librevenge:value"] && (valueType=="float" || valueType=="percentage" || valueType=="currency"))
	{
		if (valueType=="percentage") m_impl->m_stream << 100. * propList["librevenge:value"]->getDouble() << "%";
		else
		{
			m_impl->m_stream << propList["librevenge:value"]->getDouble();
			if (valueType=="currency") m_impl->m_stream << "$";
		}
	}
	else if (propList["librevenge:value"] && (valueType=="bool" || valueType=="boolean"))
		m_impl->m_stream << (propList["librevenge:value"]->getInt() ? "true" : "false");
	else if (valueType=="date")
	{
		int day=propList["librevenge:day"] ? propList["librevenge:day"]->getInt() : 1;
		int month=propList["librevenge:month"] ? propList["librevenge:month"]->getInt() : 1;
		int year=propList["librevenge:year"] ? propList["librevenge:year"]->getInt() : 2000;
		m_impl->m_stream << std::setfill('0') << std::setw(2) << month << "/"
		                 << std::setfill('0') << std::setw(2) << day << "/"
		                 << std::setfill('0') << std::setw(2) << year;
	}
	else if (valueType=="time")
	{
		int hour=propList["librevenge:hours"] ? propList["librevenge:hours"]->getInt() : 0;
		int minute=propList["librevenge:minutes"] ? propList["librevenge:minutes"]->getInt() : 0;
		int second=propList["librevenge:seconds"] ? propList["librevenge:seconds"]->getInt() : 0;
		m_impl->m_stream << std::setfill('0') << std::setw(2) << hour << ":"
		                 << std::setfill('0') << std::setw(2) << minute << ":"
		                 << std::setfill('0') << std::setw(2) << second;
	}
	else if (valueType!="string" && valueType!="text")
	{
		RVNG_DEBUG_MSG(("RVNGTextSpreadsheetGenerator::openSheetCell: unexpected value type: %s\n", valueType.c_str()));
	}
}
void RVNGTextSpreadsheetGenerator::closeSheetCell() {}

void RVNGTextSpreadsheetGenerator::defineChartStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openChart(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::closeChart() {}
void RVNGTextSpreadsheetGenerator::openChartTextObject(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::closeChartTextObject() {}
void RVNGTextSpreadsheetGenerator::openChartPlotArea(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::closeChartPlotArea() {}
void RVNGTextSpreadsheetGenerator::insertChartAxis(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openChartSerie(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::closeChartSerie() {}

void RVNGTextSpreadsheetGenerator::definePageStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openPageSpan(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closePageSpan() {}
void RVNGTextSpreadsheetGenerator::openHeader(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeHeader() {}
void RVNGTextSpreadsheetGenerator::openFooter(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeFooter() {}

void RVNGTextSpreadsheetGenerator::defineSectionStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openSection(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeSection() {}

void RVNGTextSpreadsheetGenerator::defineParagraphStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openParagraph(const RVNGPropertyList & /* propList */) {}

void RVNGTextSpreadsheetGenerator::closeParagraph()
{
	if (m_impl->m_isInfo)
		return;
	if (m_impl->m_sheetLevel)
		m_impl->m_stream << ' ';
	else
		m_impl->m_stream << '\n';
}

void RVNGTextSpreadsheetGenerator::defineCharacterStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openSpan(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeSpan() {}

void RVNGTextSpreadsheetGenerator::openLink(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeLink() {}

void RVNGTextSpreadsheetGenerator::insertTab()
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << static_cast<char>(UCS_TAB);
}

void RVNGTextSpreadsheetGenerator::insertText(const RVNGString &text)
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << text.cstr();
}

void RVNGTextSpreadsheetGenerator::insertSpace()
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << ' ';
}

void RVNGTextSpreadsheetGenerator::insertLineBreak()
{
	if (m_impl->m_isInfo)
		return;
	m_impl->m_stream << '\n';
}

void RVNGTextSpreadsheetGenerator::insertField(const RVNGPropertyList & /* propList */) {}

void RVNGTextSpreadsheetGenerator::openOrderedListLevel(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::openUnorderedListLevel(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeOrderedListLevel() {}
void RVNGTextSpreadsheetGenerator::closeUnorderedListLevel() {}
void RVNGTextSpreadsheetGenerator::openListElement(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeListElement() {}

void RVNGTextSpreadsheetGenerator::openFootnote(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeFootnote() {}
void RVNGTextSpreadsheetGenerator::openEndnote(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeEndnote() {}
void RVNGTextSpreadsheetGenerator::openComment(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeComment() {}
void RVNGTextSpreadsheetGenerator::openTextBox(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeTextBox() {}

void RVNGTextSpreadsheetGenerator::openTable(const RVNGPropertyList & /* propList */)
{
	++m_impl->m_sheetLevel;
}
void RVNGTextSpreadsheetGenerator::closeTable()
{
	--m_impl->m_sheetLevel;
}
void RVNGTextSpreadsheetGenerator::openTableRow(const RVNGPropertyList & /* propList */)
{
	m_impl->m_isFirstCell=true;
}
void RVNGTextSpreadsheetGenerator::closeTableRow()
{
	insertLineBreak();
}
void RVNGTextSpreadsheetGenerator::openTableCell(const RVNGPropertyList & /* propList */)
{
	if (m_impl->m_isFirstCell)
		m_impl->m_isFirstCell=false;
	else
		insertTab();
}
void RVNGTextSpreadsheetGenerator::closeTableCell()
{
	m_impl->m_sheetLevel=false;
}
void RVNGTextSpreadsheetGenerator::insertCoveredTableCell(const RVNGPropertyList & /* propList */) {}

void RVNGTextSpreadsheetGenerator::openFrame(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeFrame() {}

void RVNGTextSpreadsheetGenerator::openGroup(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::closeGroup() {}

void RVNGTextSpreadsheetGenerator::defineGraphicStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::drawRectangle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::drawEllipse(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::drawPolygon(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::drawPolyline(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::drawPath(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::drawConnector(const RVNGPropertyList &) {}

void RVNGTextSpreadsheetGenerator::insertBinaryObject(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::insertEquation(const RVNGPropertyList & /* propList */) {}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
