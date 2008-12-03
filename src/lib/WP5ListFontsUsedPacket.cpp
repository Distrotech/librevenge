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

#include "WP5ListFontsUsedPacket.h"
#include "WP5FileStructure.h"
#include "WP5Parser.h"
#include "libwpd_internal.h"

WP5ListFontsUsedPacket::WP5ListFontsUsedPacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, uint32_t dataOffset, uint32_t dataSize, uint16_t packetType) :
	WP5GeneralPacketData(),
	m_packetType(packetType),
	m_fontNameOffset(),
	m_fontSize()
{	
	_read(input, encryption, dataOffset, dataSize);
}

WP5ListFontsUsedPacket::~WP5ListFontsUsedPacket()
{
}

void WP5ListFontsUsedPacket::_readContents(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataSize)
{
	int numFonts = (int)(dataSize / 86); // 86 == size of the structure describing the font
	WPD_DEBUG_MSG(("WP5 List Fonts Used Packet, data size: %i, number fonts: %i\n", dataSize, numFonts));
	int tempFontNameOffset;
	double tempFontSize;
	for (int i=0; i<numFonts; i++)
	{
		input->seek(18, WPX_SEEK_CUR);
		tempFontNameOffset=(int)readU16(input, encryption);
		if (m_packetType == WP50_LIST_FONTS_USED_PACKET)
		{
			input->seek(2, WPX_SEEK_CUR);
			tempFontSize=(double)(readU16(input, encryption) / 50);
			input->seek(62, WPX_SEEK_CUR);
		}
		else
		{
			input->seek(27, WPX_SEEK_CUR);
			tempFontSize=(double)(readU16(input, encryption) / 50);
			input->seek(37, WPX_SEEK_CUR);
		}
		WPD_DEBUG_MSG(("WP5 List Fonts Used Packet, font number: %i, font name offset: %i, font size, %.4f\n", i, tempFontNameOffset, tempFontSize));
		m_fontNameOffset.push_back(tempFontNameOffset);
		m_fontSize.push_back(tempFontSize);
	}
}

int WP5ListFontsUsedPacket::getFontNameOffset(const int fontNumber) const
{
	if ((fontNumber >= 0) && ((long)fontNumber < (long)m_fontNameOffset.size()))
		return m_fontNameOffset[fontNumber];
	return 0;
}

double WP5ListFontsUsedPacket::getFontSize(const int fontNumber) const
{
	if ((fontNumber >= 0) && ((long)fontNumber < (long)m_fontSize.size()))
		return m_fontSize[fontNumber];
	return 0.0;
}

