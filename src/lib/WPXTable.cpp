/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXTable.h"
#include "libwpd_support.h"

typedef vector<WPXTableCell *>::iterator VTCIter;
typedef vector< vector<WPXTableCell *> * >::iterator VVTCIter;

_WPXTableCell::_WPXTableCell(uint8_t colSpan, uint8_t rowSpan, bool boundFromAbove, bool boundFromLeft, uint8_t borderBits) :
	m_colSpan(colSpan),
	m_rowSpan(rowSpan),
	m_boundFromAbove(boundFromAbove),
	m_boundFromLeft(boundFromLeft),
	m_borderBits(borderBits)
{
}

WPXTable::~WPXTable()
{
	typedef vector<WPXTableCell *>::iterator VTCIter;
	typedef vector< vector<WPXTableCell *> * >::iterator VVTCIter;
	for (VVTCIter iter1 = m_tableRows.begin(); iter1 != m_tableRows.end(); iter1++)
	{
		for (VTCIter iter2 = (*iter1)->begin(); iter2 != (*iter1)->end(); iter2++) 
		{
			delete(*iter2);
		}
		delete (*iter1);
	}
}

void WPXTable::insertRow()
{
	m_tableRows.push_back(new vector<WPXTableCell *>);
}

void WPXTable::insertCell(uint8_t colSpan, uint8_t rowSpan, bool boundFromLeft, bool boundFromAbove, uint8_t borderBits)
{	
	m_tableRows[(m_tableRows.size()-1)]->push_back(new WPXTableCell(colSpan, rowSpan, boundFromLeft, boundFromAbove, borderBits));
}

// makeConsistent: make the table border specification (defined per-cell) consistent, with no
// duplicated borders
// pre-condition: the table must be completely built/read before this function is called
void WPXTable::makeBordersConsistent()
{
	// make the top/bottom table borders consistent
  	for(int i=0; i<m_tableRows.size(); i++)
	{
  		for (int j=0; j<m_tableRows[i]->size(); j++)
  		{
			if (!(*m_tableRows[i])[j]->m_boundFromLeft && !(*m_tableRows[i])[j]->m_boundFromAbove)
			{
				if (i < (m_tableRows.size()-1)) 
				{
					vector<WPXTableCell *> *cellsBottomAdjacent = _getCellsBottomAdjacent(i, j);
					_makeCellBordersConsistent((*m_tableRows[i])[j], cellsBottomAdjacent, 
							       WPX_TABLE_CELL_BOTTOM_BORDER_OFF, WPX_TABLE_CELL_TOP_BORDER_OFF);
					delete cellsBottomAdjacent;		
				}
				if (j < (m_tableRows[i]->size()-1))
				{
					vector<WPXTableCell *> *cellsRightAdjacent = _getCellsRightAdjacent(i, j);
					_makeCellBordersConsistent((*m_tableRows[i])[j], cellsRightAdjacent, 
							       WPX_TABLE_CELL_RIGHT_BORDER_OFF, WPX_TABLE_CELL_LEFT_BORDER_OFF);
					delete cellsRightAdjacent;
				}
			}
		} 
	}
}

void WPXTable::_makeCellBordersConsistent(WPXTableCell *cell, vector<WPXTableCell *> *adjacentCells, 
				      int adjacencyBitCell, int adjacencyBitBoundCells)
{
	typedef vector<WPXTableCell *>::iterator VTCIter;
	if (adjacentCells->size() > 0) 
	{
		// if this cell is adjacent to > 1 cell, and it has no border
		// make the cells below have no border
		// NB: there is a corner case where this will not work but it
		// is not resolvable given how WP/OOo define table borders. see BUGS
		if (cell->m_borderBits & adjacencyBitCell)
		{
			for (VTCIter iter = adjacentCells->begin(); iter != adjacentCells->end(); iter++) 
			{
				(*iter)->m_borderBits ^= adjacencyBitBoundCells;
			}
		}
		// otherwise we can get the same effect by bottom border from
		// this cell-- if the adjacent cells have/don't have borders, this will be
		// picked up automatically
		else
			cell->m_borderBits ^= adjacencyBitCell;
	}
}

vector<WPXTableCell *> * WPXTable::_getCellsBottomAdjacent(int i, int j)
{
	int bottomAdjacentRow = i + (*m_tableRows[i])[j]->m_rowSpan;
	vector<WPXTableCell *> * cellsBottomAdjacent = new vector<WPXTableCell *>;

	if (bottomAdjacentRow >= m_tableRows.size()) 
		return cellsBottomAdjacent;
	
	for (int j1=0; j1<m_tableRows[bottomAdjacentRow]->size(); j1++)
	{
		if (!(*m_tableRows[bottomAdjacentRow])[j1]->m_boundFromAbove &&
		    !(*m_tableRows[bottomAdjacentRow])[j1]->m_boundFromLeft &&
		    ((j1 + (*m_tableRows[bottomAdjacentRow])[j1]->m_colSpan) > j) &&
		    (j1 < (j + (*m_tableRows[i])[j]->m_colSpan)))
		{
			cellsBottomAdjacent->push_back((*m_tableRows[bottomAdjacentRow])[j1]);
		}		
	}

	return cellsBottomAdjacent;
}

vector<WPXTableCell *> * WPXTable::_getCellsRightAdjacent(int i, int j)
{
	int rightAdjacentCol = j + (*m_tableRows[i])[j]->m_colSpan;
	vector<WPXTableCell *> * cellsRightAdjacent = new vector<WPXTableCell *>;

	if (rightAdjacentCol >= m_tableRows[i]->size()) // num cols is uniform across table: this comparison is valid
		return cellsRightAdjacent;
	
	for(int i1=0; i1<m_tableRows.size(); i1++)
	{
		if ((*m_tableRows[i1]).size() > rightAdjacentCol) // ignore cases where the right adjacent column 
		{                                                 // pushes us beyond table borders (FIXME: good idea?)
			if (!(*m_tableRows[i1])[rightAdjacentCol]->m_boundFromAbove &&
			    !(*m_tableRows[i1])[rightAdjacentCol]->m_boundFromLeft &&
			    ((i1 + (*m_tableRows[i1])[rightAdjacentCol]->m_rowSpan) > i) &&
			    (i1 < (i + (*m_tableRows[i])[j]->m_rowSpan)))
			{
				cellsRightAdjacent->push_back((*m_tableRows[i1])[rightAdjacentCol]);
			}
		}
	}

	return cellsRightAdjacent;
}

WPXTableList::WPXTableList() :
	m_refCount(new int),
	m_tableList(new vector<WPXTable *>)
{
	(*m_refCount) = 1;
}

WPXTableList::WPXTableList(const WPXTableList &tableList) :
	m_tableList(tableList.get())
{
	acquire(tableList.getRef(), tableList.get());
}

WPXTableList & WPXTableList::operator=(const WPXTableList & tableList)
{
	if (this != &tableList) 
	{
		release();
		acquire(tableList.getRef(), tableList.get());
	}
	
	return (*this);
}

void WPXTableList::acquire(int *refCount, vector<WPXTable *> *tableList)
{ 
	m_refCount = refCount; 
	m_tableList = tableList; 
	if (m_refCount) 
		(*m_refCount)++; 
}

void WPXTableList::release()
{
	if (m_refCount) 
	{ 
		if (--(*m_refCount) == 0) 
		{ 
			for (vector<WPXTable *>::iterator iter = (*m_tableList).begin(); iter != (*m_tableList).end(); iter++)
				delete (*iter);
			delete m_tableList; 
			delete m_refCount; 
		} 
		m_refCount = NULL; 
		m_tableList = NULL; 
	}
}

WPXTableList::~WPXTableList()
{ 	
	release();
}
