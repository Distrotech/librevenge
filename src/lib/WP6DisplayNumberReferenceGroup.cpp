
/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP6DisplayNumberReferenceGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"

WP6DisplayNumberReferenceGroup::WP6DisplayNumberReferenceGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_levelNumberToDisplay(0)
{
	_read(input, encryption);
}

void WP6DisplayNumberReferenceGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	if (!(getSubGroup() % 2) || getSubGroup() == 0)
		m_levelNumberToDisplay = readU8(input, encryption);
}

void WP6DisplayNumberReferenceGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a DisplayNumberReference group\n"));
	
	if (!(getSubGroup() % 2) || getSubGroup() == 0)
		listener->displayNumberReferenceGroupOn(getSubGroup(), m_levelNumberToDisplay);
	else
		listener->displayNumberReferenceGroupOff(getSubGroup());
}
