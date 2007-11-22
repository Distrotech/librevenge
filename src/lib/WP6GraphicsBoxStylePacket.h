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

#ifndef WP6GRAPHICSBOXSTYLEPACKET_H
#define WP6GRAPHICSBOXSTYLEPACKET_H
#include "WP6PrefixDataPacket.h"
#include "WP6Listener.h"
#include "WPXStream.h"
#include "WPXString.h"

class WP6GraphicsBoxStylePacket : public WP6PrefixDataPacket
{
public:
	WP6GraphicsBoxStylePacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP6GraphicsBoxStylePacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener * /*listener*/) const {}

	uint8_t getGeneralPositioningFlags() const { return m_generalPositioningFlags; }
	uint8_t getHorizontalPositioningFlags() const { return m_horizontalPositioningFlags; }
	int16_t getHorizontalOffset() const { return m_horizontalOffset; }
	uint8_t getLeftColumn() const { return m_leftColumn; }
	uint8_t getRightColumn() const { return m_rightColumn; }
	uint8_t getVerticalPositioningFlags() const { return m_verticalPositioningFlags; }
	int16_t getVerticalOffset() const { return m_verticalOffset; }
	uint8_t getWidthFlags() const { return m_widthFlags; }
	uint16_t getWidth() const { return m_width; }
	uint8_t getHeightFlags() const { return m_heightFlags; }
	uint16_t getHeight() const { return m_height; }
	uint8_t getContentType() const { return m_contentType; }
	uint16_t getNativeWidth() const { return m_nativeWidth; }
	uint16_t getNativeHeight() const { return m_nativeHeight; }

private:
	WP6GraphicsBoxStylePacket(const WP6GraphicsBoxStylePacket&);             
	WP6GraphicsBoxStylePacket& operator=(const WP6GraphicsBoxStylePacket&);
	bool m_isLibraryStyle;
	WPXString m_boxStyleName;
	uint8_t m_generalPositioningFlags;
	uint8_t m_horizontalPositioningFlags;
	int16_t m_horizontalOffset;
	uint8_t m_leftColumn, m_rightColumn;
	uint8_t m_verticalPositioningFlags;
	int16_t m_verticalOffset;
	uint8_t m_widthFlags;
	uint16_t m_width;
	uint8_t m_heightFlags;
	uint16_t m_height;
	uint8_t m_contentType, m_contentHAlign, m_contentVAlign;
	bool m_contentPreserveAspectRatio;
	uint16_t m_nativeWidth, m_nativeHeight;
};
#endif /* WP6GRAPHICSBOXSTYLEPACKET_H */
