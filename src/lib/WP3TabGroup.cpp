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

#include "WP3TabGroup.h"
#include "WP3FileStructure.h"
#include "WP3Listener.h"
#include "libwpd_internal.h"

WP3TabGroup::WP3TabGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP3FixedLengthGroup(groupID),
	m_subGroup(0),
	m_modeType(0),
	m_position(0.0)
{
	_read(input, encryption);
}

void WP3TabGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_subGroup = readU8(input, encryption);
	m_modeType = readU8(input, encryption);
	m_position = fixedPointToDouble(readU32(input, encryption, true)) / 72.0;
}

void WP3TabGroup::parse(WP3Listener *listener)
{
	switch (m_subGroup)
	{
	case WP3_TAB_GROUP_TAB:
		listener->insertTab();
		break;
	case WP3_TAB_GROUP_CENTER:
	case WP3_TAB_GROUP_FLUSH_RIGHT:
		listener->insertTab(m_subGroup, m_position);
		break;
	case WP3_TAB_GROUP_BACK_TAB:
		listener->backTab();
		break;
	case WP3_TAB_GROUP_CHARACTER_KERNING:
		listener->insertCharacter(0x0020);
		break;
	case WP3_TAB_GROUP_BAR_TAB:
		listener->insertTab();
		listener->insertCharacter('|');
		break;
	case WP3_TAB_GROUP_FIXED_TAB:
		listener->insertTab();
		break;
	default:
		break;
	}
}
