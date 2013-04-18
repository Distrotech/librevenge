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
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
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

#ifndef WP6PREFIXDATA_H
#define WP6PREFIXDATA_H
#include "WP6PrefixDataPacket.h"
#include <map>
#include <vector>

typedef std::map<int, WP6PrefixDataPacket *> DPH;
typedef std::multimap<int, WP6PrefixDataPacket *> MPDP;
typedef MPDP::const_iterator MPDP_CIter;
typedef MPDP::iterator MPDP_Iter;

class WP6PrefixData
{
public:
	WP6PrefixData(WPXInputStream *input, WPXEncryption *encryption, const int numPrefixIndices);
	virtual ~WP6PrefixData();
	const WP6PrefixDataPacket *getPrefixDataPacket(const int prefixID) const;
	std::pair< MPDP_CIter, MPDP_CIter > getPrefixDataPacketsOfType(const int type) const;

	uint16_t getDefaultInitialFontPID() const
	{
		return (uint16_t)m_defaultInitialFontPID;
	}

private:
	DPH m_prefixDataPacketHash;
	MPDP m_prefixDataPacketTypeHash;
	int m_defaultInitialFontPID;
};

#endif /* WP6PREFIXDATA_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
