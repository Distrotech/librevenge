/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002-2005 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#ifndef RVNGDIRECTORYSTREAM_H_INCLUDED
#define RVNGDIRECTORYSTREAM_H_INCLUDED

#include "librevenge-stream-api.h"

#include "RVNGStream.h"

namespace librevenge
{

struct RVNGDirectoryStreamImpl;

class REVENGE_STREAM_API RVNGDirectoryStream : public RVNGInputStream
{
	// disable copying
	RVNGDirectoryStream(const RVNGDirectoryStream &);
	RVNGDirectoryStream &operator=(const RVNGDirectoryStream &);

public:
	explicit RVNGDirectoryStream(const char *path);
	virtual ~RVNGDirectoryStream();

	virtual bool isStructured();
	virtual unsigned subStreamCount();
	virtual const char *subStreamName(unsigned id);
	virtual bool existsSubStream(const char *name);
	virtual RVNGInputStream *getSubStreamByName(const char *name);
	virtual RVNGInputStream *getSubStreamById(unsigned id);

	virtual const unsigned char *read(unsigned long numBytes, unsigned long &numBytesRead);
	virtual int seek(long offset, RVNG_SEEK_TYPE seekType);
	virtual long tell();
	virtual bool isEnd();

private:
	RVNGDirectoryStreamImpl *m_impl;
};

}

#endif //  RVNGDIRECTORYSTREAM_H_INCLUDED

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
