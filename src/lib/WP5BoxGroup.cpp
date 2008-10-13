/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5BoxGroup.h"
#include "WPXListener.h"
#include "libwpd_internal.h"
#include "WP5FileStructure.h"
#include "WP5PrefixData.h"
#include "WP5Listener.h"
#include "WP5GraphicsInformationPacket.h"

WP5BoxGroup::WP5BoxGroup(WPXInputStream *input, WPXEncryption *encryption) :	
	WP5VariableLengthGroup(),
	m_boxNumber(0),
	m_positionAndType(0),
	m_alignment(0),
	m_width(0),
	m_height(0),
	m_x(0),
	m_y(0),
	m_boxType(0),
	m_graphicsOffset(0),
	m_data(NULL)
{
	_read(input, encryption);
}

void WP5BoxGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch(getSubGroup())
	{
		case WP5_TOP_BOX_GROUP_FIGURE:
			m_boxNumber = readU16(input, encryption);
			m_positionAndType = readU8(input, encryption);
			m_alignment = readU8(input, encryption);
			m_width = readU16(input, encryption);
			m_height = readU16(input, encryption);
			m_x = readU16(input, encryption);
			m_y = readU16(input, encryption);
			input->seek(36, WPX_SEEK_CUR);
			m_boxType = readU8(input, encryption);
			if (m_boxType == 0x80)
			{
				input->seek(60, WPX_SEEK_CUR);
				m_graphicsOffset = readU16(input, encryption);
			}
			break;
		case WP5_TOP_BOX_GROUP_TABLE:
		case WP5_TOP_BOX_GROUP_TEXT_BOX:
		case WP5_TOP_BOX_GROUP_USER_DEFINED_BOX:
		case WP5_TOP_BOX_GROUP_EQUATION:
			break;
		case WP5_TOP_BOX_GROUP_HORIZONTAL_LINE:
			break;
		case WP5_TOP_BOX_GROUP_VERTICAL_LINE:
			break;
		default:
			break;
	}	
}

void WP5BoxGroup::parse(WP5Listener * listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Box group\n"));

	switch(getSubGroup())
	{
		case WP5_TOP_BOX_GROUP_FIGURE:
			if (m_boxType != 0x80)
				break;
			if (listener->getGeneralPacketData(8))
				m_data = static_cast<const WP5GraphicsInformationPacket *>(listener->getGeneralPacketData(8))->getImage(m_graphicsOffset);
			if (m_data)
			{
				listener->boxOn(m_positionAndType, m_alignment, m_width, m_height, m_x, m_y);
				listener->insertGraphicsData(m_data);
				listener->boxOff();
			}	
			break;
				
		case WP5_TOP_BOX_GROUP_TABLE:
		case WP5_TOP_BOX_GROUP_TEXT_BOX:
		case WP5_TOP_BOX_GROUP_USER_DEFINED_BOX:
		case WP5_TOP_BOX_GROUP_EQUATION:
			break;
		case WP5_TOP_BOX_GROUP_HORIZONTAL_LINE:
			break;
		case WP5_TOP_BOX_GROUP_VERTICAL_LINE:
			break;
		default:
			break;		
	}
}
