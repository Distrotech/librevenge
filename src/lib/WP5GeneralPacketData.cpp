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

#include "WP5GeneralPacketData.h"
#include "WP5GeneralPacketIndex.h"
#include "WP5SpecialHeaderIndex.h"
#include "WP5FileStructure.h"
#include "WP5ListFontsUsedPacket.h"
#include "WP5FontNameStringPoolPacket.h"
#include "WP5GraphicsInformationPacket.h"
#include "libwpd_internal.h"

WP5GeneralPacketData::WP5GeneralPacketData()
{
}

WP5GeneralPacketData * WP5GeneralPacketData::constructGeneralPacketData(WPXInputStream * input, WPXEncryption *encryption, WP5GeneralPacketIndex *packetIndex)
{	       
	switch (packetIndex->getType())
	{
	case WP50_LIST_FONTS_USED_PACKET:
	case WP51_LIST_FONTS_USED_PACKET:
		return new WP5ListFontsUsedPacket(input, encryption, packetIndex->getID(), packetIndex->getDataOffset(), 
						packetIndex->getDataSize(), packetIndex->getType());
	case WP5_FONT_NAME_STRING_POOL_PACKET:
		return new WP5FontNameStringPoolPacket(input, encryption, packetIndex->getID(), packetIndex->getDataOffset(), 
							packetIndex->getDataSize());
	case WP5_GRAPHICS_INFORMATION_PACKET:
		return new WP5GraphicsInformationPacket(input, encryption, packetIndex->getID(), packetIndex->getDataOffset(),
			packetIndex->getDataSize());
	default:
		return 0;
	}
}

void WP5GeneralPacketData::_read(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataOffset, uint32_t dataSize)
{
	input->seek(dataOffset, WPX_SEEK_SET);

	_readContents(input, encryption, dataSize);

	// assert that we haven't surpassed the size of the packet?
}
