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

#ifndef RVNGHTMLTEXT_TABLESTYLE_H
#define RVNGHTMLTEXT_TABLESTYLE_H

#include <librevenge/librevenge.h>

#include <ostream>
#include <string>
#include <map>
#include <vector>

namespace librevenge
{
/** Small class to manage the tables style */
class RVNGHTMLTextTableStyleManager
{
public:
	//! constructor
	RVNGHTMLTextTableStyleManager() : m_cellContentNameMap(), m_rowContentNameMap(), m_columWitdhsStack()
	{
	}
	//! destructor
	~RVNGHTMLTextTableStyleManager()
	{
	}
	//! open a table
	void openTable(RVNGPropertyListVector const &colList);
	//! close a table
	void closeTable();
	//! returns the class name corresponding to a propertylist
	std::string getCellClass(RVNGPropertyList const &pList);
	//! returns the class name corresponding to a propertylist
	std::string getRowClass(RVNGPropertyList const &pList);
	//! send the data to the stream
	void send(std::ostream &out);
private:
	//! convert a property list in a html content string
	std::string getCellContent(RVNGPropertyList const &pList) const;
	//! convert a property list in a html content string
	std::string getRowContent(RVNGPropertyList const &pList) const;
	//! try to return the col width
	bool getColumnsWidth(int i, int numSpanned, double &w) const;
	//! a map cell content -> name
	std::map<std::string, std::string> m_cellContentNameMap;
	//! a map row content -> name
	std::map<std::string, std::string> m_rowContentNameMap;
	//! a stack of column width (in inches )
	std::vector<std::vector<double> > m_columWitdhsStack;

	RVNGHTMLTextTableStyleManager(RVNGHTMLTextTableStyleManager const &orig);
	RVNGHTMLTextTableStyleManager operator=(RVNGHTMLTextTableStyleManager const &orig);
};
}
#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
