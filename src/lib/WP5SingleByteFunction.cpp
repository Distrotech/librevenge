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

#include "WP5SingleByteFunction.h"
#include "WP5Listener.h"
#include "libwpd_internal.h"

WP5SingleByteFunction *WP5SingleByteFunction::constructSingleByteFunction(WPXInputStream * /* input */, WPXEncryption * /* encryption */, uint8_t groupID)
{

	switch (groupID)
	{
	case 0x8c: // combination hard return/soft page
	case 0x90: // deletable return at EOL
	case 0x99: // dormant hard return
		return new WP5EOLFunction();

	case 0x93: // invisible return in line
	case 0x94: // invisible return EOL
	case 0x95: // invisible return EOP
		return new WP5SpaceFunction();

	case 0xa0: // hard space
		return new WP5HardSpaceFunction();

	case 0xa9: // hard hyphen in line
	case 0xaa: // hard hyphen EOL
	case 0xab: // hard hyphen EOP
		return new WP5HyphenFunction();

	case 0xac: // soft hyphen in line
	case 0xad: // soft hyphen EOL
	case 0xae: // soft hyphen EOP
		return new WP5SoftHyphenFunction();

	default:
		// should not happen
		return 0;
	}
}

void WP5EOLFunction::parse(WP5Listener *listener)
{
	listener->insertEOL();
}

void WP5SpaceFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint32_t) ' ');
}

void WP5HardSpaceFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint32_t) 0xa0);
}

void WP5HyphenFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint32_t) '-');
}

void WP5SoftHyphenFunction::parse(WP5Listener *listener)
{
	listener->insertCharacter((uint32_t) 0xad);
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
