/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
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
#include "WP6HLListener.h"
#include "WPXStream.h"
#include "WPXMemoryStream.h"

class WP6ExtendedDocumentSummaryPacket : public WP6PrefixDataPacket
{
 public:
	WP6ExtendedDocumentSummaryPacket(WPXInputStream *input, int id, uint32_t dataOffset, uint32_t dataSize);
	virtual ~WP6ExtendedDocumentSummaryPacket();
	virtual void _readContents(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener) const;

 private:              
	uint16_t m_dataSize;
	mutable WPXMemoryInputStream *m_stream;
	
};
#endif /* WP6EXTENDEDDOCUMENTSUMMARYPACKET_H */
