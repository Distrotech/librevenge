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

#ifndef WP6STYLEGROUP_H
#define WP6STYLEGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6StyleGroup_GlobalOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6StyleGroup_GlobalOnSubGroup(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint16_t m_hash;
	uint8_t m_systemStyleNumber;
};

class WP6StyleGroup : public WP6VariableLengthGroup
{
public:
	WP6StyleGroup(WPXInputStream *input, WPXEncryption *encryption);
	~WP6StyleGroup();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener);

private:
	WP6StyleGroup(const WP6StyleGroup &);
	WP6StyleGroup &operator=(const WP6StyleGroup &);
	WP6VariableLengthGroup_SubGroup *m_subGroupData;
};

#endif /* WP6STYLEGROUP_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
