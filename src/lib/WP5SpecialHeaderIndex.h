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

#ifndef WP5SPECIALHEADERINDEX_H
#define WP5SPECIALHEADERINDEX_H
#include "libwpd_types.h"
#include "WPXStream.h"
#include "WPXEncryption.h"

class WP5SpecialHeaderIndex
{
 public:
	WP5SpecialHeaderIndex(WPXInputStream * input, WPXEncryption *encryption);	
	uint16_t getType() const { return m_type; }
	uint16_t getNumOfIndexes() const { return m_numOfIndexes; }
	uint16_t getIndexBlockSize() const { return m_indexBlockSize; }
	uint32_t getNextBlockOffset() const { return m_nextBlockOffset; }

 protected:
 	void _read(WPXInputStream *input, WPXEncryption *encryption);
 
 private:
	uint16_t m_type;
	uint16_t m_numOfIndexes;
 	uint16_t m_indexBlockSize;
 	uint32_t m_nextBlockOffset;
};

#endif /* WP5SPECIALHEADERINDEX_H */
