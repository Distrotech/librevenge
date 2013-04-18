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
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP6SingleByteFunction.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"

WP6SingleByteFunction *WP6SingleByteFunction::constructSingleByteFunction(WPXInputStream * /* input */, WPXEncryption * /* encryption */, uint8_t groupID)
{

	switch (groupID)
	{
	case WP6_TOP_SOFT_EOL:
	case WP6_TOP_SOFT_EOL_AT_EOC:
	case WP6_TOP_SOFT_EOL_AT_EOC_AT_EOP:
	case WP6_TOP_SOFT_SPACE:
		return new WP6SpaceFunction();

	case WP6_TOP_HARD_SPACE:
		return new WP6HardSpaceFunction();

	case WP6_TOP_SOFT_HYPHEN_IN_LINE:
	case WP6_TOP_SOFT_HYPHEN_AT_EOL:
		return new WP6SoftHyphenFunction();

	case WP6_TOP_HARD_HYPHEN:
		return new WP6HyphenFunction();

	case WP6_TOP_HARD_EOL:
	case WP6_TOP_HARD_EOL_AT_EOC:
	case WP6_TOP_HARD_EOL_AT_EOC_AT_EOP:
	case WP6_TOP_DORMANT_HARD_RETURN:
	case WP6_TOP_DELETABLE_HARD_EOL:
	case WP6_TOP_DELETABLE_HARD_EOL_AT_EOC:
	case WP6_TOP_DELETABLE_HARD_EOL_AT_EOC_AT_EOP:
		return new WP6EOLFunction();

	case WP6_TOP_TABLE_OFF_AT_EOC:
	case WP6_TOP_TABLE_OFF:
		return new WP6TableOffFunction();

	case WP6_TOP_TABLE_OFF_AT_EOC_AT_EOP:
		return new WP6TableOffAtSoftEOPFunction();

	case WP6_TOP_TABLE_ROW_AT_EOC:
	case WP6_TOP_TABLE_ROW:
		return new WP6TableRowFunction();

	case WP6_TOP_TABLE_ROW_AT_HARD_EOC:
		return new WP6TableRowAtEOCFunction();

	case WP6_TOP_TABLE_ROW_AT_EOC_AT_EOP:
		return new WP6TableRowAtSoftEOPFunction();

	case WP6_TOP_TABLE_ROW_AT_HARD_EOP:
	case WP6_TOP_TABLE_ROW_AT_HARD_EOC_AT_HARD_EOP:
		return new WP6TableRowAtEOPFunction();

	case WP6_TOP_TABLE_CELL:
		return new WP6TableCellFunction();

	case WP6_TOP_HARD_EOC:
	case WP6_TOP_HARD_EOC_AT_EOP:
	case WP6_TOP_DELETABLE_HARD_EOC:
	case WP6_TOP_DELETABLE_HARD_EOC_AT_EOP:
		return new WP6EOCFunction();

	case WP6_TOP_HARD_EOP:
	case WP6_TOP_DELETABLE_HARD_EOP:
		return new WP6EOPFunction();

		// Add the remaining cases here
	default:
		// should not happen
		return 0;
	}
}

void WP6SpaceFunction::parse(WP6Listener *listener)
{
	listener->insertCharacter((uint32_t) ' ');
}

void WP6HardSpaceFunction::parse(WP6Listener *listener)
{
	listener->insertCharacter((uint32_t) 0xa0);
}

void WP6SoftHyphenFunction::parse(WP6Listener *listener)
{
	listener->insertCharacter((uint32_t) 0xad);
}

void WP6EOLFunction::parse(WP6Listener *listener)
{
	listener->insertEOL();
}

void WP6EOCFunction::parse(WP6Listener *listener)
{
	listener->insertBreak(WPX_COLUMN_BREAK);
}

void WP6EOPFunction::parse(WP6Listener *listener)
{
	listener->insertBreak(WPX_PAGE_BREAK);
}

void WP6SoftEOPFunction::parse(WP6Listener *listener)
{
	listener->insertBreak(WPX_SOFT_PAGE_BREAK);
}

void WP6HyphenFunction::parse(WP6Listener *listener)
{
	listener->insertCharacter((uint32_t) '-');
}

void WP6TableRowFunction::parse(WP6Listener *listener)
{
	// use default values: if they were not default values, WordPerfect
	// would use the multi-byte variant of this function
	listener->insertRow(0x0000, true, false);
	RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
	listener->insertCell(1, 1, 0x00, 0, 0, &tmpCellBorderColor, TOP, false, 0x00000000);
}

void WP6TableCellFunction::parse(WP6Listener *listener)
{
	// default values
	RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
	listener->insertCell(1, 1, 0x00, 0, 0, &tmpCellBorderColor, TOP, false, 0x00000000);
}

void WP6TableOffFunction::parse(WP6Listener *listener)
{
	listener->endTable();
}

void WP6TableOffAtSoftEOPFunction::parse(WP6Listener *listener)
{
	listener->endTable();
	listener->insertBreak(WPX_SOFT_PAGE_BREAK);
}

void WP6TableRowAtSoftEOPFunction::parse(WP6Listener *listener)
{
	// use default values: if they were not default values, WordPerfect
	// would use the multi-byte variant of this function
	listener->insertRow(0x0000, true, false);
	RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
	listener->insertCell(1, 1, 0x00, 0, 0, &tmpCellBorderColor, TOP, false, 0x00000000);
	listener->insertBreak(WPX_SOFT_PAGE_BREAK);
}

void WP6TableRowAtEOPFunction::parse(WP6Listener *listener)
{
	// use default values: if they were not default values, WordPerfect
	// would use the multi-byte variant of this function
	listener->insertRow(0x0000, true, false);
	RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
	listener->insertCell(1, 1, 0x00, 0, 0, &tmpCellBorderColor, TOP, false, 0x00000000);
	listener->insertBreak(WPX_PAGE_BREAK);
}

void WP6TableRowAtEOCFunction::parse(WP6Listener *listener)
{
	// use default values: if they were not default values, WordPerfect
	// would use the multi-byte variant of this function
	listener->insertRow(0x0000, true, false);
	RGBSColor tmpCellBorderColor(0x00, 0x00, 0x00, 0x64);
	listener->insertCell(1, 1, 0x00, 0, 0, &tmpCellBorderColor, TOP, false, 0x00000000);
	listener->insertBreak(WPX_COLUMN_BREAK);
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
