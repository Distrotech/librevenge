/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6PREFIXDATAPACKET_H
#define WP6PREFIXDATAPACKET_H
#include "RVNGParser.h"
#include "librevenge_types.h"

class WP6Listener;
class WP6PrefixIndice;
class WP6SubDocument;

class WP6PrefixDataPacket
{
public:
	WP6PrefixDataPacket(RVNGInputStream *input, RVNGEncryption *encryption);
	virtual ~WP6PrefixDataPacket() {}
	virtual void parse(WP6Listener * /* listener */) const {}
	virtual WP6SubDocument *getSubDocument() const
	{
		return 0;
	}
	uint32_t getDataSize() const
	{
		return m_dataSize;
	}

	static WP6PrefixDataPacket *constructPrefixDataPacket(RVNGInputStream *input, RVNGEncryption *encryption, WP6PrefixIndice *prefixIndice);

protected:
	virtual void _readContents(RVNGInputStream *input, RVNGEncryption *encryption) = 0;
	void _read(RVNGInputStream *input, RVNGEncryption *encryption, uint32_t dataOffset, uint32_t dataSize);

private:
	uint32_t m_dataSize;
};

#endif /* WP6PREFIXDATAPACKET_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
