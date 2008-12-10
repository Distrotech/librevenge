/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP5GRAPHICSINFORMATIONPACKET_H
#define WP5GRAPHICSINFORMATIONPACKET_H
#include "WP5GeneralPacketData.h"
#include "WPXMemoryStream.h"
#include "WPXBinaryData.h"
#include <vector>

class WP5GraphicsInformationPacket : public WP5GeneralPacketData
{
public:
	WP5GraphicsInformationPacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP5GraphicsInformationPacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataSize);
	const std::vector<WPXBinaryData *> &getImages() const { return m_images; }
	const WPXBinaryData *getImage( unsigned long imageIndex ) const { if (imageIndex < m_images.size()) return m_images[imageIndex]; return NULL; }

private:
	std::vector<WPXBinaryData *> m_images;
	std::vector<uint8_t *> m_data;
};
#endif /* WP5GRAPHICSINFORMATIONPACKET_H */
