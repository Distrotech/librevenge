/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP5GENERALPACKETINDEX_H
#define WP5GENERALPACKETINDEX_H

#include <librevenge-stream/librevenge-stream.h>
#include "librevenge_types.h"

class RVNGEncryption;

class WP5GeneralPacketIndex
{
public:
	WP5GeneralPacketIndex(RVNGInputStream *input, RVNGEncryption *encryption, int id);
	int getID() const
	{
		return m_id;
	}
	uint16_t getType() const
	{
		return m_type;
	}
	uint32_t getDataSize() const
	{
		return m_dataSize;
	}
	uint32_t getDataOffset() const
	{
		return m_dataOffset;
	}

protected:
	void _read(RVNGInputStream *input, RVNGEncryption *encryption);

private:
	int m_id;
	uint16_t m_type;
	uint32_t m_dataSize;
	uint32_t m_dataOffset;
};

#endif /* WP5GENERALPACKETINDEX_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
