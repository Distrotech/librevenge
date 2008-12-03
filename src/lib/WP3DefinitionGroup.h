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

#ifndef WP3DEFINITIONGROUP_H
#define WP3DEFINITIONGROUP_H

#include "WP3VariableLengthGroup.h"
#include "libwpd_types.h"
#include <vector>

class WPXInputStream;
class WP3Listener;

class WP3DefinitionGroup : public WP3VariableLengthGroup
{
 public:
	WP3DefinitionGroup(WPXInputStream *input, WPXEncryption *encryption);	
	~WP3DefinitionGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP3Listener *listener);

 private:
 	/* Variables used for subgroup 0x01 Set Columns On/Off */
	uint8_t m_colType;
	uint8_t m_numColumns;
	std::vector<bool> m_isFixedWidth;
	std::vector<double> m_columnWidth;

};

#endif /* WP3DefinitionGroup_H */
