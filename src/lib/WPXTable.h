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

// WPXTable: an intermediate representation of a table, designed to be created
// "ahead of time". unlike wordperfect's table definition messages, this representation
// is _consistent_: we can always count on the messages being sent using this representation
// (once it is created and finalized) to be reliable (assuming no bugs in this code!) :-) 
//
// example situation where this might be useful: WordPerfect allows two cells, 
// side by side, one with border, one without-- creating a false ambiguity (none 
// actually exists: if one cell does not have a border, the other doesn't either) 

#ifndef _WPXTABLE_H
#define _WPXTABLE_H
#include <vector>
#include <glib.h>

using namespace std;

typedef struct _WPXTableCell WPXTableCell;

struct _WPXTableCell
{
	_WPXTableCell(guint8 colSpan, guint8 rowSpan, bool boundFromLeft, bool boundFromAbove, guint8 borderBits);
	guint8 m_colSpan;
	guint8 m_rowSpan;
	bool m_boundFromLeft, m_boundFromAbove;
	guint8 m_borderBits;
};

class WPXTable
{
public:
	~WPXTable();
	void insertRow();
	void insertCell(guint8 colSpan, guint8 rowSpan, bool boundFromLeft, bool boundFromAbove, guint8 borderBits);
	const WPXTableCell * getCell(int i, int j) { return (*m_tableRows[i])[j]; }
	void makeBordersConsistent();
	void _makeCellBordersConsistent(WPXTableCell *cell, vector<WPXTableCell *> *adjacentCells, 
				      int adjacencyBitCell, int adjacencyBitBoundCells);
	vector<WPXTableCell *> * _getCellsBottomAdjacent(int i, int j);
	vector<WPXTableCell *> * _getCellsRightAdjacent(int i, int j);
private:
	vector< vector<WPXTableCell *> * > m_tableRows;
};
#endif /* _WPXTABLE_H */
