/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3WindowGroup.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP3Listener.h"

WP3WindowGroup::WP3WindowGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_width(0.0f),
	m_height(0.0f),
	m_resourceID(0)
{
	_read(input, encryption);
}

WP3WindowGroup::~WP3WindowGroup()
{
}

void WP3WindowGroup::_readContents(WPXInputStream * input, WPXEncryption * encryption)
{
	switch (getSubGroup())
	{
	case WP3_WINDOW_GROUP_FIGURE_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_TABLE_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_TEXT_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_USER_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_EQUATION_BOX_FUNCTION:
	case WP3_WINDOW_GROUP_HTML_IMAGE_BOX_FUNCTION:
		{
			input->seek(14, WPX_SEEK_CUR);
			readU16(input, encryption, true); // picture flags
			input->seek(2, WPX_SEEK_CUR);
			readU8(input, encryption); // left align column
			readU8(input, encryption); // right align column
			input->seek(8, WPX_SEEK_CUR);
			m_width = fixedPointToFloat(readU32(input, encryption, true));
			m_height = fixedPointToFloat(readU32(input, encryption, true));
			m_resourceID = readU16(input, encryption, true);
		}
		break;

	case WP3_WINDOW_GROUP_HORIZONTAL_LINE:
		break;
		
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3WindowGroup::parse(WP3Listener * /*listener*/)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Window group\n"));
}
