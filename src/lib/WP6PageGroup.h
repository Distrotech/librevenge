/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef WP6PAGEGROUP_H
#define WP6PAGEGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6PageGroup : public WP6VariableLengthGroup
{
 public:
	WP6PageGroup(WPXInputStream *input);	
	virtual ~WP6PageGroup();
	virtual void _readContents(WPXInputStream *input);
	virtual void parse(WP6Listener *listener);

 private:
	// variables needed for subgroup 0 and 1 (Left/Right Margin Set)
	uint16_t m_margin;
	uint8_t m_marginType;
	// variables needed for subgroup 2 (suppress page characteristics)
	uint8_t m_suppressedCode;
	// variables needed for subgroup 0x11 (Form)
	uint16_t m_formLength;
	uint16_t m_formWidth;
	uint8_t m_formType;
	WPXFormOrientation m_formOrientation;

};

#endif /* WP6PAGEGROUP_H */
