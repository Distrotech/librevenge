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
 *
 * For further information visit http://librevenge.sourceforge.net
 */

#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H
#include <librevenge-stream/librevenge-stream.h>

class RVNGMemoryInputStream : public RVNGInputStream
{
public:
	RVNGMemoryInputStream(unsigned char *data, unsigned long size);
	virtual ~RVNGMemoryInputStream();
	virtual bool isStructured()
	{
		return false;
	}
	virtual unsigned subStreamCount()
	{
		return 0;
	}
	virtual const char *subStreamName(unsigned)
	{
		return 0;
	}
	virtual RVNGInputStream *getSubStreamByName(const char *)
	{
		return 0;
	}
	virtual RVNGInputStream *getSubStreamById(unsigned)
	{
		return 0;
	}
	virtual const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead);
	virtual int seek(long offset, RVNG_SEEK_TYPE seekType);
	virtual long tell();
	virtual bool isEnd();
	virtual unsigned long getSize() const
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

#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
