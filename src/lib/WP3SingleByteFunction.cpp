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

#include "WP3SingleByteFunction.h"
#include "libwpd_internal.h"
#include "WP3Listener.h"

WP3SingleByteFunction *WP3SingleByteFunction::constructSingleByteFunction(WPXInputStream * /* input */, WPXEncryption * /* encryption */, uint8_t groupID)
{

	switch (groupID)
	{
	case 0x80: // condensed hard return
		return new WP3EOLFunction();

	case 0x81: // condensed hard page
		return new WP3EOPFunction();

	case 0x82: // condensed tab
		return new WP3CondensedTabFunction();

	case 0x83: // condensed back-tab
		return new WP3CondensedBackTabFunction();

	case 0x84: // condensed indent
		return new WP3CondensedIndentFunction();

	case 0x85: // condensed left-right indent
		return new WP3CondensedLRIndentFunction();

	case 0x96: // hard hyphen in line
		return new WP3HyphenFunction();

	case 0x97: // soft hyphen in line
		return new WP3SoftHyphenFunction();

	case 0xa0: // hard space
		return new WP3HardSpaceFunction();

	default:
		// should not happen
		return 0;
	}
}

void WP3HardSpaceFunction::parse(WP3Listener *listener)
{
	listener->insertCharacter((uint32_t) 0xa0);
}

void WP3HyphenFunction::parse(WP3Listener *listener)
{
	listener->insertCharacter((uint32_t) '-');
}

void WP3SoftHyphenFunction::parse(WP3Listener *listener)
{
	listener->insertCharacter((uint32_t) 0xad);
}

void WP3EOLFunction::parse(WP3Listener *listener)
{
	listener->insertEOL();
}

void WP3EOPFunction::parse(WP3Listener *listener)
{
	listener->insertBreak(WPX_PAGE_BREAK);
}

void WP3CondensedTabFunction::parse(WP3Listener *listener)
{
	listener->insertTab();
}

void WP3CondensedBackTabFunction::parse(WP3Listener *listener)
{
	listener->backTab();
}

void WP3CondensedIndentFunction::parse(WP3Listener *listener)
{
	listener->leftIndent();
}

void WP3CondensedLRIndentFunction::parse(WP3Listener *listener)
{
	listener->leftRightIndent();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
