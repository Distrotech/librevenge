/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell Inc. (http://www.novell.com)
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

#include "WP6GraphicsBoxStylePacket.h"
#include "WP6Parser.h"

WP6GraphicsBoxStylePacket::WP6GraphicsBoxStylePacket(WPXInputStream *input, int  /* id */, uint32_t dataOffset, uint32_t dataSize): 
	WP6PrefixDataPacket(input),
	m_isLibraryStyle(false),
	m_boxStyleName(),
	m_anchorValue(0),
	m_overlapFlag(0),
	m_autoFlag(0),
	m_hAlignType(0),
	m_hAlign(0),
	m_hOffset(0),
	m_vAlignType(0),
	m_vAlign(0),
	m_vOffset(0),
	m_widthFlag(0),
	m_heightFlag(0)
{	
	_read(input, dataOffset, dataSize);
}

WP6GraphicsBoxStylePacket::~WP6GraphicsBoxStylePacket()
{
}

void WP6GraphicsBoxStylePacket::_readContents(WPXInputStream *input)
{
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket position: 0x%.8x\n", (unsigned)input->tell()));
	uint16_t tmpNumChildIDs = readU16(input);
	input->seek(tmpNumChildIDs * 2, WPX_SEEK_CUR);
	uint16_t tmpSizeOfBoxData = readU16(input);
	unsigned tmpStartOfBoxData = input->tell();

	// Reading the box name/library data

	uint16_t tmpSizeOfBoxNameLibraryData = readU16(input);
	unsigned tmpBoxNameLibraryDataPosition = input->tell();
	input->seek(1, WPX_SEEK_CUR);
	m_isLibraryStyle = ((readU8(input) & 0x01) != 0x00);
	int16_t tmpBoxNameLength = (int16_t)readU16(input);

	if (tmpBoxNameLength > 0)
	{
		for (int16_t i = 0; i < (tmpBoxNameLength/2); i++)
		{
			uint16_t charWord = readU16(input);
			uint8_t characterSet = (uint8_t)((charWord >> 8) & 0x00FF);
			uint8_t character = (uint8_t)(charWord & 0xFF);

			if (character == 0x00 && characterSet == 0x00)
				break;

			const uint16_t *chars;
			int len = extendedCharacterWP6ToUCS2(character, characterSet, &chars);

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

	uint16_t tmpSizeOfBoxCounterData = readU16(input);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box counter data\n"));
	input->seek(tmpSizeOfBoxCounterData, WPX_SEEK_CUR);
	
	// Reading Box positioning data
	
	uint16_t tmpSizeOfBoxPositioningData = readU16(input);
	unsigned tmpBoxPositioningDataPosition = input->tell();
	input->seek(1, WPX_SEEK_CUR);

	uint8_t tmpGeneralPositioningFlags = readU8(input);
	m_anchorValue = tmpGeneralPositioningFlags & 0x07;
	m_overlapFlag = (tmpGeneralPositioningFlags & 0x10) >> 4;
	m_autoFlag = (tmpGeneralPositioningFlags & 0x20) >> 5;
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Positioning data (anchor value: %i) (page offset bit: %i) (overlap flag: %i) (auto flag: %i)\n",
		m_anchorValue, (tmpGeneralPositioningFlags & 0x08) >> 3, m_overlapFlag, m_autoFlag));

	uint8_t tmpHorizontalPositionFlags = readU8(input);
	m_hAlignType = tmpHorizontalPositionFlags & 0x03;
	m_hAlign = (tmpHorizontalPositionFlags & 0x1C) >> 2;
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Horizontal position (horizontal alignment type: %i) (horizontal alignment: %i)\n",
		m_hAlignType, m_hAlign));
		
	m_hOffset = readU16(input);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Horizontal Offset (%i)\n", m_hOffset));
	
#ifdef DEBUG
	uint8_t tmpLeftColumn = readU8(input);
	uint8_t tmpRightColumn = readU8(input);
#else
	readU8(input);
	readU8(input);
#endif
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Horizontal position (between columns %i and %i)\n", tmpLeftColumn, tmpRightColumn));
	
	uint8_t tmpVerticalPositionFlags = readU8(input);
	m_vAlignType = tmpVerticalPositionFlags & 0x03;
	m_vAlign = (tmpVerticalPositionFlags & 0x1C) >> 2;
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Vertical position (vertical alignment type: %i) (vertical alignment: %i) (vertical effect: %i)\n",
		m_vAlignType, m_vAlign, (tmpVerticalPositionFlags & 0x20) >> 5));

	m_vOffset = readU16(input);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Vertical Offset (%i)\n", m_vOffset));
	
	m_widthFlag = readU8(input) & 0x01;
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Width Flags: 0x%.2x\n", m_widthFlag));
	
	m_width = readU16(input);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Width: %i\n", m_width));
	
	m_heightFlag = readU8(input) & 0x01;
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Height Flags: 0x%.2x\n", m_heightFlag));
	
	m_height = readU16(input);
	WPD_DEBUG_MSG(("WP6GraphicsBoxStylePacket -- Box Height: %i\n", m_height));

	input->seek(tmpSizeOfBoxPositioningData + tmpBoxPositioningDataPosition, WPX_SEEK_SET);
	
	// Reading box content data
	
	uint16_t tmpSizeOfBoxContentData = readU16(input);
	unsigned tmpBoxContentDataPosition = input->tell();
	input->seek(1, WPX_SEEK_CUR);
	
	input->seek(tmpSizeOfBoxContentData + tmpBoxContentDataPosition, WPX_SEEK_SET);
	
	// Reading box caption data
	
	uint16_t tmpSizeOfBoxCaptionData = readU16(input);
	unsigned tmpBoxCaptionDataPosition = input->tell();
	
	input->seek(tmpSizeOfBoxCaptionData + tmpBoxCaptionDataPosition, WPX_SEEK_SET);
	
	// Reading box border data
	
	uint16_t tmpSizeOfBoxBorderData = readU16(input);
	unsigned tmpBoxBorderDataPosition = input->tell();
	
	input->seek(tmpSizeOfBoxBorderData + tmpBoxBorderDataPosition, WPX_SEEK_SET);
	
	// Reading box fill data
	
	uint16_t tmpSizeOfBoxFillData = readU16(input);
	unsigned tmpBoxFillDataPosition = input->tell();
	
	input->seek(tmpSizeOfBoxFillData + tmpBoxFillDataPosition, WPX_SEEK_SET);
	
	// Reading box wrapping data
	
	uint16_t tmpSizeOfBoxWrappingData = readU16(input);
	unsigned tmpBoxWrappingDataPosition = input->tell();
	
	input->seek(tmpSizeOfBoxWrappingData + tmpBoxWrappingDataPosition, WPX_SEEK_SET);
	
	// Reading box hypertext data
	
	uint16_t tmpSizeOfBoxHypertextData = readU16(input);
	unsigned tmpBoxHypertextDataPosition = input->tell();
	
	input->seek(tmpSizeOfBoxHypertextData + tmpBoxHypertextDataPosition, WPX_SEEK_SET);
	
	// Dumping hexadecimally the rest of the packet
	
#ifdef DEBUG
	unsigned tmpCurrentPosition = input->tell();
	for (unsigned i = 0; i < tmpStartOfBoxData + tmpSizeOfBoxData - tmpCurrentPosition; i++)
	{
		if (i % 8 == 0)
			WPD_DEBUG_MSG(("\n        "));
		WPD_DEBUG_MSG(("%.2x ", readU8(input)));
	}
#else
	input->seek(tmpStartOfBoxData + tmpSizeOfBoxData, WPX_SEEK_SET);
#endif

}
