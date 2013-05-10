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

#ifndef WP6PREFIXINDICE_H
#define WP6PREFIXINDICE_H

#include <libwpd-stream/libwpd-stream.h>
#include "libwpd_types.h"

class WPXEncryption;

class WP6PrefixIndice
{
public:
	WP6PrefixIndice(WPXInputStream *input, WPXEncryption *encryption, int id);
	int getID() const
	{
		return m_id;
	}
	uint8_t getType() const
	{
		return m_type;
	}
	uint8_t getFlags() const
	{
		return m_flags;
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
	void _read(WPXInputStream *input, WPXEncryption *encryption);

private:
	int m_id;
	uint8_t m_type;
	uint8_t m_flags;
	uint16_t m_useCount;
	uint16_t m_hideCount;
	uint32_t m_dataSize;
	uint32_t m_dataOffset;

	bool m_hasChildren;
};

#endif /* WP6PREFIXINDICE_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
