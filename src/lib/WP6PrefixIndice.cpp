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

#include "WP6PrefixIndice.h"
#include "WP6FileStructure.h"
#include "libwpd_internal.h"

WP6PrefixIndice::WP6PrefixIndice(WPXInputStream *input, WPXEncryption *encryption, int id)
	: m_id(id),
	  m_type(0),
	  m_flags(0),
	  m_useCount(0),
	  m_hideCount(0),
	  m_dataSize(0),
	  m_dataOffset(0),
	  m_hasChildren(false)
{
	_read(input, encryption);
}

void WP6PrefixIndice::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	m_flags = readU8(input, encryption);
	m_type = readU8(input, encryption);

	m_useCount = readU16(input, encryption);
	m_hideCount = readU16(input, encryption);
	m_dataSize = readU32(input, encryption);
	m_dataOffset = readU32(input, encryption);

	WPD_DEBUG_MSG(("Prefix Packet (type: %i, data size: %i, data offset: %i)\n", m_type, m_dataSize, m_dataOffset));

	if (m_flags & WP6_INDEX_HEADER_ELEMENT_CHILD_PACKET_BIT)
		m_hasChildren = true;

}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
