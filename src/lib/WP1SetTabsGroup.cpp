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

#include "WP1SetTabsGroup.h"
#include "libwpd_internal.h"
#include <vector>

WP1SetTabsGroup::WP1SetTabsGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP1VariableLengthGroup(group),
	m_tabStops(std::vector<WPXTabStop>())
{
	_read(input, encryption);
}

WP1SetTabsGroup::~WP1SetTabsGroup()
{
}

void WP1SetTabsGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// Skip first the old condensed tab table
	while (readU8(input, encryption) != 0xff && !input->atEOS())
		input->seek(2, WPX_SEEK_CUR);

	// Now read the new condensed tab table
	int8_t tmpTabType = 0;
	double tmpTabPosition = 0.0;
	WPXTabStop tmpTabStop = WPXTabStop();

	while (((tmpTabType = (int8_t)readU8(input, encryption)) & 0xff) != 0xff)
	{
		if (input->atEOS())
			throw FileException();
		tmpTabPosition = (double)((double)readU16(input, encryption, true) / 72.0);

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

			switch (tmpTabType & 0x03)
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
			default:
				tmpTabStop.m_alignment = LEFT;
				break;
			}

			if (tmpTabType & 0x04)
			{
				tmpTabStop.m_leaderCharacter = '.';
				tmpTabStop.m_leaderNumSpaces = 0;
			}
			else
			{
				tmpTabStop.m_leaderCharacter = '\0';
				tmpTabStop.m_leaderNumSpaces = 0;
			}

			m_tabStops.push_back(tmpTabStop);
		}	
	}
}

void WP1SetTabsGroup::parse(WP1Listener *listener)
{
#ifdef DEBUG
	WPD_DEBUG_MSG(("Parsing Set Tabs Group (positions: "));
	for(std::vector<WPXTabStop>::const_iterator i = m_tabStops.begin(); i != m_tabStops.end(); i++)
	{
		WPD_DEBUG_MSG((" %.4f", (*i).m_position));
	}
	WPD_DEBUG_MSG((")\n"));
#endif
	listener->setTabs(m_tabStops);
}
