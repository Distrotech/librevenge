/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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
	unsigned numFonts = dataSize / 86; // 86 == size of the structure describing the font
	WPD_DEBUG_MSG(("WP5 List Fonts Used Packet, data size: %u, number fonts: %u\n", dataSize, numFonts));
	unsigned tempFontNameOffset;
	double tempFontSize;
	for (unsigned i=0; i<numFonts; i++)
	{
		input->seek(18, WPX_SEEK_CUR);
		tempFontNameOffset=readU16(input, encryption);
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
		WPD_DEBUG_MSG(("WP5 List Fonts Used Packet, font number: %u, font name offset: %u, font size, %.4f\n", i, tempFontNameOffset, tempFontSize));
		m_fontNameOffset.push_back(tempFontNameOffset);
		m_fontSize.push_back(tempFontSize);
	}
}

unsigned WP5ListFontsUsedPacket::getFontNameOffset(unsigned fontNumber) const
{
	if (fontNumber < m_fontNameOffset.size())
		return m_fontNameOffset[fontNumber];
	return 0;
}

double WP5ListFontsUsedPacket::getFontSize(unsigned fontNumber) const
{
	if (fontNumber < m_fontSize.size())
		return m_fontSize[fontNumber];
	return 0.0;
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
