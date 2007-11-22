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

#include "WP5SpecialHeaderIndex.h"
#include "libwpd_internal.h"

WP5SpecialHeaderIndex::WP5SpecialHeaderIndex(WPXInputStream * input, WPXEncryption *encryption) :
	  m_type(0),
	  m_numOfIndexes(0),
	  m_indexBlockSize(0),
	  m_nextBlockOffset(0)
{
	_read(input, encryption);
}

void WP5SpecialHeaderIndex::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	m_type = readU16(input, encryption);
	
	m_numOfIndexes = readU16(input, encryption);

	m_indexBlockSize = readU16(input, encryption);
	m_nextBlockOffset = readU32(input, encryption);

	WPD_DEBUG_MSG(("Special Header Index (type: %i, number of indexes: %i, index block size: %i, next block offset: %i)\n",
			m_type, m_numOfIndexes, m_indexBlockSize, m_nextBlockOffset));
}
