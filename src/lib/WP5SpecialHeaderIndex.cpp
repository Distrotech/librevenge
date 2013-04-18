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

#include "WP5SpecialHeaderIndex.h"
#include "libwpd_internal.h"

WP5SpecialHeaderIndex::WP5SpecialHeaderIndex(WPXInputStream *input, WPXEncryption *encryption) :
	m_type(0),
	m_numOfIndexes(0),
	m_indexBlockSize(0),
	m_nextBlockOffset(0)
{
	_read(input, encryption);
}

void WP5SpecialHeaderIndex::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	m_type = readU16(input, encryption);

	m_numOfIndexes = readU16(input, encryption);

	m_indexBlockSize = readU16(input, encryption);
	m_nextBlockOffset = readU32(input, encryption);

	WPD_DEBUG_MSG(("Special Header Index (type: %i, number of indexes: %i, index block size: %i, next block offset: %i)\n",
	               m_type, m_numOfIndexes, m_indexBlockSize, m_nextBlockOffset));
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
