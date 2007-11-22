/* libwpd
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP1LeftRightIndentGroup.h"
#include "libwpd_internal.h"

WP1LeftRightIndentGroup::WP1LeftRightIndentGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP1FixedLengthGroup(group),
	m_leftRightMarginOffset(0)
{
	_read(input, encryption);
}

WP1LeftRightIndentGroup::~WP1LeftRightIndentGroup()
{
}

void WP1LeftRightIndentGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_leftRightMarginOffset = readU16(input, encryption, true);
}

void WP1LeftRightIndentGroup::parse(WP1Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a SuppressPageCharacteristics group\n"));
	listener->leftRightIndent(m_leftRightMarginOffset);
}
