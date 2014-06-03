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

#include <sys/stat.h>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

namespace
{

static void sanitizePath(std::string &str)
{
	boost::algorithm::replace_all(str, "\\", "/");
	boost::algorithm::trim_right_if(str, boost::is_any_of("/ "));
}

static std::string composePath(const std::vector<std::string> &splitPath, std::vector<std::string>::size_type elementCount)
{
	std::string path;
	for (std::vector<std::string>::size_type i=0; i < splitPath.size() && i < elementCount; ++i)
	{
		path.append(splitPath[i]);
		path.append("/");
	}
	sanitizePath(path);
	return path;
}

static bool isReg(const char *const path)
{
	struct stat statBuf;
	if (stat(path, &statBuf))
		return false;
	if (S_ISREG(statBuf.st_mode))
		return true;
#ifndef _WIN32
	else if (S_ISLNK(statBuf.st_mode))
	{
		if (lstat(path, &statBuf))
			return false;
		if (S_ISREG(statBuf.st_mode))
			return true;
	}
#endif
	return false;
}

static bool isDir(const char *const path)
{
	struct stat statBuf;
	if (stat(path, &statBuf))
		return false;
	if (S_ISDIR(statBuf.st_mode))
		return true;
#ifndef _WIN32
	else if (S_ISLNK(statBuf.st_mode))
	{
		if (lstat(path, &statBuf))
			return false;
		if (S_ISDIR(statBuf.st_mode))
			return true;
	}
#endif
	return false;
}

}

struct RVNGDirectoryStreamImpl
{
	std::vector<std::string> m_splitPath;

	RVNGDirectoryStreamImpl(const char *path);
};

RVNGDirectoryStreamImpl::RVNGDirectoryStreamImpl(const char *const path)
	: m_splitPath()
{
	std::string pathName(path);
	sanitizePath(pathName);
	boost::algorithm::split(m_splitPath, pathName, boost::is_any_of("/"));
}

RVNGDirectoryStream::RVNGDirectoryStream(const char *const path)
	: m_impl(0)
{
	if (isDir(path))
		m_impl = new RVNGDirectoryStreamImpl(path);
}

RVNGDirectoryStream::~RVNGDirectoryStream()
{
	delete m_impl;
}

RVNGDirectoryStream *RVNGDirectoryStream::createForParent(const char *const path)
{
	std::string parent(path);
	sanitizePath(parent);
	std::vector<std::string> splitParent;
	boost::algorithm::split(splitParent, parent, boost::is_any_of("/"));
	parent = composePath(splitParent, splitParent.size() ? splitParent.size() - 1 : 0);

	RVNGDirectoryStream *strm = new RVNGDirectoryStream(parent.c_str());
	if (strm->isStructured()) // only if parent is a dir
		return strm;

	delete strm;
	return 0;
}

bool RVNGDirectoryStream::isDirectory(const char *const path)
{
	return isDir(path);
}

bool RVNGDirectoryStream::isStructured()
{
	if (!m_impl)
		return false;
	return true;
}

unsigned RVNGDirectoryStream::subStreamCount()
{
	if (!m_impl)
		return 0;
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
	if (!m_impl)
		return false;
	std::string path(name);
	sanitizePath(path);
	std::vector<std::string> splitPath;
	boost::algorithm::split(splitPath, path, boost::is_any_of("/"));
	splitPath.insert(splitPath.begin(), m_impl->m_splitPath.begin(), m_impl->m_splitPath.end());
	path = composePath(splitPath, splitPath.size());
	return isReg(path.c_str());

}

RVNGInputStream *RVNGDirectoryStream::getSubStreamByName(const char *const name)
{
	if (!m_impl)
		return 0;
	std::string path(name);
	sanitizePath(path);
	std::vector<std::string> splitPath;
	boost::algorithm::split(splitPath, path, boost::is_any_of("/"));
	splitPath.insert(splitPath.begin(), m_impl->m_splitPath.begin(), m_impl->m_splitPath.end());
	path = composePath(splitPath, splitPath.size());

	if (isReg(path.c_str()))
		return new RVNGFileStream(path.c_str());
	else if (isDir(path.c_str()))
		return new RVNGDirectoryStream(path.c_str());

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
	if (!m_impl)
		return -1L;
	return 0;
}

bool RVNGDirectoryStream::isEnd()
{
	return true;
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
