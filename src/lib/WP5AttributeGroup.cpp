/* libwpd2
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

#include "WP5AttributeGroup.h"
#include "WP5LLListener.h"
#include "libwpd_internal.h"

WP5AttributeGroup::WP5AttributeGroup(GsfInput *input, guint8 groupID)
	: WP5FixedLengthGroup(groupID)
{
	_read(input);
}

void WP5AttributeGroup::_readContents(GsfInput *input)
{
	m_attribute = (guint8)*((const guint8 *)gsf_input_read(input, sizeof(guint8), NULL));
}

WP5AttributeOnGroup::WP5AttributeOnGroup(GsfInput *input, guint8 groupID)
	: WP5AttributeGroup(input, groupID)
{
}

void WP5AttributeOnGroup::parse(WP5LLListener *llListener)
{
	llListener->attributeChange(true, getAttribute());
}

WP5AttributeOffGroup::WP5AttributeOffGroup(GsfInput *input, guint8 groupID)
	: WP5AttributeGroup(input, groupID)
{
}

void WP5AttributeOffGroup::parse(WP5LLListener *llListener)
{
	llListener->attributeChange(false, getAttribute());
}
