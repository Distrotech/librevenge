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
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

// RVNGTable: an intermediate representation of a table, designed to be created
// "ahead of time". unlike wordperfect's table definition messages, this representation
// is _consistent_: we can always count on the messages being sent using this representation
// (once it is created and finalized) to be reliable (assuming no bugs in this code!) :-)
//
// example situation where this might be useful: WordPerfect allows two cells,
// side by side, one with border, one without-- creating a false ambiguity (none
// actually exists: if one cell does not have a border, the other doesn't either)

#ifndef _RVNGTABLE_H
#define _RVNGTABLE_H
#include <vector>
#include <stdio.h>
#include "librevenge_types.h"

typedef struct _RVNGTableCell RVNGTableCell;

struct _RVNGTableCell
{
	_RVNGTableCell(uint8_t colSpan, uint8_t rowSpan, uint8_t borderBits);
	uint8_t m_colSpan;
	uint8_t m_rowSpan;
	uint8_t m_borderBits;
};

class RVNGTable
{
public:
	RVNGTable() : m_tableRows() {}
	~RVNGTable();
	void insertRow();
	void insertCell(uint8_t colSpan, uint8_t rowSpan, uint8_t borderBits);
	const RVNGTableCell  *getCell(size_t i, size_t j)
	{
		return (m_tableRows[i])[j];
	}
	void makeBordersConsistent();
	void _makeCellBordersConsistent(RVNGTableCell *cell, std::vector<RVNGTableCell *> &adjacentCells,
	                                int adjacencyBitCell, int adjacencyBitBoundCells);
	std::vector<RVNGTableCell *>  _getCellsBottomAdjacent(int i, int j);
	std::vector<RVNGTableCell *>  _getCellsRightAdjacent(int i, int j);

	const std::vector< std::vector<RVNGTableCell *> > &getRows() const
	{
		return m_tableRows;
	}
	bool isEmpty() const
	{
		return m_tableRows.empty();
	}

private:
	std::vector< std::vector<RVNGTableCell *> > m_tableRows;
};

class RVNGTableList
{
public:
	RVNGTableList();
	RVNGTableList(const RVNGTableList &);
	RVNGTableList &operator=(const RVNGTableList &tableList);
	virtual ~RVNGTableList();

	RVNGTable *operator[](unsigned long i)
	{
		return (*m_tableList)[i];
	}
	void add(RVNGTable *table)
	{
		m_tableList->push_back(table);
	}

private:
	void release();
	void acquire(int *refCount, std::vector<RVNGTable *> *tableList);
	int *getRef() const
	{
		return m_refCount;
	}
	std::vector<RVNGTable *> *get() const
	{
		return m_tableList;
	}

	std::vector<RVNGTable *> *m_tableList;
	int *m_refCount;
};
#endif /* _RVNGTABLE_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
