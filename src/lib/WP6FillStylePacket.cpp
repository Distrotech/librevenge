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

#include "WP6FillStylePacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6FillStylePacket::WP6FillStylePacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, uint32_t dataOffset, uint32_t dataSize) :
	WP6PrefixDataPacket(input, encryption),
	m_fgColor(0xff, 0xff, 0xff),
	m_bgColor(0xff, 0xff, 0xff)
{
	_read(input, encryption, dataOffset, dataSize);
}


WP6FillStylePacket::~WP6FillStylePacket()
{

}

const int WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_PREFIX_PACKETS = 6;
const int WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_FILL_NAME = 3;

void WP6FillStylePacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	/* skip a whole bunch of useless crap */
	uint16_t numChildPrefixIDs = readU16(input, encryption);
	input->seek(sizeof(uint16_t)*numChildPrefixIDs, WPX_SEEK_CUR);
	input->seek(WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_PREFIX_PACKETS, WPX_SEEK_CUR);
	int16_t fillNameLength = readS16(input, encryption);
	if (fillNameLength > 0)
		input->seek(fillNameLength, WPX_SEEK_CUR);
	input->seek(WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_FILL_NAME, WPX_SEEK_CUR);

	/* now we can finally grab the meat */
	m_fgColor.m_r = readU8(input, encryption);
	m_fgColor.m_g = readU8(input, encryption);
	m_fgColor.m_b = readU8(input, encryption);
	m_fgColor.m_s = readU8(input, encryption);
	m_bgColor.m_r = readU8(input, encryption);
	m_bgColor.m_g = readU8(input, encryption);
	m_bgColor.m_b = readU8(input, encryption);
	m_bgColor.m_s = readU8(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: Fill Prefix Packet FG Color (%i, %i, %i, %i) BG Color (%i, %i, %i, %i)\n",
	               m_fgColor.m_r, m_fgColor.m_g, m_fgColor.m_b, m_fgColor.m_s,
	               m_bgColor.m_r, m_bgColor.m_g, m_bgColor.m_b, m_bgColor.m_s));

}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
