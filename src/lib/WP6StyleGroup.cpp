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

#include "WP6StyleGroup.h"
#include "WP6Listener.h"
#include "WP6FileStructure.h"
#include "libwpd_internal.h"

WP6StyleGroup_GlobalOnSubGroup::WP6StyleGroup_GlobalOnSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_hash(0), m_systemStyleNumber(0)
{
	m_hash = readU16(input, encryption);
	m_systemStyleNumber = readU8(input, encryption);
}

void WP6StyleGroup_GlobalOnSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */, uint16_t const * /* prefixIDs */) const
{
	listener->globalOn(m_systemStyleNumber);
}

WP6StyleGroup::WP6StyleGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_subGroupData(0)
{
	_read(input, encryption);
}

WP6StyleGroup::~WP6StyleGroup()
{
	if (m_subGroupData)
		delete(m_subGroupData);

}

void WP6StyleGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP6_STYLE_GROUP_GLOBAL_ON:
		m_subGroupData = new WP6StyleGroup_GlobalOnSubGroup(input, encryption);
		break;
	default:
		break;
	}
}

void WP6StyleGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a style group\n"));

	if (getSubGroup() == WP6_STYLE_GROUP_GLOBAL_ON)
	{
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
	}
	else if (getSubGroup() == WP6_STYLE_GROUP_GLOBAL_OFF)
	{
		listener->globalOff();
	}
	else
	{
		if (!(getSubGroup() % 2) || getSubGroup() == 0)
			listener->styleGroupOn(getSubGroup());
		else
			listener->styleGroupOff(getSubGroup());
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
