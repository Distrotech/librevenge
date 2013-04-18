/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

	const WP3Resource *getResource(uint32_t type, uint32_t ID) const;

private:
	std::multimap<uint32_t, WP3Resource *> m_resourcesTypeMultimap, m_resourcesIDMultimap;
};

#endif /* WP3RESOURCEFORK_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
