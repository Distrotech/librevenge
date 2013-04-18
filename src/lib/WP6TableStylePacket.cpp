/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell Inc. (http://www.novell.com)
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
#include <string.h>

#include "WP6TableStylePacket.h"
#include "WP6Parser.h"

WP6TableStylePacket::WP6TableStylePacket(WPXInputStream *input, WPXEncryption *encryption, int  /* id */, uint32_t dataOffset, uint32_t dataSize):
	WP6PrefixDataPacket(input, encryption)
{
	_read(input, encryption, dataOffset, dataSize);
}

WP6TableStylePacket::~WP6TableStylePacket()
{
}

void WP6TableStylePacket::_readContents(WPXInputStream * /* input */, WPXEncryption * /* encryption */)
{
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
