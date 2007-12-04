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

#ifndef WP6PARSER_H
#define WP6PARSER_H

#include "WPXParser.h"
#include "WP6Header.h"

class WP6PrefixData;
class WP6Listener;
class WPXDocumentInterface;

class WP6Parser : public WPXParser
{
public:
	WP6Parser(WPXInputStream *input, WPXHeader *header, WPXEncryption *encryption);
	~WP6Parser();

	void parse(WPXDocumentInterface *documentInterface);
	void parseSubDocument(WPXDocumentInterface *documentInterface);

	static void parseDocument(WPXInputStream *input, WPXEncryption *encryption, WP6Listener *listener);

private:
	WP6PrefixData * getPrefixData(WPXInputStream *input, WPXEncryption *encryption);

	void parse(WPXInputStream *input, WPXEncryption *encryption, WP6Listener *listener);

	void parsePacket(WP6PrefixData *prefixData, int type, WP6Listener *listener);
	void parsePackets(WP6PrefixData *prefixData, int type, WP6Listener *listener);
};

#endif /* WP6PARSER_H */
