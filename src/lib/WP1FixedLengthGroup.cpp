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

#include "WP1FixedLengthGroup.h"
#include "WP1SuppressPageCharacteristicsGroup.h"
#include "WP1UnsupportedFixedLengthGroup.h"
#include "WP1MarginResetGroup.h"
#include "WP1MarginReleaseGroup.h"
#include "WP1TopMarginGroup.h"
#include "WP1BottomMarginGroup.h"
#include "WP1ExtendedCharacterGroup.h"
#include "WP1PointSizeGroup.h"
#include "WP1LeftIndentGroup.h"
#include "WP1FontIdGroup.h"
#include "WP1LeftRightIndentGroup.h"
#include "WP1JustificationGroup.h"
#include "WP1SpacingResetGroup.h"
#include "WP1CenterTextGroup.h"
#include "WP1FlushRightGroup.h"
#include "WP1FileStructure.h"
#include "libwpd_internal.h"

WP1FixedLengthGroup::WP1FixedLengthGroup(uint8_t group)
	: m_group(group)
{
}

WP1FixedLengthGroup *WP1FixedLengthGroup::constructFixedLengthGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group)
{
	switch (group)
	{
	case WP1_MARGIN_RESET_GROUP:
		return new WP1MarginResetGroup(input, encryption, group);
	case WP1_TOP_MARGIN_SET_GROUP:
		return new WP1TopMarginGroup(input, encryption, group);
	case WP1_BOTTOM_MARGIN_SET_GROUP:
		return new WP1BottomMarginGroup(input, encryption, group);
	case WP1_LEFT_INDENT_GROUP:
		return new WP1LeftIndentGroup(input, encryption, group);
	case WP1_SUPPRESS_PAGE_CHARACTERISTICS_GROUP:
		return new WP1SuppressPageCharacteristicsGroup(input, encryption, group);
	case WP1_LEFT_RIGHT_INDENT_GROUP:
		return new WP1LeftRightIndentGroup(input, encryption, group);
	case WP1_FONT_ID_GROUP:
		return new WP1FontIdGroup(input, encryption, group);
	case WP1_MARGIN_RELEASE_GROUP:
		return new WP1MarginReleaseGroup(input, encryption, group);
	case WP1_CENTER_TEXT_GROUP:
		return new WP1CenterTextGroup(input, encryption, group);
	case WP1_FLUSH_RIGHT_GROUP:
		return new WP1FlushRightGroup(input, encryption, group);
	case WP1_EXTENDED_CHARACTER_GROUP:
		return new WP1ExtendedCharacterGroup(input, encryption, group);
	case WP1_POINT_SIZE_GROUP:
		return new WP1PointSizeGroup(input, encryption, group);
	case WP1_JUSTIFICATION_GROUP:
		return new WP1JustificationGroup(input, encryption, group);
	case WP1_SPACING_RESET_GROUP:
		return new WP1SpacingResetGroup(input, encryption, group);
	default:
		// this is an unhandled group, just skip it
		return new WP1UnsupportedFixedLengthGroup(input, encryption, group);
	}
}

void WP1FixedLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	long startPosition = input->tell();

	if (m_group >= 0xC0 && m_group <= 0xFE) // just an extra safety check
	{
		int size = WP1_FUNCTION_GROUP_SIZE[m_group-0xC0];
		if (size == -1)
			return;

		_readContents(input, encryption);

		input->seek((startPosition + size - 2), WPX_SEEK_SET);
		if (m_group != readU8(input, encryption))
		{
			WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
			throw FileException();
		}
	}
	else
		throw FileException();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
