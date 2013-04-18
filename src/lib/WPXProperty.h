/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
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

#ifndef WPXPROPERTY_H
#define WPXPROPERTY_H
#include "WPXString.h"

enum WPXUnit { WPX_INCH, WPX_PERCENT, WPX_POINT, WPX_TWIP, WPX_GENERIC };

class WPXProperty
{
public:
	virtual ~WPXProperty();
	virtual int getInt() const = 0;
	virtual double getDouble() const = 0;
	virtual WPXString getStr() const = 0;
	virtual WPXProperty *clone() const = 0;
};

class WPXPropertyFactory
{
public:
	static WPXProperty *newStringProp(const WPXString &str);
	static WPXProperty *newStringProp(const char *str);
	static WPXProperty *newIntProp(const int val);
	static WPXProperty *newBoolProp(const bool val);
	static WPXProperty *newDoubleProp(const double val);
	static WPXProperty *newInchProp(const double val);
	static WPXProperty *newPercentProp(const double val);
	static WPXProperty *newPointProp(const double val);
	static WPXProperty *newTwipProp(const double val);
};
#endif /* WPXPROPERTY_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
