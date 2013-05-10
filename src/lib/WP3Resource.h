/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP3RESOURCE_H
#define WP3RESOURCE_H

#include <libwpd/libwpd.h>
#include "WPXParser.h"
#include "libwpd_types.h"

class WP3Listener;
class WP3SubDocument;

class WP3Resource
{
public:
	WP3Resource(uint32_t resourceType, uint32_t resourceReferenceID, const WPXString &resourceName,
	            uint8_t resourceAttributes, const WPXBinaryData &resourceData);
	virtual ~WP3Resource() {}

	uint32_t getResourceType() const
	{
		return m_resourceType;
	}
	uint32_t getResourceReferenceID() const
	{
		return m_resourceReferenceID;
	}
	const WPXString &getResourceName() const
	{
		return m_resourceName;
	}
	uint8_t getResourceAttributes() const
	{
		return m_resourceAttributes;
	}
	const WPXBinaryData &getResourceData() const
	{
		return m_resourceData;
	}

private:
	uint32_t m_resourceType;
	uint32_t m_resourceReferenceID;
	const WPXString m_resourceName;
	uint8_t m_resourceAttributes;
	WPXBinaryData m_resourceData;
};

#endif /* WP3RESOURCE_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
