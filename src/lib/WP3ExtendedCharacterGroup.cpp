/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2002 Ariya Hidayat <ariyahidayat@yahoo.de>
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP3ExtendedCharacterGroup.h"
#include "WP3FileStructure.h"
#include "WP3Listener.h"
#include "libwpd_internal.h"

WP3ExtendedCharacterGroup::WP3ExtendedCharacterGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP3FixedLengthGroup(groupID),
	m_macCharacter(0),
	m_characterSet(0),
	m_character(0)
{
	_read(input, encryption);
}

void WP3ExtendedCharacterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_macCharacter = readU8(input, encryption);
	m_characterSet = readU8(input, encryption);
	m_character = readU8(input, encryption);
}

void WP3ExtendedCharacterGroup::parse(WP3Listener *listener)
{
	// We are finding the map first in the macRoman character set
	// and only if we cannot find it, we use the WP character set
	// map. It seems to be the behaviour of WordPerfect 3.x for Mac.
	if (m_macCharacter >= 0x20)
	{
		listener->insertCharacter(macRomanCharacterMap[m_macCharacter - 0x20]);
	}
	else if ((m_characterSet != 0xFF) || ((m_character != 0xFE) && (m_character != 0xFF)))
	{
		const uint32_t *chars;
		int len = extendedCharacterWP5ToUCS4(m_character,
		                                     m_characterSet, &chars);
		int i;

		for (i = 0; i < len; i++)
			listener->insertCharacter(chars[i]);
	}

}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
