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
#include <string.h>

#include "WP5FontNameStringPoolPacket.h"
#include "WP5Parser.h"
#include "libwpd_internal.h"

WP5FontNameStringPoolPacket::WP5FontNameStringPoolPacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, uint32_t dataOffset, uint32_t dataSize) :
	WP5GeneralPacketData(),
	m_fontNameString()
{	
	_read(input, encryption, dataOffset, dataSize);
}

WP5FontNameStringPoolPacket::~WP5FontNameStringPoolPacket()
{
}

void WP5FontNameStringPoolPacket::_readContents(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataSize)
{
	unsigned int tmpInitialOffset = input->tell();
	while ((long)input->tell() < (long)(tmpInitialOffset + dataSize))
	{
		unsigned int offset = input->tell() - tmpInitialOffset;
		WPXString fontName = readCString(input, encryption);
		m_fontNameString[offset] = fontName;
	}
	
	for (std::map<unsigned int, WPXString>::const_iterator Iter = m_fontNameString.begin(); Iter != m_fontNameString.end(); Iter++)
		WPD_DEBUG_MSG(("WP5 Font Name String Pool Packet: offset: %i font name: %s\n", Iter->first, (Iter->second).cstr()));
}

WPXString WP5FontNameStringPoolPacket::getFontName(const unsigned int offset) const
{
	std::map<unsigned int, WPXString>::const_iterator Iter = m_fontNameString.find(offset);
	if (Iter != m_fontNameString.end())
		return Iter->second;
	// if the offset is not correct, return the default value
	return WPXString("Times New Roman");
}

