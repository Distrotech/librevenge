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
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3SINGLEBYTEFUNCTION_H
#define WP3SINGLEBYTEFUNCTION_H

#include "WP3Part.h"

class WP3SingleByteFunction : public WP3Part
{
public:
	static WP3SingleByteFunction *constructSingleByteFunction(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID);
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
