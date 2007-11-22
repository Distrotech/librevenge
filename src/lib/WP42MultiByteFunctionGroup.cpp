/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

#include "WP42MultiByteFunctionGroup.h"
#include "WP42UnsupportedMultiByteFunctionGroup.h"
#include "WP42HeaderFooterGroup.h"
#include "WP42MarginResetGroup.h"
#include "WP42SuppressPageCharacteristicsGroup.h"
#include "WP42FileStructure.h"
#include "libwpd_internal.h"

WP42MultiByteFunctionGroup::WP42MultiByteFunctionGroup(uint8_t group)
	: m_group(group)
{
}

WP42MultiByteFunctionGroup * WP42MultiByteFunctionGroup::constructMultiByteFunctionGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group)
{
	switch (group)
	{
		case WP42_MARGIN_RESET_GROUP:
			return new WP42MarginResetGroup(input, encryption, group);
		case WP42_SUPPRESS_PAGE_CHARACTERISTICS_GROUP:
			return new WP42SuppressPageCharacteristicsGroup(input, encryption, group);
		case WP42_HEADER_FOOTER_GROUP:
			return new WP42HeaderFooterGroup(input, encryption, group);
		default:
			// this is an unhandled group, just skip it
			return new WP42UnsupportedMultiByteFunctionGroup(input, encryption, group);
	}
}

void WP42MultiByteFunctionGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	_readContents(input, encryption);
	
	// skip over the remaining bytes of the group, if any
	while (!input->atEOS() && (readU8(input, encryption) != m_group));// getGroup()));
	// IMPORTANT: if the class that implements _readContent(input, encryption) already reads the closing gate,
	// IMPORTANT: it is necessary to make an input->seek(-1, WPX_SEEK_CUR) for this function to work well.
}
