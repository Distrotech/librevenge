/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP3SINGLEBYTEFUNCTION_H
#define WP3SINGLEBYTEFUNCTION_H

#include "WP3Part.h"

class WP3SingleByteFunction : public WP3Part
{
 public:
	static WP3SingleByteFunction * constructSingleByteFunction(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID);
};

class WP3HardSpaceFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);	
};

class WP3EOLFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);	
};

class WP3EOPFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);	
};

class WP3HyphenFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);	
};

class WP3SoftHyphenFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);	
};

class WP3CondensedTabFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);
};

class WP3CondensedBackTabFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);
};

class WP3CondensedIndentFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);
};

class WP3CondensedLRIndentFunction : public WP3SingleByteFunction
{
public:
	void parse(WP3Listener *listener);
};

#endif /* WP3SINGLEBYTEFUNCTION_H */
