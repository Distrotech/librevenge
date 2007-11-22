/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell Inc. (http://www.novell.com)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#ifndef WP6BOXGROUP_H
#define WP6BOXGROUP_H

#include "WP6VariableLengthGroup.h"
#include <vector>
#include "libwpd_internal.h"

class WPXInputStream;
class WP6Listener;

class WP6BoxGroup : public WP6VariableLengthGroup
{
public:
	WP6BoxGroup(WPXInputStream *input, WPXEncryption *encryption);	
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	uint8_t m_generalPositioningFlagsMask, m_generalPositioningFlagsData;	
	bool m_hasHorizontalPositioning;
	uint8_t m_horizontalPositioningFlags;
	int16_t m_horizontalOffset;
	uint8_t m_leftColumn, m_rightColumn;
	bool m_hasVerticalPositioning;
	uint8_t m_verticalPositioningFlags;
	int16_t m_verticalOffset;
	bool m_hasWidthInformation;
	uint8_t m_widthFlags;
	uint16_t m_width;
	bool m_hasHeightInformation;
	uint8_t m_heightFlags;
	uint16_t m_height;
	bool m_hasZOrderInformation;
	uint8_t m_zOrderFlags;
	bool m_hasBoxContentType;
	uint8_t m_boxContentType;
	uint16_t m_nativeWidth, m_nativeHeight;
};

#endif /* WP6BOXGROUP_H */
