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
	int16_t fillNameLength = readU16(input, encryption);
	if (fillNameLength > 0)
		input->seek(fillNameLength*sizeof(uint8_t), WPX_SEEK_CUR);
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
