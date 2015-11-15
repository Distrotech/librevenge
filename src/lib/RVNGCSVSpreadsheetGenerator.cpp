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

#include <time.h>

#include <string>
#include <sstream>
#include <vector>

#include <librevenge-generators/librevenge-generators.h>

#include "librevenge_internal.h"

namespace librevenge
{

struct RVNGCSVSpreadsheetRowContent
{
	//!constructor
	RVNGCSVSpreadsheetRowContent() : m_content(""), m_numFields(0), m_numRepeated(1)
	{
	}
	//! the content
	std::string m_content;
	//! the number of fields
	int m_numFields;
	//! the number of repetition
	int m_numRepeated;
};

struct RVNGCSVSpreadsheetGeneratorImpl
{
	explicit RVNGCSVSpreadsheetGeneratorImpl(RVNGStringVector &sheets, bool useFormula) :
		m_sheets(sheets), m_rowStream(), m_cellStream(), m_rowContentsList(),
		m_numRowRepeated(0), m_numCellRepeated(0), m_numCellToSkip(0),
		m_useFormula(useFormula),
		m_fieldSeparator(','), m_textSeparator('"'), m_decimalSeparator('.'),
		m_dateFormat("%m/%d/%y"), m_timeFormat("%H:%M:%S"),
		m_inSheet(false), m_inSheetRow(false), m_inSheetCell(false), m_sheetCellHasFormula(false),
		m_numberSubForm(0),
		m_column(0), m_row(0), m_numColumns(0), m_numColumnsInRowStream(0) {}
	bool canWriteData(bool newCell=false) const
	{
		if (!m_inSheet || !m_inSheetRow || m_numberSubForm) return false;
		if (newCell) return true;
		return m_inSheetCell && !m_sheetCellHasFormula;
	}
	void insertCharacter(char c)
	{
		if (c==m_textSeparator)
			m_cellStream << m_textSeparator;
		m_cellStream << c;
	}
	void insertDouble(double val)
	{
		std::stringstream s;
		s << val;
		std::string res=s.str();
		if (m_decimalSeparator!='.')
		{
			std::string::size_type pos = res.find_last_of('.');
			if (pos != std::string::npos) res[pos]=m_decimalSeparator;
		}
		m_cellStream << res;
	}
	void insertInstruction(librevenge::RVNGPropertyList const &instruction);
	void storeActualSpreadsheet();
	RVNGStringVector &m_sheets;
	//! the actual row stream
	std::ostringstream m_rowStream;
	//! the actual cell stream
	std::ostringstream m_cellStream;
	//! the content of each row
	std::vector<RVNGCSVSpreadsheetRowContent> m_rowContentsList;
	//! the number of repetition of the actual row
	int m_numRowRepeated;
	//! the number of repetition of the actual cell
	int m_numCellRepeated;
	//! the number of empty cell which follow current cell
	int m_numCellToSkip;
	bool m_useFormula;
	char m_fieldSeparator, m_textSeparator, m_decimalSeparator;
	std::string m_dateFormat, m_timeFormat;
	bool m_inSheet, m_inSheetRow, m_inSheetCell, m_sheetCellHasFormula;
	int m_numberSubForm;
	int m_column/** the current column*/, m_row/*the current row*/, m_numColumns/**the total number of column*/;
	int m_numColumnsInRowStream/** the number of column written in rowStream*/;
};

void RVNGCSVSpreadsheetGeneratorImpl::insertInstruction(librevenge::RVNGPropertyList const &instr)
{
	if (!instr["librevenge:type"])
	{
		RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction can not find formula type !!!\n"));
		return;
	}
	std::string type(instr["librevenge:type"]->getStr().cstr());
	if (type=="librevenge-operator")
	{
		if (!instr["librevenge:operator"])
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction can not find operator for formula!!!\n"));
			return;
		}
		m_cellStream << instr["librevenge:operator"]->getStr().cstr();
		return;
	}
	if (type=="librevenge-function")
	{
		if (!instr["librevenge:function"])
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction can not find function for formula!!!\n"));
			return;
		}
		m_cellStream << instr["librevenge:function"]->getStr().cstr();
		return;
	}
	if (type=="librevenge-number")
	{
		if (!instr["librevenge:number"])
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction can not find value for formula!!!\n"));
			return;
		}
		insertDouble(instr["librevenge:number"]->getDouble());
		return;
	}
	if (type=="librevenge-text")
	{
		if (!instr["librevenge:text"])
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction can not find text for formula!!!\n"));
			return;
		}
		std::string text(instr["librevenge:text"]->getStr().cstr());
		insertCharacter('"');
		for (size_t t=0; t < text.length(); ++t)
			insertCharacter((char) text[t]);
		insertCharacter('"');
		return;
	}
	if (type=="librevenge-cell")
	{
		if (!instr["librevenge:row"]||!instr["librevenge:column"])
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction can not find formula cordinate!!!\n"));
			return;
		}
		int column=instr["librevenge:column"]->getInt();
		// we need to add 1 because we have added an header row
		int row=instr["librevenge:row"]->getInt()+1;
		if (column<0 || row<0)
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction: find bad coordinate for formula!!!\n"));
			return;
		}
		if (instr["librevenge:column-absolute"] && instr["librevenge:column-absolute"]->getInt()) m_cellStream << "$";
		if (column>=26) m_cellStream << char('A'+(column/26-1));
		m_cellStream << char('A'+(column%26));
		if (instr["librevenge:row-absolute"] && instr["librevenge:row-absolute"]->getInt()) m_cellStream << "$";
		m_cellStream << row+1;
		return;
	}
	if (type=="librevenge-cells")
	{
		if (!instr["librevenge:start-row"]||!instr["librevenge:start-column"])
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction can not find formula cordinate!!!\n"));
			return;
		}
		int column=instr["librevenge:start-column"]->getInt();
		// we need to add 1 because we have added an header row
		int row=instr["librevenge:start-row"]->getInt()+1;
		if (column<0 || row<0)
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction: find bad coordinate1 for formula!!!\n"));
			return;
		}
		if (instr["librevenge:start-column-absolute"] && instr["librevenge:start-column-absolute"]->getInt()) m_cellStream << "$";
		if (column>=26) m_cellStream << char('A'+(column/26-1));
		m_cellStream << char('A'+(column%26));
		if (instr["librevenge:start-row-absolute"] && instr["librevenge:start-row-absolute"]->getInt()) m_cellStream << "$";
		m_cellStream << row+1 << ":";
		if (instr["librevenge:end-column"])
			column=instr["librevenge:end-column"]->getInt();
		// we need to add 1 because we have added an header row
		if (instr["librevenge:end-row"])
			row=instr["librevenge:end-row"]->getInt()+1;
		if (column<0 || row<0)
		{
			RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction: find bad coordinate2 for formula!!!\n"));
			return;
		}
		if (instr["librevenge:end-column-absolute"] && instr["librevenge:end-column-absolute"]->getInt()) m_cellStream << "$";
		if (column>=26) m_cellStream << char('A'+(column/26-1));
		m_cellStream << char('A'+(column%26));
		if (instr["librevenge:end-row-absolute"] && instr["librevenge:end-row-absolute"]->getInt()) m_cellStream << "$";
		m_cellStream << row+1;
		return;
	}
	RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGeneratorImpl::insertInstruction find unknown type %s!!!\n", type.c_str()));
}

void RVNGCSVSpreadsheetGeneratorImpl::storeActualSpreadsheet()
{
	std::stringstream stream;
	for (int i=0; i < m_numColumns; ++i)
	{
		if (i)
			stream << m_fieldSeparator;
		stream << "Col" << i+1;
	}
	stream << "\n";

	for (size_t i=0; i<m_rowContentsList.size(); ++i)
	{
		RVNGCSVSpreadsheetRowContent const &row=m_rowContentsList[i];
		if (row.m_numRepeated<=0) continue;
		m_rowStream.str("");
		m_rowStream << row.m_content;
		// add empty cells
		for (int c=row.m_numFields; c < m_numColumns; ++c)
		{
			if (c)
				m_rowStream << m_fieldSeparator;
			m_rowStream << m_textSeparator << m_textSeparator;
		}
		m_rowStream << "\n";
		for (int r=0; r<row.m_numRepeated; ++r)
			stream << m_rowStream.str();
	}
	m_sheets.append(stream.str().c_str());
}

RVNGCSVSpreadsheetGenerator::RVNGCSVSpreadsheetGenerator(RVNGStringVector &sheets, bool generateFormula) :
	m_impl(new RVNGCSVSpreadsheetGeneratorImpl(sheets,generateFormula))
{
}

RVNGCSVSpreadsheetGenerator::~RVNGCSVSpreadsheetGenerator()
{
	delete m_impl;
}

void RVNGCSVSpreadsheetGenerator::setSeparators(char fieldSep, char textSep, char decimalSep)
{
	m_impl->m_fieldSeparator=fieldSep;
	m_impl->m_textSeparator=textSep;
	m_impl->m_decimalSeparator=decimalSep;
}

void RVNGCSVSpreadsheetGenerator::setDTFormats(RVNGString const &date, RVNGString const &time)
{
	m_impl->m_dateFormat=date.cstr();
	m_impl->m_timeFormat=time.cstr();
}

void RVNGCSVSpreadsheetGenerator::setDocumentMetaData(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::defineEmbeddedFont(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::startDocument(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::endDocument() {}

void RVNGCSVSpreadsheetGenerator::defineSheetNumberingStyle(const RVNGPropertyList &) {}

void RVNGCSVSpreadsheetGenerator::openSheet(const RVNGPropertyList &)
{
	if (m_impl->m_numberSubForm || m_impl->m_inSheet)
	{
		++m_impl->m_numberSubForm;
		return;
	}
	m_impl->m_inSheet=true;
	m_impl->m_row = m_impl->m_column = 0;
	m_impl->m_numColumns = 0;
}

void RVNGCSVSpreadsheetGenerator::closeSheet()
{
	if (m_impl->m_numberSubForm)
	{
		--m_impl->m_numberSubForm;
		return;
	}
	m_impl->storeActualSpreadsheet();
	m_impl->m_inSheet=false;
}

void RVNGCSVSpreadsheetGenerator::openSheetRow(const RVNGPropertyList &propList)
{
	if (m_impl->m_numberSubForm || !m_impl->m_inSheet || m_impl->m_inSheetRow)
	{
		++m_impl->m_numberSubForm;
		return;
	}
	int row = propList["librevenge:row"] ? propList["librevenge:row"]->getInt() : m_impl->m_row;
	if (m_impl->m_row < row)
	{
		m_impl->m_rowContentsList.push_back(RVNGCSVSpreadsheetRowContent());
		m_impl->m_rowContentsList.back().m_numRepeated=row-m_impl->m_row;
	}
	m_impl->m_rowStream.str("");
	if (propList["table:number-rows-repeated"] && propList["table:number-rows-repeated"]->getInt()>1)
		m_impl->m_numRowRepeated=propList["table:number-rows-repeated"]->getInt();
	else
		m_impl->m_numRowRepeated=1;
	m_impl->m_inSheetRow=true;
	m_impl->m_row = row;
	m_impl->m_column = m_impl->m_numColumnsInRowStream = 0;
}

void RVNGCSVSpreadsheetGenerator::closeSheetRow()
{
	if (m_impl->m_numberSubForm)
	{
		--m_impl->m_numberSubForm;
		return;
	}
	m_impl->m_rowContentsList.push_back(RVNGCSVSpreadsheetRowContent());
	RVNGCSVSpreadsheetRowContent &row=m_impl->m_rowContentsList.back();
	row.m_content=m_impl->m_rowStream.str();
	row.m_numFields=m_impl->m_numColumnsInRowStream;
	row.m_numRepeated=m_impl->m_numRowRepeated;
	if (m_impl->m_numColumnsInRowStream>m_impl->m_numColumns)
		m_impl->m_numColumns=m_impl->m_numColumnsInRowStream;
	m_impl->m_row+=m_impl->m_numRowRepeated;
	m_impl->m_numRowRepeated=0;
	m_impl->m_inSheetRow=false;
}

void RVNGCSVSpreadsheetGenerator::openSheetCell(const RVNGPropertyList &propList)
{
	if (m_impl->m_numberSubForm || !m_impl->m_inSheetRow || m_impl->m_inSheetCell)
	{
		++m_impl->m_numberSubForm;
		return;
	}
	int column = propList["librevenge:column"] ? propList["librevenge:column"]->getInt() : m_impl->m_column;
	m_impl->m_column=column;
	m_impl->m_inSheetCell=true;
	m_impl->m_sheetCellHasFormula=false;
	m_impl->m_cellStream.str("");
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
	if (m_impl->m_useFormula)
	{
		librevenge::RVNGPropertyListVector const *formula=propList.child("librevenge:formula");
		if (formula && formula->count())
		{
			m_impl->m_sheetCellHasFormula=true;
			m_impl->insertCharacter('=');
			for (unsigned long s=0; s<formula->count(); ++s)
				m_impl->insertInstruction((*formula)[s]);
			return;
		}
	}
	if (!propList["librevenge:value-type"]) return;

	// now we need to retrieve the value in proplist
	std::string valueType(propList["librevenge:value-type"]->getStr().cstr());
	if (valueType=="double" || valueType=="scientific") valueType="float";
	else if (valueType=="percent") valueType="percentage";

	if (propList["librevenge:value"] && (valueType=="float" || valueType=="percentage" || valueType=="currency"))
	{
		if (valueType=="percentage")
		{
			m_impl->insertDouble(100. * propList["librevenge:value"]->getDouble());
			m_impl->m_cellStream << "%";
		}
		else
		{
			// if (valueType=="currency") m_impl->m_cellStream << "$";
			m_impl->insertDouble(propList["librevenge:value"]->getDouble());
		}
	}
	else if (propList["librevenge:value"] && (valueType=="bool" || valueType=="boolean"))
	{
		if (propList["librevenge:value"]->getDouble()<0||propList["librevenge:value"]->getDouble()>0)
			m_impl->m_cellStream << "true";
		else
			m_impl->m_cellStream << "false";
	}
	else if (valueType=="date" || valueType=="time")
	{
		// checkme
		struct tm time;
		time.tm_sec=time.tm_min=time.tm_hour=0;
		time.tm_mday=time.tm_mon=1;
		time.tm_year=100;
		time.tm_wday=time.tm_yday=time.tm_isdst=-1;
		char buf[256];
		if (valueType=="date")
		{
			time.tm_mday=propList["librevenge:day"] ? propList["librevenge:day"]->getInt() : 1;
			time.tm_mon=propList["librevenge:month"] ? propList["librevenge:month"]->getInt()-1 : 0;
			time.tm_year=propList["librevenge:year"] ? propList["librevenge:year"]->getInt()-1900 : 100;
			if (mktime(&time)!=-1 && strftime(buf, 256, m_impl->m_dateFormat.c_str(), &time))
				m_impl->m_cellStream << buf;
		}
		else
		{
			time.tm_hour=propList["librevenge:hours"] ? propList["librevenge:hours"]->getInt() : 0;
			time.tm_min=propList["librevenge:minutes"] ? propList["librevenge:minutes"]->getInt() : 0;
			time.tm_sec=propList["librevenge:seconds"] ? propList["librevenge:seconds"]->getInt() : 0;
			if (mktime(&time)!=-1 && strftime(buf, 256, m_impl->m_timeFormat.c_str(), &time))
				m_impl->m_cellStream << buf;
		}
	}
	else if (valueType!="string" && valueType!="text")
	{
		RVNG_DEBUG_MSG(("RVNGCSVSpreadsheetGenerator::openSheetCell: unexpected value type: %s\n", valueType.c_str()));
	}
}

void RVNGCSVSpreadsheetGenerator::closeSheetCell()
{
	if (m_impl->m_numberSubForm)
	{
		--m_impl->m_numberSubForm;
		return;
	}

	m_impl->m_inSheetCell=false;
	if (m_impl->m_cellStream.str().empty())
	{
		m_impl->m_column+=m_impl->m_numCellRepeated+m_impl->m_numCellToSkip;
		return;
	}
	// time to add the empty cell
	for (int i=m_impl->m_numColumnsInRowStream; i < m_impl->m_column; ++i)
	{
		if (i)
			m_impl->m_rowStream << m_impl->m_fieldSeparator;
		m_impl->m_rowStream << m_impl->m_textSeparator << m_impl->m_textSeparator;
	}
	// add the current cell
	for (int i=0; i<m_impl->m_numCellRepeated; ++i)
	{
		if (i || m_impl->m_column)
			m_impl->m_rowStream << m_impl->m_fieldSeparator;
		m_impl->m_rowStream << m_impl->m_textSeparator << m_impl->m_cellStream.str() << m_impl->m_textSeparator;
	}
	m_impl->m_numColumnsInRowStream = m_impl->m_column+m_impl->m_numCellRepeated;
	m_impl->m_column=m_impl->m_numColumnsInRowStream+m_impl->m_numCellToSkip;
}

void RVNGCSVSpreadsheetGenerator::defineChartStyle(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::openChart(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeChart()
{
	--m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::openChartTextObject(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeChartTextObject() {}
void RVNGCSVSpreadsheetGenerator::openChartPlotArea(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeChartPlotArea() {}
void RVNGCSVSpreadsheetGenerator::insertChartAxis(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::openChartSerie(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeChartSerie() {}

void RVNGCSVSpreadsheetGenerator::definePageStyle(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::openPageSpan(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closePageSpan() {}
void RVNGCSVSpreadsheetGenerator::openHeader(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeHeader()
{
	--m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::openFooter(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeFooter()
{
	--m_impl->m_numberSubForm;
}


void RVNGCSVSpreadsheetGenerator::defineSectionStyle(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::openSection(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeSection() {}

void RVNGCSVSpreadsheetGenerator::defineParagraphStyle(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::openParagraph(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeParagraph() {}

void RVNGCSVSpreadsheetGenerator::defineCharacterStyle(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::openSpan(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeSpan() {}

void RVNGCSVSpreadsheetGenerator::openLink(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeLink() {}

void RVNGCSVSpreadsheetGenerator::insertTab()
{
	if (!m_impl->canWriteData())
		return;
	m_impl->insertCharacter('\t');
}

void RVNGCSVSpreadsheetGenerator::insertText(const RVNGString &text)
{
	if (!m_impl->canWriteData())
		return;
	char const *data=text.cstr();
	for (unsigned long c=0; c<text.size(); ++c)
		m_impl->insertCharacter(data[c]);
}

void RVNGCSVSpreadsheetGenerator::insertSpace()
{
	if (!m_impl->canWriteData())
		return;
	m_impl->insertCharacter(' ');
}

void RVNGCSVSpreadsheetGenerator::insertLineBreak()
{
	if (!m_impl->canWriteData())
		return;
	m_impl->insertCharacter(' ');
}

void RVNGCSVSpreadsheetGenerator::insertField(const RVNGPropertyList &) {}

void RVNGCSVSpreadsheetGenerator::openOrderedListLevel(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::openUnorderedListLevel(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeOrderedListLevel() {}
void RVNGCSVSpreadsheetGenerator::closeUnorderedListLevel() {}
void RVNGCSVSpreadsheetGenerator::openListElement(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeListElement() {}

void RVNGCSVSpreadsheetGenerator::openFootnote(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeFootnote()
{
	--m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::openEndnote(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeEndnote()
{
	--m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::openComment(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeComment()
{
	--m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::openTextBox(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeTextBox()
{
	--m_impl->m_numberSubForm;
}

void RVNGCSVSpreadsheetGenerator::openTable(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeTable()
{
	--m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::openTableRow(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeTableRow() {}
void RVNGCSVSpreadsheetGenerator::openTableCell(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::closeTableCell() {}
void RVNGCSVSpreadsheetGenerator::insertCoveredTableCell(const RVNGPropertyList &) {}

void RVNGCSVSpreadsheetGenerator::openFrame(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeFrame()
{
	--m_impl->m_numberSubForm;
}

void RVNGCSVSpreadsheetGenerator::openGroup(const RVNGPropertyList &)
{
	++m_impl->m_numberSubForm;
}
void RVNGCSVSpreadsheetGenerator::closeGroup()
{
	--m_impl->m_numberSubForm;
}

void RVNGCSVSpreadsheetGenerator::defineGraphicStyle(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::drawRectangle(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::drawEllipse(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::drawPolygon(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::drawPolyline(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::drawPath(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::drawConnector(const RVNGPropertyList &) {}

void RVNGCSVSpreadsheetGenerator::insertBinaryObject(const RVNGPropertyList &) {}
void RVNGCSVSpreadsheetGenerator::insertEquation(const RVNGPropertyList &) {}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
