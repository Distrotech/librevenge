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

WP5GeneralPacketData *WP5GeneralPacketData::constructGeneralPacketData(WPXInputStream *input, WPXEncryption *encryption, WP5GeneralPacketIndex *packetIndex)
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
