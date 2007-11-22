/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#ifndef WP6PREFIXINDICE_H
#define WP6PREFIXINDICE_H
#include "WPXStream.h"
#include "libwpd_types.h"

class WPXEncryption;

class WP6PrefixIndice
{
 public:
	WP6PrefixIndice(WPXInputStream * input, WPXEncryption *encryption, int id);	
	int getID() const { return m_id; }
	uint8_t getType() const { return m_type; }
	uint8_t getFlags() const { return m_flags; }
	uint32_t getDataSize() const { return m_dataSize; }
	uint32_t getDataOffset() const { return m_dataOffset; }

 protected:
 	void _read(WPXInputStream *input, WPXEncryption *encryption);
 
 private:
	int m_id;
	uint8_t m_type;
 	uint8_t m_flags;
 	uint16_t m_useCount;
 	uint16_t m_hideCount;
 	uint32_t m_dataSize;
 	uint32_t m_dataOffset;
 
 	bool m_hasChildren;
};

#endif /* WP6PREFIXINDICE_H */
