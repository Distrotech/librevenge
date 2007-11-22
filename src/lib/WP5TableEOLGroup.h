/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#ifndef WP5TABLEEOLGROUP_H
#define WP5TABLEEOLGROUP_H

#include "WP5VariableLengthGroup.h"

class WP5TableEOLGroup : public WP5VariableLengthGroup
{
 public:
	WP5TableEOLGroup(WPXInputStream *input, WPXEncryption *encryption);	
	~WP5TableEOLGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP5Listener *listener);

private:
	// Beginning column sub-function
	uint8_t m_cellVerticalAlignment;
	bool m_useCellAttributes;
	bool m_useCellJustification;
	uint8_t m_columnNumber;
	uint8_t m_colSpan;
	uint8_t m_rowSpan;
	bool m_spannedFromAbove;
	uint16_t m_cellAttributes;
	uint8_t m_cellJustification;	
};

#endif /* WP5TABLEEOLGROUP_H */
