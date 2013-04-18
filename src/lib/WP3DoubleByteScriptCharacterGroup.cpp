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
		const uint32_t *chars;
		int len = appleWorldScriptToUCS4(m_scriptCharacter, &chars);
		for (int i = 0; i < len; i++)
			listener->insertCharacter(chars[i]);
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
