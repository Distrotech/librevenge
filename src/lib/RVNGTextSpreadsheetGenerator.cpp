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

	//! list of final string corresponding to each sheet
	RVNGStringVector &m_sheets;
	//! the file stream
	std::ostringstream m_stream;
	//! the actual row stream
	std::ostringstream m_rowStream;
	//! the actual cell stream
	std::ostringstream m_cellStream;
	//! the number of repetition of the actual row
	int m_numRowRepeated;
	//! the number of repetition of the actual cell
	int m_numCellRepeated;
	//! the number of empty cell which follow current cell
	int m_numCellToSkip;
	int m_column/** the current column*/, m_row/*the current row*/;
	int m_numColumnsInRow/** the number of column written in the current row*/;
	bool m_isInfo /** if set, do not generate output*/;
	int m_sheetLevel /** the number of opened sheet, to avoid recursion*/;
};

RVNGTextSpreadsheetGeneratorImpl::RVNGTextSpreadsheetGeneratorImpl(RVNGStringVector &sheets, const bool isInfo)
	: m_sheets(sheets)
	, m_stream()
	, m_rowStream()
	, m_cellStream()
	, m_numRowRepeated(0)
	, m_numCellRepeated(0)
	, m_numCellToSkip(0)
	, m_column(0), m_row(0)
	, m_numColumnsInRow(0)
	, m_isInfo(isInfo)
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
	m_impl->m_row = m_impl->m_column = 0;
}
void RVNGTextSpreadsheetGenerator::closeSheet()
{
	if (--m_impl->m_sheetLevel) return;
	m_impl->m_sheets.append(m_impl->m_stream.str().c_str());
	m_impl->m_stream.str("");
}

void RVNGTextSpreadsheetGenerator::openSheetRow(const RVNGPropertyList &propList)
{
	if (m_impl->m_sheetLevel!=1) return;
	m_impl->m_rowStream.str("");
	int row = propList["librevenge:row"] ? propList["librevenge:row"]->getInt() : m_impl->m_row;
	// check for unseen rows but bound them to 10
	for (int r=m_impl->m_row; r<row && r<m_impl->m_row+10; ++r)
		m_impl->m_stream << '\n';

	if (propList["table:number-rows-repeated"] && propList["table:number-rows-repeated"]->getInt()>1)
		m_impl->m_numRowRepeated=propList["table:number-rows-repeated"]->getInt();
	else
		m_impl->m_numRowRepeated=1;
	m_impl->m_row = row;
	m_impl->m_column = m_impl->m_numColumnsInRow = 0;
}
void RVNGTextSpreadsheetGenerator::closeSheetRow()
{
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1) return;
	// in general, repeated value is used to repeat empty rows, so bound it to 10
	for (int r=0; r<m_impl->m_numRowRepeated && r<10; ++r)
		m_impl->m_stream << m_impl->m_rowStream.str() << '\n';
	m_impl->m_row+=m_impl->m_numRowRepeated;
	m_impl->m_numRowRepeated=0;
}

void RVNGTextSpreadsheetGenerator::openSheetCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1) return;
	m_impl->m_cellStream.str("");
	int column = propList["librevenge:column"] ? propList["librevenge:column"]->getInt() : m_impl->m_column;
	m_impl->m_column=column;
	if (propList["table:number-columns-repeated"] && propList["table:number-columns-repeated"]->getInt()>1)
		m_impl->m_numCellRepeated=propList["table:number-columns-repeated"]->getInt();
	else
		m_impl->m_numCellRepeated=1;
	if (propList["table:number-columns-spanned"] && propList["table:number-columns-spanned"]->getInt()>1)
		m_impl->m_numCellToSkip=propList["table:number-columns-spanned"]->getInt()-1;
	else
		m_impl->m_numCellToSkip=0;
	if (propList["table:number-matrix-columns-spanned"] && propList["table:number-matrix-columns-spanned"]->getInt()>1)
		m_impl->m_numCellToSkip+=propList["table:number-matrix-columns-spanned"]->getInt()-1;

	if (!propList["librevenge:value-type"])
		return;
	// now we need to retrieve the value in proplist
	std::string valueType(propList["librevenge:value-type"]->getStr().cstr());
	if (valueType=="double" || valueType=="scientific") valueType="float";
	else if (valueType=="percent") valueType="percentage";

	if (propList["librevenge:value"] && (valueType=="float" || valueType=="percentage" || valueType=="currency"))
	{
		if (valueType=="percentage") m_impl->m_cellStream << 100. * propList["librevenge:value"]->getDouble() << "%";
		else
		{
			m_impl->m_cellStream << propList["librevenge:value"]->getDouble();
			if (valueType=="currency") m_impl->m_cellStream << "$";
		}
	}
	else if (propList["librevenge:value"] && (valueType=="bool" || valueType=="boolean"))
		m_impl->m_cellStream << (propList["librevenge:value"]->getInt() ? "true" : "false");
	else if (valueType=="date")
	{
		int day=propList["librevenge:day"] ? propList["librevenge:day"]->getInt() : 1;
		int month=propList["librevenge:month"] ? propList["librevenge:month"]->getInt() : 1;
		int year=propList["librevenge:year"] ? propList["librevenge:year"]->getInt() : 2000;
		m_impl->m_cellStream << std::setfill('0') << std::setw(2) << month << "/"
		                     << std::setfill('0') << std::setw(2) << day << "/"
		                     << std::setfill('0') << std::setw(2) << year;
	}
	else if (valueType=="time")
	{
		int hour=propList["librevenge:hours"] ? propList["librevenge:hours"]->getInt() : 0;
		int minute=propList["librevenge:minutes"] ? propList["librevenge:minutes"]->getInt() : 0;
		int second=propList["librevenge:seconds"] ? propList["librevenge:seconds"]->getInt() : 0;
		m_impl->m_cellStream << std::setfill('0') << std::setw(2) << hour << ":"
		                     << std::setfill('0') << std::setw(2) << minute << ":"
		                     << std::setfill('0') << std::setw(2) << second;
	}
	else if (valueType!="string" && valueType!="text")
	{
		RVNG_DEBUG_MSG(("RVNGTextSpreadsheetGenerator::openSheetCell: unexpected value type: %s\n", valueType.c_str()));
	}
}
void RVNGTextSpreadsheetGenerator::closeSheetCell()
{
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1) return;

	bool isEmpty=m_impl->m_cellStream.str().empty();
	if (isEmpty)
	{
		m_impl->m_column+=m_impl->m_numCellRepeated+m_impl->m_numCellToSkip;
		return;
	}
	// time to add the empty cell
	for (int i=m_impl->m_numColumnsInRow; i < m_impl->m_column; ++i)
	{
		if (i)
			m_impl->m_rowStream << static_cast<char>(UCS_TAB);
	}
	if (m_impl->m_column)
		m_impl->m_rowStream << static_cast<char>(UCS_TAB);
	for (int i=0; i<m_impl->m_numCellRepeated; ++i)
		m_impl->m_rowStream << m_impl->m_cellStream.str();
	m_impl->m_numColumnsInRow = m_impl->m_column+m_impl->m_numCellRepeated;
	m_impl->m_column=m_impl->m_numColumnsInRow+m_impl->m_numCellToSkip;
}

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
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1)
		return;
	m_impl->m_cellStream << ' ';
}

void RVNGTextSpreadsheetGenerator::defineCharacterStyle(const RVNGPropertyList &) {}
void RVNGTextSpreadsheetGenerator::openSpan(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeSpan() {}

void RVNGTextSpreadsheetGenerator::openLink(const RVNGPropertyList & /* propList */) {}
void RVNGTextSpreadsheetGenerator::closeLink() {}

void RVNGTextSpreadsheetGenerator::insertTab()
{
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1)
		return;
	m_impl->m_cellStream << static_cast<char>(UCS_TAB);
}

void RVNGTextSpreadsheetGenerator::insertText(const RVNGString &text)
{
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1)
		return;
	m_impl->m_cellStream << text.cstr();
}

void RVNGTextSpreadsheetGenerator::insertSpace()
{
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1)
		return;
	m_impl->m_cellStream << ' ';
}

void RVNGTextSpreadsheetGenerator::insertLineBreak()
{
	if (m_impl->m_isInfo || m_impl->m_sheetLevel!=1)
		return;
	m_impl->m_cellStream << ' ';
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
	m_impl->m_row = m_impl->m_column = 0;
}
void RVNGTextSpreadsheetGenerator::closeTable()
{
	--m_impl->m_sheetLevel;
}
void RVNGTextSpreadsheetGenerator::openTableRow(const RVNGPropertyList &propList)
{
	openSheetRow(propList);
}
void RVNGTextSpreadsheetGenerator::closeTableRow()
{
	closeSheetRow();
}
void RVNGTextSpreadsheetGenerator::openTableCell(const RVNGPropertyList &propList)
{
	openSheetCell(propList);
}
void RVNGTextSpreadsheetGenerator::closeTableCell()
{
	closeSheetCell();
}
void RVNGTextSpreadsheetGenerator::insertCoveredTableCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_sheetLevel!=1) return;
	if (propList["table:number-columns-repeated"] && propList["table:number-columns-repeated"]->getInt()>1)
		m_impl->m_column+=propList["table:number-columns-repeated"]->getInt();
	else
		++m_impl->m_column;
}

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
