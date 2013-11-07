/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002-2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <iostream>
#include <sstream>

#include <librevenge/librevenge.h>

#include "librevenge_internal.h"

#include "RVNGHTMLTextTableStyle.h"

namespace librevenge
{
void RVNGHTMLTextTableStyleManager::openTable(RVNGPropertyListVector const &colList)
{
	std::vector<double> colWidths;
	for (unsigned long i = 0; i < colList.count(); i++)
	{
		RVNGPropertyList const &prop=colList[i];
		double width=0;
		if (prop["style:column-width"])
		{
			RVNGUnit unit=prop["style:column-width"]->getUnit();
			if (unit==RVNG_POINT)
				width=prop["style:column-width"]->getDouble()/72.;
			else if (unit==RVNG_INCH)
				width=prop["style:column-width"]->getDouble();
			else if (unit==RVNG_TWIP)
				width=prop["style:column-width"]->getDouble()/1440.;
		}
		colWidths.push_back(width);
	}
	m_columWitdhsStack.push_back(colWidths);
}

void RVNGHTMLTextTableStyleManager::closeTable()
{
	if (!m_columWitdhsStack.size())
	{
		RVNG_DEBUG_MSG(("RVNGHTMLTextTableStyleManager::closeTable: can not find the columns witdh\n"));
		return;
	}
	m_columWitdhsStack.pop_back();
}

bool RVNGHTMLTextTableStyleManager::getColumnsWidth(int col, int numSpanned, double &w) const
{
	if (!m_columWitdhsStack.size())
		return false;
	std::vector<double> const &widths=m_columWitdhsStack.back();
	if (col < 0 || size_t(col+numSpanned-1) >= widths.size())
	{
		RVNG_DEBUG_MSG(("RVNGHTMLTextTableStyleManager::getColumnsWidth: can not compute the columns witdh\n"));
		return false;
	}
	bool fixed = true;
	w = 0;
	for (size_t i=size_t(col); i < size_t(col+numSpanned); i++)
	{
		if (widths[i] < 0)
		{
			w += -widths[i];
			fixed = false;
		}
		else if (widths[i] > 0)
			w += widths[i];
		else
		{
			w = 0;
			return true;
		}
	}
	if (!fixed) w = -w;
	return true;
}

std::string RVNGHTMLTextTableStyleManager::getCellClass(RVNGPropertyList const &pList)
{
	std::string content=getCellContent(pList);
	std::map<std::string, std::string>::iterator it=m_cellContentNameMap.find(content);
	if (it != m_cellContentNameMap.end())
		return it->second;
	std::stringstream s;
	s << "cellTable" << m_cellContentNameMap.size();
	m_cellContentNameMap[content]=s.str();
	return s.str();
}

std::string RVNGHTMLTextTableStyleManager::getRowClass(RVNGPropertyList const &pList)
{
	std::string content=getRowContent(pList);
	std::map<std::string, std::string>::iterator it=m_rowContentNameMap.find(content);
	if (it != m_rowContentNameMap.end())
		return it->second;
	std::stringstream s;
	s << "rowTable" << m_rowContentNameMap.size();
	m_rowContentNameMap[content]=s.str();
	return s.str();
}

void RVNGHTMLTextTableStyleManager::send(std::ostream &out)
{
	std::map<std::string, std::string>::iterator it=m_cellContentNameMap.begin();
	while (it != m_cellContentNameMap.end())
	{
		out << "." << it->second << " " << it->first << "\n";
		++it;
	}
	it=m_rowContentNameMap.begin();
	while (it != m_rowContentNameMap.end())
	{
		out << "." << it->second << " " << it->first << "\n";
		++it;
	}
}

std::string RVNGHTMLTextTableStyleManager::getCellContent(RVNGPropertyList const &pList) const
{
	std::stringstream s;
	s << "{\n";
	// try to get the cell width
	if (pList["librevenge:column"])
	{
		int c=pList["librevenge:column"]->getInt();
		int span=1;
		if (pList["table:number-columns-spanned"])
			span = pList["table:number-columns-spanned"]->getInt();
		double w;
		if (!getColumnsWidth(c,span,w))
		{
			RVNG_DEBUG_MSG(("RVNGHTMLTextTableStyleManager::getCellContent: can not find columns witdth for %d[%d]\n", c, span));
		}
		else if (w > 0)
			s << "\twidth:" << w << "in;\n";
		else if (w < 0)
			s << "\tmin-width:" << -w << "in;\n";
	}
	if (pList["fo:text-align"])
	{
		if (pList["fo:text-align"]->getStr() == RVNGString("end")) // stupid OOo convention..
			s << "\ttext-align:right;\n";
		else
			s << "\ttext-align:" << pList["fo:text-align"]->getStr().cstr() << ";\n";
	}
	if (pList["style:vertical-align"])
		s << "\tvertical-align:" << pList["style:vertical-align"]->getStr().cstr() << ";\n";
	else
		s << "\tvertical-align:top;\n";
	if (pList["fo:background-color"])
		s << "\tbackground-color:" << pList["fo:background-color"]->getStr().cstr() << ";\n";

	static char const *(type[]) = {"border", "border-left", "border-top", "border-right", "border-bottom" };
	for (int i = 0; i < 5; i++)
	{
		std::string field("fo:");
		field+=type[i];
		if (!pList[field.c_str()])
			continue;
		s << "\t" << type[i] << ": " << pList[field.c_str()]->getStr().cstr() << ";\n";
	}

	s << "}";
	return s.str();
}

std::string RVNGHTMLTextTableStyleManager::getRowContent(RVNGPropertyList const &pList) const
{
	std::stringstream s;
	s << "{\n";
	if (pList["style:min-row-height"])
		s << "\tmin-height:" << pList["style:min-row-height"]->getStr().cstr() << ";\n";
	else if (pList["style:row-height"])
		s << "\theight:" << pList["style:row-height"]->getStr().cstr() << ";\n";
	s << "}";
	return s.str();
}
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
