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

#include "WP5GraphicsInformationPacket.h"
#include "WP5Parser.h"
#include "libwpd_internal.h"

WP5GraphicsInformationPacket::WP5GraphicsInformationPacket(WPXInputStream *input, int /* id */, uint32_t dataOffset, uint32_t dataSize) :
	WP5GeneralPacketData(input),
	m_images()
{	
	_read(input, dataOffset, dataSize);
}

WP5GraphicsInformationPacket::~WP5GraphicsInformationPacket()
{
}

void WP5GraphicsInformationPacket::_readContents(WPXInputStream *input, uint32_t dataSize)
{
	uint16_t tmpImagesCount = readU16(input);
	printf("I have %i images\n", tmpImagesCount);
	std::vector<uint32_t> tmpImagesSizes;
	for (uint16_t i = 0; i < tmpImagesCount; i++)
		tmpImagesSizes.push_back(readU32(input));

	for (uint16_t j = 0; j < tmpImagesCount; j++)
	{
		printf("offset of image %i is 0x%x\n", j, input->tell());
		uint8_t *tmpData = new uint8_t[tmpImagesSizes[j]+16];
		
		// Here we create a file header, since it looks like the embedded files do not contain it.
		tmpData[0] = 0xff; tmpData[1] = 0x57; tmpData[2] = 0x50; tmpData[3] = 0x43;
		tmpData[4] = 0x10; tmpData[5] = 0x00; tmpData[6] = 0x00; tmpData[7] = 0x00;
		tmpData[8] = 0x01; tmpData[9] = 0x16; tmpData[10] = 0x01; tmpData[11] = 0x00;
		tmpData[12] = 0x00; tmpData[13] = 0x00; tmpData[14] = 0x00; tmpData[15] = 0x00;
		
		for (uint32_t k = 0; k < tmpImagesSizes[j]; k++)
			tmpData[k+16] = readU8(input);
#if 0
		WPXString filename;
		filename.sprintf("binarydump%.4x.wpg", j);
		FILE *f = fopen(filename.cstr(), "w");
		for (uint32_t l = 0; l < tmpImagesSizes[j] + 16; l++)
			fprintf(f, "%c", tmpData[l]);
		fclose(f);
#endif
		m_images.push_back( new WPXMemoryInputStream(tmpData, tmpImagesSizes[j]) );
	}
}
