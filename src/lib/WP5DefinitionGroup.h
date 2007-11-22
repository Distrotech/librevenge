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

#ifndef WP5DEFINITIONGROUP_H
#define WP5DEFINITIONGROUP_H

#include "WP5VariableLengthGroup.h"
#include "WP5FileStructure.h"

class WP5DefinitionGroup_DefineTablesSubGroup : public WP5VariableLengthGroup_SubGroup
{
public:
	WP5DefinitionGroup_DefineTablesSubGroup(WPXInputStream *input, WPXEncryption *encryption, uint16_t subGroupSize);
	void parse(WP5Listener *listener);

private:
	uint8_t m_position;
	uint16_t m_numColumns;
	uint16_t m_leftOffset;
	uint16_t m_leftGutter;
	uint16_t m_rightGutter;
	uint16_t m_columnWidth[32];
	uint16_t m_attributeBits[32];
	uint8_t m_columnAlignment[32];

};

class WP5DefinitionGroup : public WP5VariableLengthGroup
{
public:
	WP5DefinitionGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP5DefinitionGroup();	
	void parse(WP5Listener *listener);
	
protected:
	virtual void _readContents(WPXInputStream *input, WPXEncryption *encryption);

private:
	WP5DefinitionGroup(const WP5DefinitionGroup&);
	WP5DefinitionGroup& operator=(const WP5DefinitionGroup&);
	WP5VariableLengthGroup_SubGroup * m_subGroupData;
};

#endif /* WP5DEFINITIONGROUP_H */
