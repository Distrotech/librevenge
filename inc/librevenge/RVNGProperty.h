/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 */

#ifndef RVNGPROPERTY_H
#define RVNGPROPERTY_H

#include "librevenge-api.h"

#include "RVNGString.h"

namespace librevenge
{

enum RVNGUnit { RVNG_INCH, RVNG_PERCENT, RVNG_POINT, RVNG_TWIP, RVNG_GENERIC, RVNG_UNIT_ERROR };

class REVENGE_API RVNGProperty
{
public:
	virtual ~RVNGProperty();
	virtual int getInt() const = 0;
	virtual double getDouble() const = 0;
	/** returns the property unit when possible. If not, returns RVNG_UNIT_ERROR */
	virtual RVNGUnit getUnit() const = 0;
	virtual RVNGString getStr() const = 0;
	virtual RVNGProperty *clone() const = 0;
};

class REVENGE_API RVNGPropertyFactory
{
public:
	static RVNGProperty *newStringProp(const RVNGString &str);
	static RVNGProperty *newStringProp(const char *str);
	static RVNGProperty *newBinaryDataProp(const RVNGBinaryData &data);
	static RVNGProperty *newBinaryDataProp(const unsigned char *buffer,
	                                       const unsigned long bufferSize);
	static RVNGProperty *newIntProp(const int val);
	static RVNGProperty *newBoolProp(const bool val);
	static RVNGProperty *newDoubleProp(const double val);
	static RVNGProperty *newInchProp(const double val);
	static RVNGProperty *newPercentProp(const double val);
	static RVNGProperty *newPointProp(const double val);
	static RVNGProperty *newTwipProp(const double val);
};

}

#endif /* RVNGPROPERTY_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
