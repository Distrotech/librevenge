/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell, Inc. (http://www.novell.com)
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

#ifndef WP5PAGEFORMATGROUP_H
#define WP5PAGEFORMATGROUP_H

#include "WP5VariableLengthGroup.h"
#include <vector>

class WP5PageFormatGroup : public WP5VariableLengthGroup
{
 public:
	WP5PageFormatGroup(WPXInputStream *input, WPXEncryption *encryption);	
	~WP5PageFormatGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP5Listener *listener);

 private:
	// variables needed for subgroup 1 (Left/Right Margin Set)
	uint16_t m_leftMargin;
	uint16_t m_rightMargin;
	// variable needed for subgroup 2 (Spacing Set)
	double m_lineSpacing;
	// variables needed for subbroup 4 (Tab Set)
	std::vector<WPXTabStop> m_tabStops;
	uint16_t m_marginOffset;	
	// variables needed for subgroup 5 (Top/Bottom Margin Set)
	uint16_t m_topMargin;
	uint16_t m_bottomMargin;
	// variables needed for subgroup 6 (Justification)
	uint8_t m_justification;
	// variable needed for subgroup 7 (Suppress Page Characteristics)
	uint8_t m_suppressCode;
	// variables needed for subgroup 11 (Form)
	uint16_t m_formLength;
	uint16_t m_formWidth;
	WPXFormOrientation m_formOrientation;

};

#endif /* WP5PAGEFORMATGROUP_H */
