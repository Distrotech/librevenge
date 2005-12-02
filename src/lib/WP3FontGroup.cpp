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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include <math.h>
#include "WP3FontGroup.h"
#include "WP3FileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"

WP3FontGroup::WP3FontGroup(WPXInputStream *input) :
	WP3VariableLengthGroup(),
	m_fontColor(),
	m_fontSize(0)
{
	_read(input);
}

WP3FontGroup::~WP3FontGroup()
{
}

void WP3FontGroup::_readContents(WPXInputStream *input)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP3_FONT_GROUP_SET_TEXT_COLOR:
		input->seek(6, WPX_SEEK_CUR);
		{
			uint16_t tmpRed = readU16(input, true);
			uint16_t tmpGreen = readU16(input, true);
			uint16_t tmpBlue = readU16(input, true);
			m_fontColor = RGBSColor(tmpRed, tmpGreen, tmpBlue);
		}
		break;		
	case WP3_FONT_GROUP_SET_TEXT_FONT:
		input->seek(12, WPX_SEEK_CUR);
		m_fontName = readPascalString(input);
		break;
	case WP3_FONT_GROUP_SET_FONT_SIZE:
		input->seek(2, WPX_SEEK_CUR);
		m_fontSize = readU16(input, true);
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3FontGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Font group\n"));

	switch (getSubGroup())
	{
	case WP3_FONT_GROUP_SET_TEXT_COLOR:
		listener->setTextColor(&m_fontColor);
		break;		
	case WP3_FONT_GROUP_SET_TEXT_FONT:
		listener->setTextFont(m_fontName);
		break;
	case WP3_FONT_GROUP_SET_FONT_SIZE:
		listener->setFontSize(m_fontSize);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
