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

#include "librevenge-api.h"

#include <librevenge-stream/librevenge-stream.h>

#include "RVNGString.h"

namespace librevenge
{

struct RVNGBinaryDataImpl;

class REVENGE_API RVNGBinaryData
{
public:
	RVNGBinaryData();
	RVNGBinaryData(const RVNGBinaryData &);
	RVNGBinaryData(const unsigned char *buffer, const unsigned long bufferSize);
	explicit RVNGBinaryData(const RVNGString &base64);
	explicit RVNGBinaryData(const char *base64);
	~RVNGBinaryData();

	void append(const RVNGBinaryData &data);
	void append(const unsigned char *buffer, const unsigned long bufferSize);
	void append(const unsigned char c);
	void appendBase64Data(const RVNGString &base64);
	void appendBase64Data(const char *base64);
	void clear();

	unsigned long size() const;
	bool empty() const;
	const unsigned char *getDataBuffer() const;
	const RVNGString getBase64Data() const;
	const RVNGInputStream *getDataStream() const;

	RVNGBinaryData &operator=(const RVNGBinaryData &);

private:
	RVNGBinaryDataImpl *m_binaryDataImpl;
};

}

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
