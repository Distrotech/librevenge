/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP42MultiByteFunctionGroup.h"
#include "WP42UnsupportedMultiByteFunctionGroup.h"
#include "WP42HeaderFooterGroup.h"
#include "WP42MarginResetGroup.h"
#include "WP42SuppressPageCharacteristicsGroup.h"
#include "WP42ExtendedCharacterGroup.h"
#include "WP42DefineColumnsGroup.h"
#include "WP42FileStructure.h"
#include "libwpd_internal.h"

WP42MultiByteFunctionGroup::WP42MultiByteFunctionGroup(uint8_t group)
	: m_group(group)
{
}

WP42MultiByteFunctionGroup *WP42MultiByteFunctionGroup::constructMultiByteFunctionGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group)
{
	switch (group)
	{
	case WP42_MARGIN_RESET_GROUP:
		return new WP42MarginResetGroup(input, encryption, group);
	case WP42_SUPPRESS_PAGE_CHARACTERISTICS_GROUP:
		return new WP42SuppressPageCharacteristicsGroup(input, encryption, group);
	case WP42_HEADER_FOOTER_GROUP:
		return new WP42HeaderFooterGroup(input, encryption, group);
	case WP42_EXTENDED_CHARACTER_GROUP:
		return new WP42ExtendedCharacterGroup(input, encryption, group);
	case WP42_DEFINE_COLUMNS_OLD_GROUP:
	case WP42_DEFINE_COLUMNS_NEW_GROUP:
		return new WP42DefineColumnsGroup(input, encryption, group);
	default:
		// this is an unhandled group, just skip it
		return new WP42UnsupportedMultiByteFunctionGroup(input, encryption, group);
	}
}

void WP42MultiByteFunctionGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	_readContents(input, encryption);

	// skip over the remaining bytes of the group, if any
	while (!input->atEOS() && (readU8(input, encryption) != m_group)) // getGroup()));
	{
	}
	// IMPORTANT: if the class that implements _readContent(input, encryption) already reads the closing gate,
	// IMPORTANT: it is necessary to make an input->seek(-1, WPX_SEEK_CUR) for this function to work well.
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
