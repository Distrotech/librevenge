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

#ifndef WP6GRAPHICSCACHEDFILEDATAPACKET_H
#define WP6GRAPHICSCACHEDFILEDATAPACKET_H
#include "WP6PrefixDataPacket.h"
#include "WP6FileStructure.h"
#include "WPXBinaryData.h"
#include "WP6Listener.h"

class WP6GraphicsCachedFileDataPacket : public WP6PrefixDataPacket
{
public:
	WP6GraphicsCachedFileDataPacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP6GraphicsCachedFileDataPacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener * /* listener */) const {}
	const WPXBinaryData *getBinaryObject() const { return m_object; }

private:
	WP6GraphicsCachedFileDataPacket(const WP6GraphicsCachedFileDataPacket&);             
	WP6GraphicsCachedFileDataPacket& operator=(const WP6GraphicsCachedFileDataPacket&);
	int m_id;
	WPXBinaryData *m_object;
	uint8_t *m_data;           
	
};
#endif /* WP6GRAPHICSCACHEDFILEDATAPACKET_H */
