/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell Inc. (http://www.novell.com)
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

#include "WP6GraphicsBoxStylePacket.h"
#include "WP6Parser.h"

WP6GraphicsBoxStylePacket::WP6GraphicsBoxStylePacket(WPXInputStream *input, WPXEncryption *encryption, int  /* id */, uint32_t dataOffset, uint32_t dataSize):
	WP6PrefixDataPacket(input, encryption),
	m_isLibraryStyle(false),
	m_boxStyleName(),
	m_generalPositioningFlags(0x00),
	m_horizontalPositioningFlags(0x00),
	m_horizontalOffset(0),
	m_leftColumn(0x00),
	m_rightColumn(0x00),
	m_verticalPositioningFlags(0x00),
	m_verticalOffset(0),
	m_widthFlags(0x00),
	m_width(0),
	m_heightFlags(0x00),
	m_height(0),
	m_contentType(0x00),
	m_contentHAlign(0),
	m_contentVAlign(0),
	m_contentPreserveAspectRatio(true),
	m_nativeWidth(0),
	m_nativeHeight(0)
{
	_read(input, encryption, dataOffset, dataSize);
}

WP6GraphicsBoxStylePacket::~WP6GraphicsBoxStylePacket()
{
}

void WP6GraphicsBoxStylePacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket position: 0x%.8x\n", (unsigned)input->tell()));
	uint16_t tmpNumChildIDs = readU16(input, encryption);
	input->seek(tmpNumChildIDs * 2, WPX_SEEK_CUR);
	uint16_t tmpSizeOfBoxData = readU16(input, encryption);
	long tmpStartOfBoxData = input->tell();

	// Reading the box name/library data

	uint16_t tmpSizeOfBoxNameLibraryData = readU16(input, encryption);
	long tmpBoxNameLibraryDataPosition = input->tell();
	input->seek(1, WPX_SEEK_CUR);
	m_isLibraryStyle = ((readU8(input, encryption) & 0x01) != 0x00);
	int16_t tmpBoxNameLength = (int16_t)readU16(input, encryption);

	if (tmpBoxNameLength > 0)
	{
		for (int16_t i = 0; i < (tmpBoxNameLength/2); i++)
		{
			uint16_t charWord = readU16(input, encryption);
			uint8_t characterSet = (uint8_t)((charWord >> 8) & 0x00FF);
			uint8_t character = (uint8_t)(charWord & 0xFF);

			if (character == 0x00 && characterSet == 0x00)
				break;

			const uint32_t *chars;
			int len = extendedCharacterWP6ToUCS4(character, characterSet, &chars);

			for (int j = 0; j < len; j++)
				appendUCS4(m_boxStyleName, (uint32_t)chars[j]);
		}
	}
	else
	{
		switch (tmpBoxNameLength)
		{
		case 0:
			m_boxStyleName = "Figure Box";
			break;
		case -1:
			m_boxStyleName = "Table Box";
			break;
		case -2:
			m_boxStyleName = "Text Box";
			break;
		case -3:
			m_boxStyleName = "User Box";
			break;
		case -4:
			m_boxStyleName = "Equation Box";
			break;
		case -5:
			m_boxStyleName = "Button Box";
			break;
		default:
			break;
		}
	}
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Style name: %s\n", m_boxStyleName.cstr()));
	input->seek(tmpSizeOfBoxNameLibraryData + tmpBoxNameLibraryDataPosition, WPX_SEEK_SET);

	// Skipping box counter data

	uint16_t tmpSizeOfBoxCounterData = readU16(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box counter data\n"));
	input->seek(tmpSizeOfBoxCounterData, WPX_SEEK_CUR);

	// Reading Box positioning data

	uint16_t tmpSizeOfBoxPositioningData = readU16(input, encryption);
	long tmpBoxPositioningDataPosition = input->tell();
	input->seek(1, WPX_SEEK_CUR);

	m_generalPositioningFlags = readU8(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Positioning data (general positioning flags: 0x%.2x)\n", m_generalPositioningFlags));
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Positioning data (anchor value: %i) (page offset bit: %i) (overlap flag: %i) (auto flag: %i)\n",
	               m_generalPositioningFlags & 0x07, (m_generalPositioningFlags & 0x08) >> 3, (m_generalPositioningFlags & 0x10) >> 4, (m_generalPositioningFlags & 0x20) >> 5));

	m_horizontalPositioningFlags = readU8(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Horizontal position (horizontal alignment type: %i) (horizontal alignment: %i)\n",
	               m_horizontalPositioningFlags & 0x03, (m_horizontalPositioningFlags & 0x1C) >> 2));

	m_horizontalOffset = (int16_t)readU16(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Horizontal Offset (%i)\n", m_horizontalOffset));

	m_leftColumn = readU8(input, encryption);
	m_rightColumn = readU8(input, encryption);

	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Horizontal position (between columns %i and %i)\n", m_leftColumn, m_rightColumn));

	m_verticalPositioningFlags = readU8(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Vertical position (vertical alignment type: %i) (vertical alignment: %i) (vertical effect: %i)\n",
	               m_verticalPositioningFlags & 0x03, (m_verticalPositioningFlags & 0x1C) >> 2, (m_verticalPositioningFlags & 0x20) >> 5));

	m_verticalOffset = (int16_t)readU16(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Vertical Offset (%i)\n", m_verticalOffset));

	m_widthFlags = readU8(input, encryption) & 0x01;
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Width Flags: 0x%.2x\n", m_widthFlags));

	m_width = readU16(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Width: %i\n", m_width));

	m_heightFlags = readU8(input, encryption) & 0x01;
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Height Flags: 0x%.2x\n", m_heightFlags));

	m_height = readU16(input, encryption);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Height: %i\n", m_height));

	input->seek(tmpSizeOfBoxPositioningData + tmpBoxPositioningDataPosition, WPX_SEEK_SET);

	// Reading box content data

	uint16_t tmpSizeOfBoxContentData = readU16(input, encryption);
	long tmpBoxContentDataPosition = input->tell();
	input->seek(1, WPX_SEEK_CUR);

	m_contentType = readU8(input, encryption);
	uint8_t tmpContentAlignFlags = readU8(input, encryption);
	m_contentHAlign = tmpContentAlignFlags & 0x03;
	m_contentVAlign = (uint8_t)((tmpContentAlignFlags & 0xC0) >> 2);
	m_contentPreserveAspectRatio = ((tmpContentAlignFlags & 0x10) == 0x00);

	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Content Type (%i)\n", m_contentType));
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Content Alignment (horizontal: 0x%.2x) (vertical: 0x%.2x) (preserve aspect ratio: %s)\n",
	               m_contentHAlign, m_contentVAlign, m_contentPreserveAspectRatio ? "true" : "false"));

	switch (m_contentType)
	{
	case 0x03:
	{
		uint16_t tmpGraphicsRenderingInfoSize = readU16(input, encryption);
		long tmpGraphicsRenderingInfoBegin = input->tell();
		if (0x01 == (readU8(input, encryption) & 0xFF))
		{
			m_nativeWidth = readU16(input, encryption);
			m_nativeHeight = readU16(input, encryption);
		}
		else
			input->seek(4, WPX_SEEK_CUR);

		input->seek(tmpGraphicsRenderingInfoSize + tmpGraphicsRenderingInfoBegin, WPX_SEEK_CUR);
	}
	break;
	default:
		break;
	}

	input->seek(tmpSizeOfBoxContentData + tmpBoxContentDataPosition, WPX_SEEK_SET);

	// Reading box caption data

	uint16_t tmpSizeOfBoxCaptionData = readU16(input, encryption);
	long tmpBoxCaptionDataPosition = input->tell();

	input->seek(tmpSizeOfBoxCaptionData + tmpBoxCaptionDataPosition, WPX_SEEK_SET);

	// Reading box border data

	uint16_t tmpSizeOfBoxBorderData = readU16(input, encryption);
	long tmpBoxBorderDataPosition = input->tell();

	input->seek(tmpSizeOfBoxBorderData + tmpBoxBorderDataPosition, WPX_SEEK_SET);

	// Reading box fill data

	uint16_t tmpSizeOfBoxFillData = readU16(input, encryption);
	long tmpBoxFillDataPosition = input->tell();

	input->seek(tmpSizeOfBoxFillData + tmpBoxFillDataPosition, WPX_SEEK_SET);

	// Reading box wrapping data

	uint16_t tmpSizeOfBoxWrappingData = readU16(input, encryption);
	long tmpBoxWrappingDataPosition = input->tell();

	input->seek(tmpSizeOfBoxWrappingData + tmpBoxWrappingDataPosition, WPX_SEEK_SET);

	// Reading box hypertext data

	uint16_t tmpSizeOfBoxHypertextData = readU16(input, encryption);
	long tmpBoxHypertextDataPosition = input->tell();

	input->seek(tmpSizeOfBoxHypertextData + tmpBoxHypertextDataPosition, WPX_SEEK_SET);

	// Dumping hexadecimally the rest of the packet

	long tmpCurrentPosition = input->tell();
	if ((long)tmpStartOfBoxData + (long)tmpSizeOfBoxData - tmpCurrentPosition < 0)
		throw FileException();

#ifdef DEBUG
	for (unsigned i = 0; i < tmpStartOfBoxData + tmpSizeOfBoxData - tmpCurrentPosition; i++)
	{
		if (i % 8 == 0)
			WPD_DEBUG_MSG(("\n        "));
		WPD_DEBUG_MSG(("%.2x ", readU8(input, encryption)));
	}
#else
	if (input->seek(tmpStartOfBoxData + tmpSizeOfBoxData, WPX_SEEK_SET))
		throw FileException();
#endif

}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
