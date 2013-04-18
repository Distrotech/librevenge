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

#include "WP6GraphicsFilenamePacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6GraphicsFilenamePacket::WP6GraphicsFilenamePacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, const uint8_t flags, uint32_t dataOffset, uint32_t dataSize):
	WP6PrefixDataPacket(input, encryption),
	m_childIds(),
	m_flags(flags)
{
	_read(input, encryption, dataOffset, dataSize);
}

WP6GraphicsFilenamePacket::~WP6GraphicsFilenamePacket()
{
}

void WP6GraphicsFilenamePacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	if ((m_flags & 0x01) == 0x00)
		return;
	uint16_t tmpNumChildIds = readU16(input, encryption);
	for (uint16_t i = 0; i < tmpNumChildIds; i++)
		m_childIds.push_back(readU16(input, encryption));
}

const std::vector<unsigned> WP6GraphicsFilenamePacket::getChildIds() const
{
	return m_childIds;
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
