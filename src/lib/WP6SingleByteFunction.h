/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#ifndef WP6SINGLEBYTEFUNCTION_H
#define WP6SINGLEBYTEFUNCTION_H

#include "WP6Part.h"

class WP6SingleByteFunction : public WP6Part
{
 public:
	static WP6SingleByteFunction * constructSingleByteFunction(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID);
};

class WP6SpaceFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6HardSpaceFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6EOLFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6EOCFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6EOPFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6SoftEOPFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6SoftHyphenFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6HyphenFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6TableOffFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6TableCellFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6TableRowFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);	
};

class WP6TableOffAtSoftEOPFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);
};

class WP6TableRowAtSoftEOPFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);
};

class WP6TableRowAtEOPFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);
};

class WP6TableRowAtEOCFunction : public WP6SingleByteFunction
{
public:
	void parse(WP6Listener *listener);
};

#endif /* WP6SINGLEBYTEFUNCTION_H */
