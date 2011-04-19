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

#include "WP3DoubleByteScriptCharacterGroup.h"
#include "WP3FileStructure.h"
#include "WP3Listener.h"
#include "libwpd_internal.h"

WP3DoubleByteScriptCharacterGroup::WP3DoubleByteScriptCharacterGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP3FixedLengthGroup(groupID),
	m_scriptCharacter(0)
{
	_read(input, encryption);
}

void WP3DoubleByteScriptCharacterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_scriptCharacter = readU16(input, encryption, true);
}

void WP3DoubleByteScriptCharacterGroup::parse(WP3Listener *listener)
{
	// We assume for the time being that the script characters are
	// actually 16 bit Unicode characters in Big Endian.
	if (m_scriptCharacter)
	{
		const uint16_t *chars;
		int len = appleWorldScriptToUCS2(m_scriptCharacter, &chars);
		for (int i = 0; i < len; i++)
			listener->insertCharacter(chars[i]);
	}
}
