/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2007 Fridrich Strba (fridrich_strba@bluewin.ch)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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

#include "WP3Resource.h"
#include "WP3FileStructure.h"
#include "libwpd_internal.h"

WP3Resource::WP3Resource(uint32_t resourceType, uint32_t resourceReferenceID, const WPXString resourceName,
		uint8_t resourceAttributes, const WPXBinaryData resourceData) :
	m_resourceType(resourceType),
	m_resourceReferenceID(resourceReferenceID),
	m_resourceName(resourceName),
	m_resourceAttributes(resourceAttributes),
	m_resourceData(resourceData)
{
}

const WPXString WP3Resource::getResourceTypeString() const
{
	WPXString resourceTypeString;
	for (int j=3; j >= 0; j--)
		resourceTypeString.append((char)((m_resourceType >> (j*8)) & 0xff));
	return resourceTypeString;
}
