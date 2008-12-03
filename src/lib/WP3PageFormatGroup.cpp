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

#include <math.h>
#include "WP3PageFormatGroup.h"
#include "WP3FileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"
#include "WP3Listener.h"

WP3PageFormatGroup::WP3PageFormatGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_leftMargin(0),
	m_rightMargin(0),
	m_lineSpacing(1.0),
	m_isRelative(false),
	m_tabStops(),
	m_topMargin(0),
	m_bottomMargin(0),
	m_justification(0),
	m_suppressCode(0),
	m_indent(0)
{
	_read(input, encryption);
}

WP3PageFormatGroup::~WP3PageFormatGroup()
{
}

void WP3PageFormatGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	int8_t tmpTabType = 0;
	double tmpTabPosition = 0.0;
	WPXTabStop tmpTabStop = WPXTabStop();

	switch (getSubGroup())
	{
	case WP3_PAGE_FORMAT_GROUP_HORIZONTAL_MARGINS:
		// skip 8 bytes (old values of no interest for us)
		input->seek(8, WPX_SEEK_CUR);
		m_leftMargin = readU32(input, encryption, true);
		m_rightMargin = readU32(input, encryption, true);
		WPD_DEBUG_MSG(("WordPerfect: Page format group horizontal margins\n"));
		break;
		
	case WP3_PAGE_FORMAT_GROUP_LINE_SPACING:
		// skip 4 bytes (old spacing of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		{
			uint32_t lineSpacing = readU32(input, encryption, true);
			int16_t lineSpacingIntegerPart = (int16_t)((lineSpacing & 0xFFFF0000) >> 16);
			double lineSpacingFractionalPart = (double)((double)(lineSpacing & 0xFFFF)/(double)0xFFFF);
			WPD_DEBUG_MSG(("WordPerfect: Page format group line spacing - integer part: %i fractional part: %f (original value: %i)\n",
				       lineSpacingIntegerPart, lineSpacingFractionalPart, lineSpacing));
			m_lineSpacing = lineSpacingIntegerPart + lineSpacingFractionalPart;
		}
		break;

	case WP3_PAGE_FORMAT_GROUP_SET_TABS:
		// skip old condensed tab table if the first value is not 0xFF
		if (0xFF != readU8(input, encryption))
		{
			while (readU8(input, encryption) != 0xFF)
				input->seek(4, WPX_SEEK_CUR);
		}

		m_isRelative = (readU8(input, encryption) & 0x01);

		while ((tmpTabType = (int8_t)readU8(input, encryption))  != (int8_t)0xff)
		{
			if (input->atEOS())
				throw FileException();
			tmpTabPosition = fixedPointToDouble(readU32(input, encryption, true)) / 72.0;

			if (tmpTabType < 0)
			{
				for (int8_t i = tmpTabType; i < 0; i++)
				{
					tmpTabStop.m_position += tmpTabPosition;
					m_tabStops.push_back(tmpTabStop);
				}
			}
			else
			{
				tmpTabStop.m_position = tmpTabPosition;

				switch (tmpTabType & 0x0f)
				{
				case 0:
					tmpTabStop.m_alignment = LEFT;
					break;
				case 1:
					tmpTabStop.m_alignment = CENTER;
					break;
				case 2:
					tmpTabStop.m_alignment = RIGHT;
					break;
				case 3:
					tmpTabStop.m_alignment = DECIMAL;
					break;
				case 4: tmpTabStop.m_alignment = BAR;
					break;
				default:
					tmpTabStop.m_alignment = LEFT;
					break;
				}

				switch ((tmpTabType & 0x70) >> 4 )
				{
				case 0:
					tmpTabStop.m_leaderCharacter = '\0';
					tmpTabStop.m_leaderNumSpaces = 0;
					break;
				case 1:
					tmpTabStop.m_leaderCharacter = '.';
					tmpTabStop.m_leaderNumSpaces = 1;
					break;
				case 2:
					tmpTabStop.m_leaderCharacter = '.';
					tmpTabStop.m_leaderNumSpaces = 0;
					break;
				case 3:
					tmpTabStop.m_leaderCharacter = '-';
					tmpTabStop.m_leaderNumSpaces = 1;
					break;
				case 4:
					tmpTabStop.m_leaderCharacter = '_';
					tmpTabStop.m_leaderNumSpaces = 0;
					break;
				default:
					tmpTabStop.m_leaderCharacter = '.';
					tmpTabStop.m_leaderNumSpaces = 0;
					break;
				}

				m_tabStops.push_back(tmpTabStop);
			}	
		}
		break;
		
	case WP3_PAGE_FORMAT_GROUP_VERTICAL_MARGINS:
		// skip 8 bytes (old values of no interest for us)
		input->seek(8, WPX_SEEK_CUR);
		m_topMargin = readU32(input, encryption, true);
		m_bottomMargin = readU32(input, encryption, true);
		WPD_DEBUG_MSG(("WordPerfect: Page format group vertical margins\n"));
		break;
		
	case WP3_PAGE_FORMAT_GROUP_JUSTIFICATION_MODE:
		// skip 1 byte (old justifcation value of no interest for us)
		input->seek(1, WPX_SEEK_CUR);
		m_justification = readU8(input, encryption);
		break;

	case WP3_PAGE_FORMAT_GROUP_SUPPRESS_PAGE:
		// skip 2 bytes (old suppress code)
		input->seek(2, WPX_SEEK_CUR);
		m_suppressCode = readU16(input, encryption, true);
		break;
		
	case WP3_PAGE_FORMAT_GROUP_INDENT_AT_BEGINNING_OF_PARAGRAPH:
		// skip 4 bytes (old indent indent value of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		m_indent = readU32(input, encryption, true);
		break;
		
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3PageFormatGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Page Format group\n"));

#ifdef DEBUG
    std::vector<WPXTabStop>::const_iterator tabStopsIter;
#endif
    
	switch (getSubGroup())
	{
	case WP3_PAGE_FORMAT_GROUP_HORIZONTAL_MARGINS:
		if (m_leftMargin != 0x80000000)
		{
			listener->marginChange(WPX_LEFT, fixedPointToWPUs(m_leftMargin));
			WPD_DEBUG_MSG(("WordPerfect: Page format group left margin - WPUs: %i (original value: %i)\n",
				       fixedPointToWPUs(m_leftMargin), m_leftMargin));
		}
		if (m_rightMargin != 0x8000000)
		{
			listener->marginChange(WPX_RIGHT, fixedPointToWPUs(m_rightMargin));
			WPD_DEBUG_MSG(("WordPerfect: Page format group right margin - integer part: WPUs: %i (original value: %i)\n",
				       fixedPointToWPUs(m_rightMargin), m_rightMargin));
		}
		break;
		
	case WP3_PAGE_FORMAT_GROUP_LINE_SPACING:
	        WPD_DEBUG_MSG(("WordPerfect: parsing a line spacing change of: %f\n", m_lineSpacing));
	        listener->lineSpacingChange(m_lineSpacing);
		break;
				
	case WP3_PAGE_FORMAT_GROUP_SET_TABS:
 #ifdef DEBUG
		WPD_DEBUG_MSG(("Parsing Set Tabs Group (positions: "));
        for(tabStopsIter = m_tabStops.begin(); tabStopsIter != m_tabStops.end(); tabStopsIter++)
		{
			WPD_DEBUG_MSG((" %.4f", (*tabStopsIter).m_position));
		}
		WPD_DEBUG_MSG((")\n"));
#endif
		listener->setTabs(m_isRelative, m_tabStops);
		break;

	case WP3_PAGE_FORMAT_GROUP_VERTICAL_MARGINS:
		if (m_topMargin != 0x80000000)
			listener->pageMarginChange(WPX_TOP, fixedPointToWPUs(m_topMargin));
		if (m_bottomMargin != 0x80000000)
			listener->pageMarginChange(WPX_BOTTOM, fixedPointToWPUs(m_bottomMargin));
		break;

	case WP3_PAGE_FORMAT_GROUP_JUSTIFICATION_MODE:
		listener->justificationChange(m_justification);
		break;

	case WP3_PAGE_FORMAT_GROUP_SUPPRESS_PAGE:
		listener->suppressPage(m_suppressCode);
		break;

	case WP3_PAGE_FORMAT_GROUP_INDENT_AT_BEGINNING_OF_PARAGRAPH:
		listener->indentFirstLineChange(fixedPointToWPUs(m_indent));
		break;
		

	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
