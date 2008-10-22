/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3RESOURCEFORK_H
#define WP3RESOURCEFORK_H
#include "WP3Resource.h"
#include <map>
#include <vector>

class WPXInputStream;
class WPXEncryption;

class WP3ResourceFork
{
 public:
	WP3ResourceFork(WPXInputStream *input, WPXEncryption *encryption);
	virtual ~WP3ResourceFork();

	std::pair< std::multimap<uint32_t, WP3Resource *>::const_iterator, std::multimap<uint32_t, WP3Resource *>::const_iterator > getResourcesByType(uint32_t type) const;
	std::pair< std::multimap<uint32_t, WP3Resource *>::const_iterator, std::multimap<uint32_t, WP3Resource *>::const_iterator > getResourcesByID(uint32_t ID) const;
	const WP3Resource * getResource(uint32_t type, uint32_t ID) const;

private:
	std::multimap<uint32_t, WP3Resource *> m_resourcesTypeMultimap, m_resourcesIDMultimap;
};

#endif /* WP3RESOURCEFORK_H */
