/* libwpd2
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

#ifndef WP6PREFIXDATAPACKET_H
#define WP6PREFIXDATAPACKET_H
#include <gsf/gsf-input.h>
#include <stdlib.h>
#include <glib.h>
//#include "WP6LLListener.h"

class WP6PrefixIndice;
class WP6LLListener;

class WP6PrefixDataPacket
{
 public:
	WP6PrefixDataPacket(GsfInput * input);	
	virtual ~WP6PrefixDataPacket() {}
	virtual void parse(WP6LLListener *llListener) const {}

	static WP6PrefixDataPacket * constructPrefixDataPacket(GsfInput * input, WP6PrefixIndice *prefixIndice);

 protected:
	virtual void _readContents(GsfInput *input) = 0;
 	void _read(GsfInput *input, guint32 dataOffset, guint32 dataSize);
};

#endif /* WP6PREFIXDATAPACKET_H */
