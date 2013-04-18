/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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
#include <string.h>

#include "WP6OutlineStylePacket.h"
#include "libwpd_internal.h"

WP6OutlineStylePacket::WP6OutlineStylePacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, uint32_t dataOffset, uint32_t dataSize) :
	WP6PrefixDataPacket(input, encryption),
	m_numPIDs(0),
	m_outlineHash(0),
	m_outlineFlags(0),
	m_tabBehaviourFlag(0)
{
	for (unsigned i = 0; i < WP6_NUM_LIST_LEVELS; i++)
		m_numberingMethods[i] = 0;
	_read(input, encryption, dataOffset, dataSize);
}

WP6OutlineStylePacket::~WP6OutlineStylePacket()
{
}

void WP6OutlineStylePacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_numPIDs = readU16(input, encryption);
	input->seek(2 * WP6_NUM_LIST_LEVELS, WPX_SEEK_CUR);
#if 0
	for (i=0; i<WP6_NUM_LIST_LEVELS; i++)
		m_paragraphStylePIDs[i] = readU16(input, encryption); // seemingly useless
#endif
	m_outlineFlags = readU8(input, encryption);
	m_outlineHash = readU16(input, encryption);
	for (unsigned i=0; i<WP6_NUM_LIST_LEVELS; i++)
		m_numberingMethods[i] = readU8(input, encryption);
	m_tabBehaviourFlag = readU8(input, encryption);

	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (numPrefixIDs: %i, outlineHash: %i, outlineFlags: %i, tab behaviour flag: %i)\n", (int) m_numPIDs, (int) m_outlineHash, (int) m_outlineFlags, (int) m_tabBehaviourFlag));
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_numberingMethods: %i %i %i %i %i %i %i %i)\n",
	               m_numberingMethods[0], m_numberingMethods[1], m_numberingMethods[2], m_numberingMethods[3],
	               m_numberingMethods[4], m_numberingMethods[5], m_numberingMethods[6], m_numberingMethods[7]));

}

void WP6OutlineStylePacket::parse(WP6Listener *listener) const
{
	listener->updateOutlineDefinition(indexHeader, m_outlineHash, m_numberingMethods, m_tabBehaviourFlag);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
