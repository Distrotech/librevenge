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

#include "WP6NumberingMethodGroup.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"


WP6NumberingMethodGroup::WP6NumberingMethodGroup(WPXInputStream *input, 
				     WPXEncryption *encryption) :
	WP6VariableLengthGroup()
{
	_read(input, encryption);
}

WP6NumberingMethodGroup::~WP6NumberingMethodGroup()
{
}

void WP6NumberingMethodGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch (getSubGroup())
	{
	case WP6_NUMBERING_METHOD_GROUP_SET_PAGE_NUMBERING_MODE:
		m_levelNumber = readU8(input, encryption);;
		m_numberingMethod = readU8(input, encryption);
		break;
	}
}

void WP6NumberingMethodGroup::parse(WP6Listener *listener)
{
	switch (getSubGroup())
	{
	case WP6_NUMBERING_METHOD_GROUP_SET_PAGE_NUMBERING_MODE:
		switch (m_numberingMethod) 
		{
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_LOWERCASE:
			listener->setPageNumberingType(LOWERCASE);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_UPPERCASE:
			listener->setPageNumberingType(UPPERCASE);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_LOWERCASE_ROMAN:
			listener->setPageNumberingType(LOWERCASE_ROMAN);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_UPPERCASE_ROMAN:
			listener->setPageNumberingType(UPPERCASE_ROMAN);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_ARABIC:
		default:
			listener->setPageNumberingType(ARABIC);
			break;			
		}
		break;
	}
}
