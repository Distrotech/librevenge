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

#include "WP6GraphicsCachedFileDataPacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"
#include "WPXMemoryStream.h"
#include "WPXString.h"

WP6GraphicsCachedFileDataPacket::WP6GraphicsCachedFileDataPacket(WPXInputStream *input, int  id, uint32_t dataOffset, uint32_t dataSize): 
	WP6PrefixDataPacket(input),
	m_id(id),
	m_object(0)
{	
	_read(input, dataOffset, dataSize);
}

WP6GraphicsCachedFileDataPacket::~WP6GraphicsCachedFileDataPacket()
{
	if (m_object)
		delete m_object;
}

void WP6GraphicsCachedFileDataPacket::_readContents(WPXInputStream *input)
{
	uint32_t tmpDataSize = getDataSize();
	uint8_t *tmpData = new uint8_t[tmpDataSize];
	for (uint32_t i = 0; i < tmpDataSize; i++)
		tmpData[i] = readU8(input);
#if 0
	WPXString filename;
	filename.sprintf("binarydump%.4x.wpg", m_id);
	FILE *f = fopen(filename.cstr(), "w");
	for (uint32_t j = 0; j < tmpDataSize; j++)
		fprintf(f, "%c", tmpData[j]);
	fclose(f);
#endif
	m_object = new WPXBinaryData(tmpData, tmpDataSize);
}
