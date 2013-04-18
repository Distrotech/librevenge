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

#ifndef WP5LISTFONTSUSEDPACKET_H
#define WP5LISTFONTSUSEDPACKET_H
#include "WP5GeneralPacketData.h"
#include "WPXMemoryStream.h"
#include <vector>

class WP5ListFontsUsedPacket : public WP5GeneralPacketData
{
public:
	WP5ListFontsUsedPacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize, uint16_t packetType);
	~WP5ListFontsUsedPacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataSize);
	unsigned getFontNameOffset(unsigned fontNumber) const;
	double getFontSize(unsigned fontNumber) const;

private:
	uint16_t m_packetType;
	std::vector<unsigned> m_fontNameOffset;
	std::vector<double> m_fontSize;
};
#endif /* WP5LISTFONTSUSEDPACKET_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
