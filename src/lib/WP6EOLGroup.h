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

#ifndef WP6EOLGROUP_H
#define WP6EOLGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6EOLGroup : public WP6VariableLengthGroup
{
public:
	WP6EOLGroup(WPXInputStream *input, WPXEncryption *encryption);	
	~WP6EOLGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

 private:
 	WP6EOLGroup(const WP6EOLGroup&);
 	WP6EOLGroup& operator=(const WP6EOLGroup&);
	uint8_t m_colSpan, m_rowSpan;
	bool m_boundFromAbove;

 	bool m_useCellAttributes, m_useCellJustification;
	bool m_ignoreInCalculations, m_cellIsLocked;
	uint32_t m_cellAttributes;
	uint8_t m_cellJustification;
	WPXVerticalAlignment m_cellVerticalAlign;
 
	// cell forground and background color
 	RGBSColor * m_cellFgColor;
 	RGBSColor * m_cellBgColor;
	RGBSColor * m_cellBorderColor;
 
	uint8_t m_cellBorders;
	bool m_isHeaderRow;
	bool m_isMinimumHeight;
	uint16_t m_rowHeight;

	bool m_isDontEndAParagraphStyleForThisHardReturn;
};

#endif /* WP6EOLGROUP_H */
