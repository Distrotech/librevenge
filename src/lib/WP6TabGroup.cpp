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

#include "WP6TabGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6TabGroup::WP6TabGroup(WPXInputStream *input) :
	WP6VariableLengthGroup(),
	m_position(0),
	m_ignoreFunction(false)
{
	_read(input);
}

void WP6TabGroup::_readContents(WPXInputStream *input)
{
	if ((getFlags() & 0x40) == 0x40) // 0x40 is "ignore function" flag
	{
		m_ignoreFunction = true;
	}
	if ((getSize() >= 12) & (getSize() <= 18)) // Minimum size of the function if the position information is present
	{
		input->seek((getSize() - 12), WPX_SEEK_CUR);
		m_position = readU16(input);
	}
	else if (getSize() > 18)
	{
		input->seek(6, WPX_SEEK_CUR);
		m_position = readU16(input);
	}
}

void WP6TabGroup::parse(WP6HLListener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Tab group (Tab type: %i, Tab position: %i, Ignore function: %s)\n", 
			getSubGroup(), m_position, (m_ignoreFunction?"true":"false")));
	if (!m_ignoreFunction)
	{
		WPD_DEBUG_MSG(("WordPerfect: Parsing a Tab group\n"));		
		listener->insertTab(getSubGroup(), m_position);
	}
}
