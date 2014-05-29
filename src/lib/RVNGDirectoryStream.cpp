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

#ifndef BOOST_SYSTEM_NO_DEPRECATED
#define BOOST_SYSTEM_NO_DEPRECATED 1
#endif
#include <boost/filesystem.hpp>

#include <librevenge-stream/librevenge-stream.h>

namespace fs = boost::filesystem;

namespace librevenge
{

namespace
{

struct NotADirectoryException
{
};

}

struct RVNGDirectoryStreamImpl
{
	fs::path path;

	RVNGDirectoryStreamImpl(const char *path_);
};

RVNGDirectoryStreamImpl::RVNGDirectoryStreamImpl(const char *const path_)
	: path(path_)
{
}

RVNGDirectoryStream::RVNGDirectoryStream(const char *const path)
	: m_impl(new RVNGDirectoryStreamImpl(path))
{
	if (!is_directory(m_impl->path))
		throw NotADirectoryException();
}

RVNGDirectoryStream::~RVNGDirectoryStream()
{
	delete m_impl;
}

bool RVNGDirectoryStream::isStructured()
{
	return true;
}

unsigned RVNGDirectoryStream::subStreamCount()
{
	// TODO: implement me
	return 1;
}

const char *RVNGDirectoryStream::subStreamName(unsigned id)
{
	// TODO: implement me
	(void) id;
	return 0;
}

bool RVNGDirectoryStream::existsSubStream(const char *name)
{
	const fs::path path = m_impl->path / name;

	if (is_regular_file(path))
		return true;
	return false;
}

RVNGInputStream *RVNGDirectoryStream::getSubStreamByName(const char *const name)
{
	const fs::path path = m_impl->path / name;

	if (is_regular_file(path))
		return new RVNGFileStream(path.string().c_str());
	else if (is_directory(path))
		return new RVNGDirectoryStream(path.string().c_str());

	return 0;
}

RVNGInputStream *RVNGDirectoryStream::getSubStreamById(unsigned id)
{
	// TODO: implement me
	(void) id;
	return 0;
}

const unsigned char *RVNGDirectoryStream::read(const unsigned long, unsigned long &numBytesRead)
{
	numBytesRead = 0;
	return 0;
}

int RVNGDirectoryStream::seek(const long, const RVNG_SEEK_TYPE)
{
	return -1;
}

long RVNGDirectoryStream::tell()
{
	return 0;
}

bool RVNGDirectoryStream::isEnd()
{
	return true;
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
