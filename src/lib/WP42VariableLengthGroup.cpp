/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "WP42VariableLengthGroup.h"
#include "WP42UnsupportedVariableLengthGroup.h"
#include "libwpd_internal.h"

WP42VariableLengthGroup::WP42VariableLengthGroup(guint8 group)
	: m_group(group)
{
}

WP42VariableLengthGroup * WP42VariableLengthGroup::constructVariableLengthGroup(WPXInputStream *input, guint8 group)
{
	switch (group)
	{
		default:
			// this is an unhandled group, just skip it
			return new WP42UnsupportedVariableLengthGroup(input, group);
	}
}

void WP42VariableLengthGroup::_read(WPXInputStream *input)
{
	_readContents(input);
	
	// skip over the remaining bytes of the group, if any
	while (!input->atEOS())
	{
		guint8 readNextVal;
		readNextVal = gsf_le_read_guint8(input);
		if (readNextVal == getGroup())
			break;
	}	
	
}
