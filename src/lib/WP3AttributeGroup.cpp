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

#include "WP3AttributeGroup.h"
#include "libwpd_internal.h"
#include "WP3Listener.h"

WP3AttributeGroup::WP3AttributeGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID)
	: WP3FixedLengthGroup(groupID),
	m_attribute(0),
	m_state(0)
{
	_read(input, encryption);
}

void WP3AttributeGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_attribute = readU8(input, encryption);
	m_state = readU8(input, encryption);
}

void WP3AttributeGroup::parse(WP3Listener *listener)
{
	// FIXME: handle flags
	bool tmpState;
	tmpState = ((m_state & 0x01) == 0x01);
	listener->attributeChange(tmpState, m_attribute);
}

