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
 * MERCHANINDENTILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
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

#include "WP3IndentGroup.h"
#include "WP3FileStructure.h"
#include "WP3Listener.h"
#include "libwpd_internal.h"

WP3IndentGroup::WP3IndentGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP3FixedLengthGroup(groupID),
	m_subGroup(0),
	m_offset(0.0)
{
	_read(input, encryption);
}

void WP3IndentGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_subGroup = readU8(input, encryption);
	m_offset = fixedPointToDouble(readU32(input, encryption, true));
}

void WP3IndentGroup::parse(WP3Listener *listener)
{
	switch (m_subGroup)
	{
	case WP3_INDENT_GROUP_LEFT_INDENT:
		listener->leftIndent(m_offset);
		break;
	case WP3_INDENT_GROUP_LEFT_RIGHT_INDENT:
		listener->leftRightIndent(m_offset);
		break;
	default:
		break;
	}
}
