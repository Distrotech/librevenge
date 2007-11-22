/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5GeneralPacketIndex.h"
#include "libwpd_internal.h"

WP5GeneralPacketIndex::WP5GeneralPacketIndex(WPXInputStream * input, WPXEncryption *encryption, int id)
	: m_id(id),
	  m_type(0),
	  m_dataSize(0),
	  m_dataOffset(0)
{
	_read(input, encryption);
}

void WP5GeneralPacketIndex::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	m_type = readU16(input, encryption);

	m_dataSize = readU32(input, encryption);
	m_dataOffset = readU32(input, encryption);

	WPD_DEBUG_MSG(("General Packet Index (id: %i, type: %i, data size: %u, data offset: %u)\n", m_id, m_type, m_dataSize, m_dataOffset));
}
