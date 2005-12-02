/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP5PageFormatGroup.h"
#include "WP5FileStructure.h"
#include "libwpd_internal.h"

WP5PageFormatGroup::WP5PageFormatGroup(WPXInputStream *input) :
	WP5VariableLengthGroup(),
	m_leftMargin(0),
	m_rightMargin(0),
	m_lineSpacing(1.0f),
	m_topMargin(0),
	m_bottomMargin(0),
	m_justification(0),
	m_formLength(0),
	m_formWidth(0)
{
	_read(input);
}

WP5PageFormatGroup::~WP5PageFormatGroup()
{
}

void WP5PageFormatGroup::_readContents(WPXInputStream *input)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP5_TOP_PAGE_FORMAT_GROUP_LEFT_RIGHT_MARGIN_SET:
		// skip 4 bytes (old values of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		m_leftMargin = readU16(input);
		m_rightMargin = readU16(input);
		WPD_DEBUG_MSG(("WordPerfect: Page format group left/right margin set (left margin: %i, right margin: %i)\n", m_leftMargin, m_rightMargin));
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_SPACING_SET:
		// skip 2 bytes (old spacing of no interest for us)
		input->seek(2, WPX_SEEK_CUR);
		{
			uint16_t lineSpacing = readU32(input, true);
			int8_t lineSpacingIntegerPart = (int8_t)((lineSpacing & 0xFF00) >> 8);
			float lineSpacingFractionalPart = (float)(lineSpacing & 0xFF)/(float)0xFF;
			WPD_DEBUG_MSG(("WordPerfect: Page format group line spacing - integer part: %i fractional part: %f (original value: %i)\n",
				       lineSpacingIntegerPart, lineSpacingFractionalPart, lineSpacing));
			m_lineSpacing = lineSpacingIntegerPart + lineSpacingFractionalPart;
		}
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_TOP_BOTTOM_MARGIN_SET:
		// skip 4 bytes (old values of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		m_topMargin = readU16(input);
		m_bottomMargin = readU16(input);
		WPD_DEBUG_MSG(("WordPerfect: Page format group top/bottom margin set (top margin: %i, bottom margin: %i)\n", m_topMargin, m_bottomMargin));
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_JUSTIFICATION:
		// skip 1 byte (old justification of no interest for us)
		input->seek(1, WPX_SEEK_CUR);
		m_justification = readU8(input);
		// WP6 and WP3 have one more category of justification
		// Following hack allows us to use the same function for the three parsers
		if (m_justification == 0x04)
			m_justification = 0x05;
		WPD_DEBUG_MSG(("WordPerfect: Page format group justification (0x%2x)\n", m_justification));
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_FORM:
		uint8_t tmpOrientation;
		// skip to the new DESIRED values (99 - 4)
		input->seek(95, WPX_SEEK_CUR);
		m_formLength = readU16(input); // New DESIRED length
		m_formWidth = readU16(input); // New DESIRED width
		// skipp to the orientation value (193 - 103)
		input->seek(90, WPX_SEEK_CUR);
		tmpOrientation = readU8(input); // New EFFECTIVE orientation
		switch (tmpOrientation)
		{
		case 0x01:
			m_formOrientation = LANDSCAPE;
			break;
		case 0x00:
			m_formOrientation = PORTRAIT;
			break;
		default:
			m_formOrientation = PORTRAIT;
			break;
		}
		WPD_DEBUG_MSG(("WordPerfect: Read form information (length: %i), (width: %i), (form orientation: %s),\n",
				m_formLength, m_formWidth, ((m_formOrientation==PORTRAIT)?"portrait":"landscape")));
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP5PageFormatGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Page group\n"));

	switch (getSubGroup())
	{
	case WP5_TOP_PAGE_FORMAT_GROUP_LEFT_RIGHT_MARGIN_SET:
		listener->marginChange(WPX_LEFT, m_leftMargin);
		listener->marginChange(WPX_RIGHT, m_rightMargin);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_SPACING_SET:
	        WPD_DEBUG_MSG(("WordPerfect: parsing a line spacing change of: %f\n", m_lineSpacing));
	        listener->lineSpacingChange(m_lineSpacing);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_TOP_BOTTOM_MARGIN_SET:
		listener->pageMarginChange(WPX_TOP, m_topMargin);
		listener->pageMarginChange(WPX_BOTTOM, m_bottomMargin);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_JUSTIFICATION:
		listener->justificationChange(m_justification);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_FORM:
		listener->pageFormChange(m_formLength, m_formWidth, m_formOrientation, true);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
