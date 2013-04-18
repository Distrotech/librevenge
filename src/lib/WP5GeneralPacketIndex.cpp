/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5GeneralPacketIndex.h"
#include "libwpd_internal.h"

WP5GeneralPacketIndex::WP5GeneralPacketIndex(WPXInputStream *input, WPXEncryption *encryption, int id)
	: m_id(id),
	  m_type(0),
	  m_dataSize(0),
	  m_dataOffset(0)
{
	_read(input, encryption);
}

void WP5GeneralPacketIndex::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	m_type = readU16(input, encryption);

	m_dataSize = readU32(input, encryption);
	m_dataOffset = readU32(input, encryption);

	WPD_DEBUG_MSG(("General Packet Index (id: %i, type: %i, data size: %u, data offset: %u)\n", m_id, m_type, m_dataSize, m_dataOffset));
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
