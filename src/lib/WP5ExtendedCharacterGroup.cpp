/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2002 Ariya Hidayat <ariyahidayat@yahoo.de>
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5ExtendedCharacterGroup.h"
#include "WP5FileStructure.h"
#include "libwpd_internal.h"
#include "WP5Listener.h"

WP5ExtendedCharacterGroup::WP5ExtendedCharacterGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP5FixedLengthGroup(groupID),
	m_character(0),
	m_characterSet(0)
{
	_read(input, encryption);
}

void WP5ExtendedCharacterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_character = readU8(input, encryption);
	m_characterSet = readU8(input, encryption);
}

void WP5ExtendedCharacterGroup::parse(WP5Listener *listener)
{
	const uint16_t *chars;
	int len = extendedCharacterWP5ToUCS2(m_character,
				  	m_characterSet, &chars);
	for (int i = 0; i < len; i++)
		listener->insertCharacter(chars[i]);
}
