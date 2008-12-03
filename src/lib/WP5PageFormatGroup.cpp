/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004-2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell, Inc. (http://www.novell.com)
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

#include "WP5PageFormatGroup.h"
#include "WP5FileStructure.h"
#include "WP5Listener.h"
#include "WPXFileStructure.h"

WP5PageFormatGroup::WP5PageFormatGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup(),
	m_leftMargin(0),
	m_rightMargin(0),
	m_lineSpacing(1.0),
	m_tabStops(),
	m_marginOffset(0xffff),
	m_topMargin(0),
	m_bottomMargin(0),
	m_justification(0),
	m_suppressCode(0),
	m_formLength(0),
	m_formWidth(0),
	m_formOrientation(PORTRAIT)
{
	_read(input, encryption);
}

WP5PageFormatGroup::~WP5PageFormatGroup()
{
}

void WP5PageFormatGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP5_TOP_PAGE_FORMAT_GROUP_LEFT_RIGHT_MARGIN_SET:
		// skip 4 bytes (old values of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		m_leftMargin = readU16(input, encryption);
		m_rightMargin = readU16(input, encryption);
		WPD_DEBUG_MSG(("WordPerfect: Page format group left/right margin set (left margin: %i, right margin: %i)\n", m_leftMargin, m_rightMargin));
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_SPACING_SET:
		// skip 2 bytes (old spacing of no interest for us)
		input->seek(2, WPX_SEEK_CUR);
		{
			uint16_t lineSpacing = readU16(input, encryption);
			int8_t lineSpacingIntegerPart = (int8_t)((lineSpacing & 0xFF00) >> 8);
			double lineSpacingFractionalPart = (double)(lineSpacing & 0x00FF)/(double)0xFF;
			WPD_DEBUG_MSG(("WordPerfect: Page format group line spacing - integer part: %i fractional part: %f (original value: %i)\n",
				       lineSpacingIntegerPart, lineSpacingFractionalPart, lineSpacing));
			m_lineSpacing = lineSpacingIntegerPart + lineSpacingFractionalPart;
		}
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_TAB_SET:
		input->seek(100, WPX_SEEK_CUR);
		m_tabStops.reserve(40);
		{

			uint16_t tmpTabPosition = 0;
			for (unsigned i=0; i < 40 && (tmpTabPosition = readU16(input, encryption)) != 0xFFFF; i++)
			{
				m_tabStops.push_back(WPXTabStop());
				m_tabStops[i].m_position = (double)((double)tmpTabPosition/(double)WPX_NUM_WPUS_PER_INCH);
			}
			if ((tmpTabPosition & 0xFFFF) == 0xFFFF)
				input->seek((39 - m_tabStops.size()) * 2, WPX_SEEK_CUR);
			else
				input->seek((40 - m_tabStops.size()) * 2, WPX_SEEK_CUR);

			for (unsigned j=0; (j < (m_tabStops.size() / 2) + (m_tabStops.size() % 2)) && (j < 20); j++)
			{
				uint8_t tmpTabType = readU8(input, encryption);
				if (j*2 < m_tabStops.size())
				{
					switch ((tmpTabType & 0x30) >> 4)
					{
					case 0x00:
						m_tabStops[j*2].m_alignment = LEFT;
						break;
					case 0x01:
						m_tabStops[j*2].m_alignment = CENTER;
						break;
					case 0x02:
						m_tabStops[j*2].m_alignment = RIGHT;
						break;
					case 0x03:
						m_tabStops[j*2].m_alignment = DECIMAL;
						break;
					default: // should not happen, maybe corruption
						m_tabStops[j*2].m_alignment = LEFT;
						break;
					}
					if (tmpTabType & 0x40)
					{
						m_tabStops[j*2].m_leaderCharacter = '.';
						m_tabStops[j*2].m_leaderNumSpaces = 0;
					}
				}
				if (j*2 + 1 < m_tabStops.size())
				{
					switch (tmpTabType & 0x03)
					{
					case 0x00:
						m_tabStops[j*2+1].m_alignment = LEFT;
						break;
					case 0x01:
						m_tabStops[j*2+1].m_alignment = CENTER;
						break;
					case 0x02:
						m_tabStops[j*2+1].m_alignment = RIGHT;
						break;
					case 0x03:
						m_tabStops[j*2+1].m_alignment = DECIMAL;
						break;
					default: // should not happen, maybe corruption
						m_tabStops[j*2+1].m_alignment = LEFT;
						break;
					}
					if (tmpTabType & 0x40)
					{
						m_tabStops[j*2+1].m_leaderCharacter = '.';
						m_tabStops[j*2+1].m_leaderNumSpaces = 0;
					}
				}
			}
			input->seek(20 - (m_tabStops.size() / 2 ) - (m_tabStops.size() % 2), WPX_SEEK_CUR);
			
			if ((getSize() > 4) && (getSize() - 4 == 0x00D0))
			{
				input->seek(2, WPX_SEEK_CUR);
				m_marginOffset = readU16(input, encryption);
				if (0xFFFF != (m_marginOffset & 0xFFFF))
				{
					for (std::vector<WPXTabStop>::iterator iter = m_tabStops.begin(); iter != m_tabStops.end(); iter++)
						iter->m_position -= (double)((double)m_marginOffset/(double)WPX_NUM_WPUS_PER_INCH);
				}
			}
			else
				m_marginOffset = 0xFFFF;
		}
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_TOP_BOTTOM_MARGIN_SET:
		// skip 4 bytes (old values of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		m_topMargin = readU16(input, encryption);
		m_bottomMargin = readU16(input, encryption);
		WPD_DEBUG_MSG(("WordPerfect: Page format group top/bottom margin set (top margin: %i, bottom margin: %i)\n", m_topMargin, m_bottomMargin));
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_JUSTIFICATION:
		// skip 1 byte (old justification of no interest for us)
		input->seek(1, WPX_SEEK_CUR);
		m_justification = readU8(input, encryption);
		// WP6 and WP3 have one more category of justification
		// Following hack allows us to use the same function for the three parsers
		if (m_justification == 0x04)
			m_justification = 0x05;
		WPD_DEBUG_MSG(("WordPerfect: Page format group justification (0x%2x)\n", m_justification));
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_SUPPRESS_PAGE_CHARACTERISTICS:
		// skip 1 byte (old suppress code)
		input->seek(1, WPX_SEEK_CUR);
		m_suppressCode = readU8(input, encryption);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_FORM:
		uint8_t tmpOrientation;
		// skip to the new DESIRED values (99 - 4)
		input->seek(95, WPX_SEEK_CUR);
		m_formLength = readU16(input, encryption); // New DESIRED length
		m_formWidth = readU16(input, encryption); // New DESIRED width
		// skipp to the orientation value (193 - 103)
		input->seek(90, WPX_SEEK_CUR);
		tmpOrientation = readU8(input, encryption); // New EFFECTIVE orientation
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
	case WP5_TOP_PAGE_FORMAT_GROUP_TAB_SET:
	        WPD_DEBUG_MSG(("WordPerfect: parsing a tab set\n"));
		listener->setTabs(m_tabStops, m_marginOffset);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_TOP_BOTTOM_MARGIN_SET:
		listener->pageMarginChange(WPX_TOP, m_topMargin);
		listener->pageMarginChange(WPX_BOTTOM, m_bottomMargin);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_JUSTIFICATION:
		listener->justificationChange(m_justification);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_SUPPRESS_PAGE_CHARACTERISTICS:
		listener->suppressPageCharacteristics(m_suppressCode);
		break;
	case WP5_TOP_PAGE_FORMAT_GROUP_FORM:
		listener->pageFormChange(m_formLength, m_formWidth, m_formOrientation);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
