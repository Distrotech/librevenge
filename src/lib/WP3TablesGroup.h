/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP3TABLESGROUP_H
#define WP3TABLESGROUP_H

#include "WP3VariableLengthGroup.h"
#include "libwpd_internal.h"

class WP3TablesGroup : public WP3VariableLengthGroup
{
 public:
	WP3TablesGroup(WPXInputStream *input, WPXEncryption *encryption);	
	~WP3TablesGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP3Listener *listener);

 private:
	// variables needed for subgroup 1 (Table Function)
	uint8_t m_tableMode;
	uint32_t m_offsetFromLeftEdge;
	uint32_t m_topGutterSpacing;
	uint32_t m_leftGutterSpacing;
	uint32_t m_bottomGutterSpacing;
	uint32_t m_rightGutterSpacing;
	uint8_t m_numColumns;
	uint8_t m_columnMode[32];
	uint8_t m_numberFormat[32];
	uint32_t m_columnWidth[32];
	uint32_t m_rightOffsetForDecimalAlign[32];
	
	// variables needed for subgroup 2 (Set Table Cell Span)
	uint16_t m_colSpan;
	uint16_t m_rowSpan;
	
	// variables needed for subgroup 0x0B (Set Table Cell Fill Color/Patern)
	RGBSColor m_cellFillColor;
};

#endif /* WP3TABLESGROUP_H */
