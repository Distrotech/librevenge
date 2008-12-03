/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

#ifndef WP42LISTENER_H
#define WP42LISTENER_H

#include "libwpd_types.h"

class WP42SubDocument;

class WP42Listener
{
public:
	WP42Listener();
	virtual ~WP42Listener() {};

	virtual void startDocument() = 0;
	virtual void startSubDocument() = 0;
	virtual void insertCharacter(uint16_t character) = 0;
	virtual void insertTab(uint8_t tabType, double tabPosition) = 0;
	virtual void insertBreak(uint8_t breakType) = 0;
	virtual void insertEOL() = 0;
	virtual void attributeChange(bool isOn, uint8_t attribute) = 0;
	virtual void marginReset(uint8_t leftMargin, uint8_t rightMargin) = 0;
	virtual void headerFooterGroup(uint8_t headerFooterDefinition, WP42SubDocument *subDocument) = 0;
	virtual void suppressPageCharacteristics(uint8_t suppressCode) = 0;
	virtual void endDocument() = 0;
	virtual void endSubDocument() = 0;
};

#endif /* WP42LISTENER_H */
