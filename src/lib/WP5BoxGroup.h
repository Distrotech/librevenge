/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP5BOXGROUP_H
#define WP5BOXGROUP_H

#include "WP5VariableLengthGroup.h"
#include "WPXBinaryData.h"

class WP5BoxGroup : public WP5VariableLengthGroup
{
public:
	WP5BoxGroup(WPXInputStream *input, WPXEncryption *encryption);	
	void parse(WP5Listener *listener);
	
protected:
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);

private:
	uint16_t m_boxNumber;
	uint8_t m_positionAndType, m_alignment;
	uint16_t m_width, m_height, m_x, m_y;
	uint8_t m_boxType;
	uint16_t m_graphicsOffset;
	const WPXBinaryData *m_data;
};

#endif /* WP5BOXGROUP_H */
