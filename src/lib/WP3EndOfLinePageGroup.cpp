/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
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

#include "WP3EndOfLinePageGroup.h"
#include "WP3Listener.h"
#include "libwpd_internal.h"

WP3EndOfLinePageGroup::WP3EndOfLinePageGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup()
{
	_read(input, encryption);
}

void WP3EndOfLinePageGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an End of Line/Page group\n"));

	// NOTE: the mappings like Soft End of Line -> Space come from the actual
	// file format documentation. Don't change them unless you have a very
	// good reason.

	// TODO: handle the values > 0x02
	switch(getSubGroup())
	{
	case 0x00: // Soft End of Line
		listener->insertCharacter((uint32_t) ' ');
		break;
	case 0x01: // Soft End of Page/Column
		listener->insertCharacter((uint32_t) ' ');
		break;
	case 0x02: // Hard End of Line
		listener->insertEOL();
		break;
	case 0x03: // Hard End of Line/Soft End of Page/Column
		listener->insertEOL();
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x04: // Temporary End of Line
		listener->insertEOL();
		break;
	case 0x05: // Temporary End of Page/Column
		listener->insertBreak(WPX_PAGE_BREAK);
		break;
	case 0x06: // Dormant Hard Return
		listener->insertEOL();
		break;
	case 0x07: // Hard End of Page
		listener->insertBreak(WPX_PAGE_BREAK);
		break;
	case 0x08: // Hard End of Column
		listener->insertBreak(WPX_COLUMN_BREAK);
		break;
	case 0x09: // Hard End of Column/Soft End of Page
		listener->insertBreak(WPX_COLUMN_BREAK);
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x0A: // Hard End of Line (Hard EOC not in columns)
		listener->insertEOL();
		break;
	case 0x0B: // Hard EOL/Soft EOP (Hard EOC/soft EOP not columns)
		listener->insertEOL();
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x0C: // Hard Hyphen at End of Line
		listener->insertCharacter((uint32_t) '-');
		break;
	case 0x0D: // Hard Hyphen at End of Page/Column
		listener->insertCharacter((uint32_t) '-');
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x0E: // Soft Hyphen at End of Line
		listener->insertCharacter((uint32_t) 0xad);
		break;
	case 0x0F: // Soft Hyphen at End of Page/Column
		listener->insertCharacter((uint32_t) 0xad);
		break;
	case 0x10: // Auto Hyphen at End of Line
		break;
	case 0x11: // Auto Hyphen at End of Page/Column
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x12: // (reserved)
		break;
	case 0x13: // Hard Beginning of File
		break;
	case 0x14: // Temporary Hard End of Column
		listener->insertBreak(WPX_COLUMN_BREAK);
		break;
	case 0x15: // Temporary Hard End of Column/Soft EOP
		listener->insertBreak(WPX_COLUMN_BREAK);
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x16: // Hard End of Table Cell
		listener->closeCell();
		break;
	case 0x17: // (reserved)
		break;
	case 0x18: // Hard End of Table Row/Cell
		listener->closeRow();
		break;
	case 0x19: // Hard End of Table Row/Cell/Soft EOP
		listener->closeRow();
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x1A: // Hard End of Table Row/End of Table
		listener->closeRow();
		listener->endTable();
		break;
	case 0x1B: // Hard End of Table Row/End of Table/Soft EOP
		listener->closeRow();
		listener->endTable();
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x1C: // Hard End of Table Row/Cell/End of Header
		listener->closeRow();
		break;
	case 0x1D: // Hard End of Table Row/Cell/Soft EOP/Start of Header
		listener->closeRow();
		listener->insertBreak(WPX_SOFT_PAGE_BREAK);
		break;
	case 0x1E: // (reserved)
		break;
	case 0x1F: // (reserved)
		break;
	default: // unhandled or undocumented subgroup value
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
