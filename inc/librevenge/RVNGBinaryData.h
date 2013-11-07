/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGBINARYDATA_H
#define RVNGBINARYDATA_H

#include <librevenge-stream/librevenge-stream.h>
#include "RVNGString.h"

namespace librevenge
{

struct RVNGBinaryDataImpl;

/** A representation of arbitrary binary data.

This class is optimized for copying, which means the copies of an
instance share the data (@seealso copy).

The typical usage pattern of this class is:
1. construction
2. (optional) modification, using @c append and @c clear
3. usage (treating it as immutable)
*/
class RVNGBinaryData
{
public:
	RVNGBinaryData();

	/** Creates a new (shallow) copy of @c other.

	  The copy is shallow, which means that the actual content is shared
	  with original. If you need a separately changeable copy, use @c
	  copy.

	  @seealso copy
	  */
	RVNGBinaryData(const RVNGBinaryData &other);
	RVNGBinaryData(const unsigned char *buffer, const unsigned long bufferSize);
	~RVNGBinaryData();

	/** Assigns a (shallow) copy of @c other to @c this.

	  The copy is shallow, which means that the actual content is shared
	  with original. If you need a separately changeable copy, use @c
	  copy.

	  @seealso copy
	  */
	RVNGBinaryData &operator=(const RVNGBinaryData &other);

	/** Creates a (deep) copy of @c this.

	  A copy created by this function does not share data with the
	  original and can thus be changed separately.

	  @seealso RVNGBinaryData(const RVNGBinaryData &)
	  */
	RVNGBinaryData copy() const;

	void append(const RVNGBinaryData &data);
	void append(const unsigned char *buffer, const unsigned long bufferSize);
	void append(const unsigned char c);
	void clear();

	unsigned long size() const;
	const unsigned char *getDataBuffer() const;
	const RVNGString getBase64Data() const;
	const RVNGInputStream *getDataStream() const;

private:
	RVNGBinaryDataImpl *m_binaryDataImpl;
};

}

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
