/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#ifndef WP6COLUMNGROUP_H
#define WP6COLUMNGROUP_H

#include "WP6VariableLengthGroup.h"
#include <vector>
#include "libwpd_internal.h"

class WPXInputStream;
class WP6Listener;

class WP6ColumnGroup : public WP6VariableLengthGroup
{
 public:
	WP6ColumnGroup(WPXInputStream *input, WPXEncryption *encryption);	
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

 private:
	// variables needed for subgroup 0 and 1 (Left/Right Margin Set)
	uint16_t m_margin;
 
 	// variables used for subgroup 2 (Columns)
	uint8_t m_colType;
	uint8_t m_numColumns;
	double m_rowSpacing;
	std::vector<bool> m_isFixedWidth;
	std::vector<double> m_columnWidth;
};

#endif /* WP6COLUMNGROUP_H */
