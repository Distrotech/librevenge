/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#include "WP6SingleByteFunction.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"

WP6SingleByteFunction * WP6SingleByteFunction::constructSingleByteFunction(WPXInputStream * /* input */, WPXEncryption * /* encryption */, uint8_t groupID)
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
	listener->insertCharacter((uint16_t) ' ');
}

void WP6HardSpaceFunction::parse(WP6Listener *listener)
{
	listener->insertCharacter((uint16_t) 0xa0);
}

void WP6SoftHyphenFunction::parse(WP6Listener *listener)
{
	listener->insertCharacter((uint16_t) 0xad);
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
	listener->insertCharacter((uint16_t) '-');
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

