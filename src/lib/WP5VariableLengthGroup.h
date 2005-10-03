/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP5VARIABLELENGTHGROUP_H
#define WP5VARIABLELENGTHGROUP_H

#include "WP5Part.h"

class WP5VariableLengthGroup_SubGroup
{
public:
	virtual ~WP5VariableLengthGroup_SubGroup() {}
	virtual void parse(WP5HLListener *listener) = 0;
};

class WP5VariableLengthGroup : public WP5Part
{
 public:
	WP5VariableLengthGroup(); // WP5VariableLengthGroup should _never_ be constructed, only its inherited classes
	virtual ~WP5VariableLengthGroup() {}
	
	static WP5VariableLengthGroup * constructVariableLengthGroup(WPXInputStream *input, uint8_t group);

 protected:
	void _read(WPXInputStream *input);
 	virtual void _readContents(WPXInputStream *input) {} // we don't always need more information than that provided generically

	const uint8_t getSubGroup() const { return m_subGroup; }

 private:
	uint8_t m_subGroup;
	uint16_t m_size; 
};

#endif /* WP5VARIABLELENGTHGROUP_H */
