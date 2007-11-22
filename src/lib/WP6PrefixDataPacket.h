/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6PREFIXDATAPACKET_H
#define WP6PREFIXDATAPACKET_H
#include "WPXParser.h"
#include "libwpd_types.h"

class WP6Listener;
class WP6PrefixIndice;
class WP6SubDocument;

class WP6PrefixDataPacket
{
public:
	WP6PrefixDataPacket(WPXInputStream * input, WPXEncryption *encryption);	
	virtual ~WP6PrefixDataPacket() {}
	virtual void parse(WP6Listener * /* listener */) const {}
	virtual WP6SubDocument * getSubDocument() const { return 0; }
	uint32_t getDataSize() const { return m_dataSize; }

	static WP6PrefixDataPacket * constructPrefixDataPacket(WPXInputStream * input, WPXEncryption *encryption, WP6PrefixIndice *prefixIndice);

protected:
	virtual void _readContents(WPXInputStream *input, WPXEncryption *encryption) = 0;
 	void _read(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataOffset, uint32_t dataSize);

private:
	uint32_t m_dataSize;
};

#endif /* WP6PREFIXDATAPACKET_H */
