/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2004-2005 William Lachance (wrlach@gmail.com)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGMEMORYSTREAM_H
#define RVNGMEMORYSTREAM_H

#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

class RVNGMemoryInputStream : public RVNGInputStream
{
public:
	RVNGMemoryInputStream(unsigned char *data, unsigned long size);
	~RVNGMemoryInputStream();
	bool isStructured()
	{
		return false;
	}
	unsigned subStreamCount()
	{
		return 0;
	}
	const char *subStreamName(unsigned)
	{
		return 0;
	}
	bool existsSubStream(const char *)
	{
		return false;
	}
	RVNGInputStream *getSubStreamByName(const char *)
	{
		return 0;
	}
	RVNGInputStream *getSubStreamById(unsigned)
	{
		return 0;
	}
	const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead);
	int seek(long offset, RVNG_SEEK_TYPE seekType);
	long tell();
	bool isEnd();
	unsigned long getSize() const
	{
		return m_size;
	}

private:
	long m_offset;
	unsigned long m_size;
	unsigned char *m_data;
	RVNGMemoryInputStream(const RVNGMemoryInputStream &);
	RVNGMemoryInputStream &operator=(const RVNGMemoryInputStream &);
};

}

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
