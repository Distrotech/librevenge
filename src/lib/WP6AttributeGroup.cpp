/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "WP6AttributeGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6AttributeGroup::WP6AttributeGroup(GsfInput *input)
	: WP6FixedLengthGroup()
{
	_read(input, WP6_ATTRIBUTE_GROUP_SIZE);
}

void WP6AttributeGroup::_readContents(GsfInput *input)
{
	m_attribute = (guint8)*((const guint8 *)gsf_input_read(input, sizeof(guint8), NULL));
}

WP6AttributeOnGroup::WP6AttributeOnGroup(GsfInput *input) : WP6AttributeGroup(input)
{
}

void WP6AttributeOnGroup::parse(WP6LLListener *llListener)
{
	// FIXME: handle flags
	llListener->attributeChange(true, getAttribute());
}

WP6AttributeOffGroup::WP6AttributeOffGroup(GsfInput *input) : WP6AttributeGroup(input)
{
}

void WP6AttributeOffGroup::parse(WP6LLListener *llListener)
{
	// FIXME: handle flags
	llListener->attributeChange(false, getAttribute());
}
