/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP6FIXEDLENGTHGROUP_H
#define WP6FIXEDLENGTHGROUP_H

#include "WP6Part.h"

class WP6FixedLengthGroup : public WP6Part
{
 public:
	WP6FixedLengthGroup(uint8_t groupID);
	static WP6FixedLengthGroup * constructFixedLengthGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID);
	static bool isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID);

 	uint8_t getGroup() const { return m_group; }
 
 protected:
	void _read(WPXInputStream *input, WPXEncryption *encryption);
	virtual void _readContents(WPXInputStream *input, WPXEncryption *encryption) = 0; // we always read the contents in the case of a fixed length group
 
 private:
	 uint8_t m_group;
};

#endif /* WP6FIXEDLENGTHGROUP_H */
