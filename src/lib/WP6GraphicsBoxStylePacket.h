/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP6GRAPHICSBOXSTYLEPACKET_H
#define WP6GRAPHICSBOXSTYLEPACKET_H

#include <libwpd/libwpd.h>
#include <libwpd-stream/libwpd-stream.h>
#include "WP6PrefixDataPacket.h"
#include "WP6Listener.h"

class WP6GraphicsBoxStylePacket : public WP6PrefixDataPacket
{
public:
	WP6GraphicsBoxStylePacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP6GraphicsBoxStylePacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener * /*listener*/) const {}

	uint8_t getGeneralPositioningFlags() const
	{
		return m_generalPositioningFlags;
	}
	uint8_t getHorizontalPositioningFlags() const
	{
		return m_horizontalPositioningFlags;
	}
	int16_t getHorizontalOffset() const
	{
		return m_horizontalOffset;
	}
	uint8_t getLeftColumn() const
	{
		return m_leftColumn;
	}
	uint8_t getRightColumn() const
	{
		return m_rightColumn;
	}
	uint8_t getVerticalPositioningFlags() const
	{
		return m_verticalPositioningFlags;
	}
	int16_t getVerticalOffset() const
	{
		return m_verticalOffset;
	}
	uint8_t getWidthFlags() const
	{
		return m_widthFlags;
	}
	uint16_t getWidth() const
	{
		return m_width;
	}
	uint8_t getHeightFlags() const
	{
		return m_heightFlags;
	}
	uint16_t getHeight() const
	{
		return m_height;
	}
	uint8_t getContentType() const
	{
		return m_contentType;
	}
	uint16_t getNativeWidth() const
	{
		return m_nativeWidth;
	}
	uint16_t getNativeHeight() const
	{
		return m_nativeHeight;
	}

private:
	WP6GraphicsBoxStylePacket(const WP6GraphicsBoxStylePacket &);
	WP6GraphicsBoxStylePacket &operator=(const WP6GraphicsBoxStylePacket &);
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
