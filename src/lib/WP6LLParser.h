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

#ifndef WP6LLPARSER_H
#define WP6LLPARSER_H

#include "WPXParser.h"
#include "WP6Header.h"

class WP6PrefixData;
class WP6LLListener;

class WP6LLParser
{
public:
	static GsfInput * getDocument(GsfInput *input);
	static WP6Header * getHeader(GsfInput *input);
	static WP6PrefixData * getPrefixData(GsfInput *input, WP6Header *header);

	static void parse(GsfInput *input, WP6Header *header, WP6LLListener *llistener);
	static void parseDocument(GsfInput *stream, WP6LLListener *llListener);

	static void parsePacket(WP6PrefixData *prefixData, int type, WP6LLListener *llListener);
	static void parsePackets(WP6PrefixData *prefixData, int type, WP6LLListener *llListener);
};

#endif /* WP6LLPARSER_H */
