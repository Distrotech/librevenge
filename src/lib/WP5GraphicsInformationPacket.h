/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP5GRAPHICSINFORMATIONPACKET_H
#define WP5GRAPHICSINFORMATIONPACKET_H
#include "WP5GeneralPacketData.h"
#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>
#include <vector>

class WP5GraphicsInformationPacket : public WP5GeneralPacketData
{
public:
	WP5GraphicsInformationPacket(RVNGInputStream *input, RVNGEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP5GraphicsInformationPacket();
	void _readContents(RVNGInputStream *input, RVNGEncryption *encryption, uint32_t dataSize);
	const std::vector<RVNGBinaryData *> &getImages() const
	{
		return m_images;
	}
	const RVNGBinaryData *getImage( unsigned long imageIndex ) const
	{
		if (imageIndex < m_images.size()) return m_images[imageIndex];
		return 0;
	}

private:
	std::vector<RVNGBinaryData *> m_images;
	std::vector<uint8_t *> m_data;
};
#endif /* WP5GRAPHICSINFORMATIONPACKET_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
