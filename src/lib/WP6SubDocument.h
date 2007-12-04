/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP6SUBDOCUMENT_H
#define WP6SUBDOCUMENT_H

#include "WPXMemoryStream.h"
#include "WPXSubDocument.h"
#include "WP6Listener.h"

class WP6SubDocument : public WPXSubDocument
{
public:
	WP6SubDocument(WPXInputStream *input, WPXEncryption *encryption, const unsigned dataSize);
	WP6SubDocument(uint8_t * streamData, const unsigned dataSize);
	void parse(WP6Listener *listener) const;
};
#endif /* WP6SUBDOCUMENT_H */
