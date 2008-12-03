/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3PAGEFORMATGROUP_H
#define WP3PAGEFORMATGROUP_H

#include "WP3VariableLengthGroup.h"
#include "libwpd_internal.h"
#include <vector>

class WP3PageFormatGroup : public WP3VariableLengthGroup
{
 public:
	WP3PageFormatGroup(WPXInputStream *input, WPXEncryption *encryption);	
	~WP3PageFormatGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP3Listener *listener);

 private:
	// variables needed for subgroup 0x01 (Horizontal Margins)
	uint32_t m_leftMargin;
	uint32_t m_rightMargin;
	// variable needed for subgroup 0x02 (Line Spacing)
	double m_lineSpacing;
	// variables needed for subgroup 0x04 (Set Tabs)
	bool m_isRelative;
	std::vector<WPXTabStop> m_tabStops;
	// variables needed for subgroup 0x05 (Vertical Margins)
	uint32_t m_topMargin;
	uint32_t m_bottomMargin;
	// variable needed for subgroup 0x06 (Justification Mode)
	uint8_t m_justification;
	// variable needed for subgroup 0x07 (Suppress Page)
	uint16_t m_suppressCode;
	// variable needed for subgroup 0x0C (Indent At Beginning of Line)
	uint32_t m_indent;

};

#endif /* WP3PAGEFORMATGROUP_H */
