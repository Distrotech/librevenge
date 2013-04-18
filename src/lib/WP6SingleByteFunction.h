/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
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
	static WP6SingleByteFunction *constructSingleByteFunction(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID);
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
