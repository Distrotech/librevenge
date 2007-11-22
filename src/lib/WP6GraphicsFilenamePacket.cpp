/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell Inc. (http://www.novell.com)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
#include <string.h>

#include "WP6GraphicsFilenamePacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6GraphicsFilenamePacket::WP6GraphicsFilenamePacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, const uint8_t flags, uint32_t dataOffset, uint32_t dataSize): 
	WP6PrefixDataPacket(input, encryption),
	m_childIds(),
	m_flags(flags)
{	
	_read(input, encryption, dataOffset, dataSize);
}

WP6GraphicsFilenamePacket::~WP6GraphicsFilenamePacket()
{
}

void WP6GraphicsFilenamePacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	if ((m_flags && 0x01) == 0x00)
		return;
	uint16_t tmpNumChildIds = readU16(input, encryption);
	for (uint16_t i = 0; i < tmpNumChildIds; i++)
		m_childIds.push_back(readU16(input, encryption));
}

