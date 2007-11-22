/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2005-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3VARIABLELENGTHGROUP_H
#define WP3VARIABLELENGTHGROUP_H

#include "WP3Part.h"

class WPXEncryption;

class WP3VariableLengthGroup : public WP3Part
{
 public:
	WP3VariableLengthGroup(); // WP3VariableLengthGroup should _never_ be constructed, only its inherited classes
	virtual ~WP3VariableLengthGroup() {}
	
	static WP3VariableLengthGroup * constructVariableLengthGroup(WPXInputStream *input, WPXEncryption *encryption, const uint8_t group);

	static bool isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t group);

 protected:
	void _read(WPXInputStream *input, WPXEncryption *encryption);
 	virtual void _readContents(WPXInputStream * /* input */, WPXEncryption * /* encryption */) {} // we don't always need more information than that provided generically

	uint8_t getSubGroup() const { return m_subGroup; }
	uint16_t getSize() const { return m_size;}

 private:
	uint8_t m_subGroup;
	uint16_t m_size; 
};

#endif /* WP3VARIABLELENGTHGROUP_H */
