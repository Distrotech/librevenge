/* libwpd2
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2002 Ariya Hidayat <ariyahidayat@yahoo.de>
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

#include "WP6ExtendedCharacterGroup.h"
#include "WP6FileStructure.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6ExtendedCharacterGroup::WP6ExtendedCharacterGroup(GsfInput *input) :
	m_character(0),
	m_characterSet(0)
{
	_read(input, WP6_EXTENDED_CHARACTER_GROUP_SIZE);
}

void WP6ExtendedCharacterGroup::_readContents(GsfInput *input)
{
	m_character = gsf_le_read_guint8(input);
	m_characterSet = gsf_le_read_guint8(input);
}

void WP6ExtendedCharacterGroup::parse(WP6LLListener *llListener)
{
	llListener->insertCharacter(extendedCharacterToUCS2(m_character, m_characterSet));
}
