/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#ifndef WP3RESOURCE_H
#define WP3RESOURCE_H
#include "WPXParser.h"
#include "WPXString.h"
#include "WPXBinaryData.h"
#include "libwpd_types.h"

class WP3Listener;
class WP3SubDocument;

class WP3Resource
{
public:
	WP3Resource(uint32_t resourceType, uint32_t resourceReferenceID, const WPXString resourceName,
		uint8_t resourceAttributes, const WPXBinaryData m_resource);	
	virtual ~WP3Resource() {}

	const WPXString getResourceTypeString() const;
	uint32_t getResourceType() const { return m_resourceType; }
	uint32_t getResourceReferenceID() const { return m_resourceReferenceID; }
	const WPXString& getResourceName() const { return m_resourceName; }
	uint8_t getResourceAttributes() const { return m_resourceAttributes; }
	const WPXBinaryData& getResourceData() const { return m_resourceData; }
 
private:
	uint32_t m_resourceType;
	uint32_t m_resourceReferenceID;
	const WPXString m_resourceName;
	uint8_t m_resourceAttributes;
	WPXBinaryData m_resourceData;
};

#endif /* WP3RESOURCE_H */
