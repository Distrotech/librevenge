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

/** A representation of arbitrary binary data.
*/
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

	/** Remove current content.
	*/
	void clear();

	/** Get the size of the content in bytes.

	@returns the size of the content.
	*/
	unsigned long size() const;

	/** Test if the content is empty.

	@returns @c true if the content is emtpy, @c false otherwise.
	*/
	bool empty() const;

	/** Get the data buffer.

	The buffer is managed internally, so it shall not be deleted.
	If this object is empty, 0 is returned.

	@returns the data buffer or 0 if emtpy
	*/
	const unsigned char *getDataBuffer() const;

	/** Get the content encoded as base64.

	@returns a string containing a base64 representation of the content
	*/
	const RVNGString getBase64Data() const;

	/** Create a stream for the data.

	The stream is managed internally, so it shall not be deleted.
	If this object is empty, 0 is returned.

	@warning The stream is destroyed (and the pointer thus becomes
	invalid) when this object is either deleted or changed.

	@returns a stream or 0
	*/
	RVNGInputStream *getDataStream() const;

	/** Assign the content of another @c RVNGBinaryData object.

	@returns @c this object
	*/
	RVNGBinaryData &operator=(const RVNGBinaryData &);

private:
	RVNGBinaryDataImpl *m_binaryDataImpl;
};

}

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
