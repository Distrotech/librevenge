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

#ifndef WP6EXTENDEDDOCUMENTSUMMARYPACKET_H
#define WP6EXTENDEDDOCUMENTSUMMARYPACKET_H
#include "WP6PrefixDataPacket.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "WPXStream.h"
#include "WPXMemoryStream.h"

class WP6ExtendedDocumentSummaryPacket : public WP6PrefixDataPacket
{
 public:
	WP6ExtendedDocumentSummaryPacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP6ExtendedDocumentSummaryPacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	void parse(WP6Listener *listener) const;

 private:
 	WP6ExtendedDocumentSummaryPacket(const WP6ExtendedDocumentSummaryPacket&);            
 	WP6ExtendedDocumentSummaryPacket& operator=(const WP6ExtendedDocumentSummaryPacket&);            
	uint32_t m_dataSize;
	uint8_t *m_streamData;
	mutable WPXMemoryInputStream *m_stream;
	
};
#endif /* WP6EXTENDEDDOCUMENTSUMMARYPACKET_H */
