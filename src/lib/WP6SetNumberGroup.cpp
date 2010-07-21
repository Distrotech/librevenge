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

#include "WP6SetNumberGroup.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"


WP6SetNumberGroup::WP6SetNumberGroup(WPXInputStream *input, 
				     WPXEncryption *encryption) :
	WP6VariableLengthGroup()
{
	_read(input, encryption);
}

WP6SetNumberGroup::~WP6SetNumberGroup()
{
}

void WP6SetNumberGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch (getSubGroup())
	{
	case WP6_SET_NUMBER_GROUP_SET_PAGE_NUMBER:
		m_countOfLevelNumbersSetting = readU8(input, encryption);
		m_startingLevelNumber = readU8(input, encryption);
		m_countNumbers = readU16(input, encryption);
		WPD_DEBUG_MSG(("WordPerfect: Set number page number (%d, %d, %d)\n", m_countOfLevelNumbersSetting, m_startingLevelNumber, m_countNumbers));
		break;
	}
}

void WP6SetNumberGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a set number group\n"));

	switch (getSubGroup())
	{
	case WP6_SET_NUMBER_GROUP_SET_PAGE_NUMBER:
		listener->setPageNumber(m_countNumbers);
		break;
	}
}
