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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP5DefinitionGroup.h"
#include "WP5LLListener.h"
#include "WPXHLListener.h"
#include "libwpd_internal.h"

WP5DefinitionGroup_DefineTablesSubGroup::WP5DefinitionGroup_DefineTablesSubGroup(WPXInputStream *input) :
	WP5VariableLengthGroup_SubGroup(),
	m_position(0),
	m_numColumns(0),
	m_leftGutter(0),
	m_rightGutter(0),
	m_leftOffset(0)
{
	// Skip useless old values to read the old column number
	input->seek(2, WPX_SEEK_CUR);
	m_numColumns = readU16(input);
	// Skip to new values
	input->seek(20+(5*m_numColumns), WPX_SEEK_CUR);
	// Read the new values
	uint8_t tmpFlags = readU8(input);
	m_position = tmpFlags & 0x07;
	input->seek(1, WPX_SEEK_CUR);
	m_numColumns = readU16(input);
	input->seek(4, WPX_SEEK_CUR);
	m_leftGutter = readU16(input);
	m_rightGutter = readU16(input);
	input->seek(10, WPX_SEEK_CUR);
	m_leftOffset = readU16(input);
	int i;
	for (i=0; i < m_numColumns; i++)
		m_columnWidth[i] = readU16(input);
	for (i=0; i < m_numColumns; i++)
		m_attributeBits[i] = readU16(input);
	for (i=0; i < m_numColumns; i++)
		m_columnAlignment[i] = readU8(input);
}

void WP5DefinitionGroup_DefineTablesSubGroup::parse(WP5HLListener *listener)
{
	listener->defineTable(m_position, m_leftOffset);
	for (int i=0; i < m_numColumns; i++)
	{
		listener->addTableColumnDefinition(m_columnWidth[i], m_leftGutter, m_rightGutter, m_attributeBits[i], m_columnAlignment[i]);
	}
	listener->startTable();
}


WP5DefinitionGroup::WP5DefinitionGroup(WPXInputStream *input) :	
	WP5VariableLengthGroup(),
	m_subGroupData(NULL)
{
	_read(input);
}

WP5DefinitionGroup::~WP5DefinitionGroup()
{
	delete m_subGroupData;
}

void WP5DefinitionGroup::_readContents(WPXInputStream *input)
{
	switch(getSubGroup())
	{
		case WP5_TOP_DEFINITION_GROUP_DEFINE_TABLES:
			m_subGroupData = new WP5DefinitionGroup_DefineTablesSubGroup(input);
			break;
		default:
			break;
	}	
}

void WP5DefinitionGroup::parse(WP5HLListener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Definition group\n"));

	switch(getSubGroup())
	{
		case WP5_TOP_DEFINITION_GROUP_DEFINE_TABLES:
			m_subGroupData->parse(listener);
			break;		
		default:
			break;		
	}
}
