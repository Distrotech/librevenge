/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#ifndef WP5SINGLEBYTEFUNCTION_H
#define WP5SINGLEBYTEFUNCTION_H

#include "WP5Part.h"

class WP5SingleByteFunction : public WP5Part
{
 public:
	static WP5SingleByteFunction * WP5SingleByteFunction::constructSingleByteFunction(WPXInputStream *input, uint8_t groupID);
};

class WP5SpaceFunction : public WP5SingleByteFunction
{
public:
	virtual void parse(WP5HLListener *listener);	
};

class WP5HardSpaceFunction : public WP5SingleByteFunction
{
public:
	virtual void parse(WP5HLListener *listener);	
};


class WP5HyphenFunction : public WP5SingleByteFunction
{
public:
	virtual void parse(WP5HLListener *listener);	
};

class WP5SoftHyphenFunction : public WP5SingleByteFunction
{
public:
	virtual void parse(WP5HLListener *listener);	
};

#endif /* WP5SINGLEBYTEFUNCTION_H */
