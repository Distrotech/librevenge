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

#ifndef WP5SPECIALHEADERINDEX_H
#define WP5SPECIALHEADERINDEX_H

#include <libwpd-stream/libwpd-stream.h>
#include "libwpd_types.h"
#include "WPXEncryption.h"

class WP5SpecialHeaderIndex
{
public:
	WP5SpecialHeaderIndex(WPXInputStream *input, WPXEncryption *encryption);
	uint16_t getType() const
	{
		return m_type;
	}
	uint16_t getNumOfIndexes() const
	{
		return m_numOfIndexes;
	}
	uint16_t getIndexBlockSize() const
	{
		return m_indexBlockSize;
	}
	uint32_t getNextBlockOffset() const
	{
		return m_nextBlockOffset;
	}

protected:
	void _read(WPXInputStream *input, WPXEncryption *encryption);

private:
	uint16_t m_type;
	uint16_t m_numOfIndexes;
	uint16_t m_indexBlockSize;
	uint32_t m_nextBlockOffset;
};

#endif /* WP5SPECIALHEADERINDEX_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
