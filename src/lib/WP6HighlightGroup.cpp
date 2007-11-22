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

#include "WP6HighlightGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"

WP6HighlightGroup::WP6HighlightGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP6FixedLengthGroup(groupID),
	m_color()
{
	_read(input, encryption);
}

void WP6HighlightGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_color.m_r = readU8(input, encryption);
	m_color.m_g = readU8(input, encryption);
	m_color.m_b = readU8(input, encryption);
	m_color.m_s = readU8(input, encryption);
}

WP6HighlightOnGroup::WP6HighlightOnGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP6HighlightGroup(input, encryption, groupID)
{
}

void WP6HighlightOnGroup::parse(WP6Listener *listener)
{
	listener->highlightChange(true, getColor());
}

WP6HighlightOffGroup::WP6HighlightOffGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP6HighlightGroup(input, encryption, groupID)
{
}

void WP6HighlightOffGroup::parse(WP6Listener *listener)
{
	listener->highlightChange(false, getColor());
}
