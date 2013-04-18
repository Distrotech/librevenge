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

#include "WP6CommentAnnotationPacket.h"
#include "WP6Parser.h"

WP6CommentAnnotationPacket::WP6CommentAnnotationPacket(WPXInputStream *input, WPXEncryption *encryption, int  /* id */, uint32_t dataOffset, uint32_t dataSize):
	WP6PrefixDataPacket(input, encryption),
	m_textPID(0),
	m_flags(0)
{
	_read(input, encryption, dataOffset, dataSize);
}

WP6CommentAnnotationPacket::~WP6CommentAnnotationPacket()
{
}

void WP6CommentAnnotationPacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	uint16_t tmpNumPrefixIDs = readU16(input, encryption);
	if (tmpNumPrefixIDs != 0x0001)
		return;
	m_textPID = readU16(input, encryption);
	m_flags = readU8(input, encryption);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
