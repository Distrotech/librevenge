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
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include "librevenge_internal.h"

#include <cstring>
#include <string>
#include <stdarg.h>
#include <stdio.h>

#define FIRST_BUF_SIZE 128
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

namespace librevenge
{

namespace
{

static const unsigned char librvng_utf8_skip_data[256] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

#define librvng_utf8_next_char(p) (char const *)((p) + librvng_utf8_skip_data[*((unsigned char const *)p)])

static int librvng_utf8_strlen(const char *p, const char *end)
{
	if (!p)
		return 0;

	int len = 0;
	while (p < end && *p)
	{
		p = librvng_utf8_next_char(p);
		++len;
	}

	return len;
}

} // anonymous namespace

class RVNGStringImpl
{
public:
	RVNGStringImpl() : m_buf() {}
	bool empty() const
	{
		return m_buf.empty();
	}
	int len() const
	{
		if (empty()) return 0;
		return librvng_utf8_strlen(m_buf.c_str(), m_buf.c_str()+m_buf.length());
	}
	unsigned long size() const
	{
		return (unsigned long)m_buf.size();
	}
	void appendEscapedXML(const char *s, const unsigned long sz);
	std::string m_buf;
};

void RVNGStringImpl::appendEscapedXML(const char *s, const unsigned long sz)
{
	m_buf.reserve(m_buf.size() + 2 * sz);
	const char *p = s;
	const char *const end = p + sz;
	while (p != end)
	{
		const char *const next = librvng_utf8_next_char(p);
		if (next > end)
		{
			RVNG_DEBUG_MSG(("RVNGStringImpl::appendEscapedXML: oops, we have a problem\n"));
			break;
		}
		switch (*p)
		{
		case '&':
			m_buf.append("&amp;");
			break;
		case '<':
			m_buf.append("&lt;");
			break;
		case '>':
			m_buf.append("&gt;");
			break;
		case '\'':
			m_buf.append("&apos;");
			break;
		case '"':
			m_buf.append("&quot;");
			break;
		default:
			while (p != next)
			{
				m_buf.append(1, *p);
				++p;
			}
			break;
		}

		p = next;
	}
}

RVNGString::~RVNGString()
{
	delete m_stringImpl;
}

RVNGString::RVNGString() :
	m_stringImpl(new RVNGStringImpl)
{
	m_stringImpl->m_buf.reserve(FIRST_BUF_SIZE);
}

RVNGString::RVNGString(const RVNGString &other) :
	m_stringImpl(new RVNGStringImpl)
{
	m_stringImpl->m_buf = other.m_stringImpl->m_buf;
}

RVNGString::RVNGString(const char *str) :
	m_stringImpl(new RVNGStringImpl)
{
	if (str)
		m_stringImpl->m_buf = str;
}

RVNGString RVNGString::escapeXML(const RVNGString &s)
{
	RVNGString escaped;
	escaped.appendEscapedXML(s);
	return escaped;
}

RVNGString RVNGString::escapeXML(const char *const s)
{
	RVNGString escaped;
	escaped.appendEscapedXML(s);
	return escaped;
}

const char *RVNGString::cstr() const
{
	return m_stringImpl->m_buf.c_str();
}

void RVNGString::sprintf(const char *format, ...)
{
	va_list args;

	int bufsize = FIRST_BUF_SIZE;
	char firstBuffer[FIRST_BUF_SIZE];
	char *buf = firstBuffer;

	while (true)
	{
		va_start(args, format);
		int outsize = vsnprintf(buf, size_t(bufsize), format, args);
		va_end(args);

		if ((outsize == -1) || (outsize == bufsize) || (outsize == bufsize - 1))
			bufsize = bufsize * 2;
		else if (outsize > bufsize)
			bufsize = outsize + 2;
		else
			break;

		if (buf != firstBuffer)
			delete [] buf;
		buf = new char[bufsize];
	}

	clear();
	append(buf);
	if (buf != firstBuffer)
		delete [] buf;
}

void RVNGString::append(const RVNGString &s)
{
	m_stringImpl->m_buf.append(s.m_stringImpl->m_buf);
}

void RVNGString::append(const char *s)
{
	if (s)
		m_stringImpl->m_buf.append(s);
}

void RVNGString::append(const char c)
{
	m_stringImpl->m_buf.append(1, c);
}

void RVNGString::appendEscapedXML(const RVNGString &s)
{
	m_stringImpl->appendEscapedXML(s.cstr(), s.size());
}

void RVNGString::appendEscapedXML(const char *const s)
{
	m_stringImpl->appendEscapedXML(s, std::strlen(s));
}

void RVNGString::clear()
{
	m_stringImpl->m_buf.erase(m_stringImpl->m_buf.begin(), m_stringImpl->m_buf.end());
}

int RVNGString::len() const
{
	return m_stringImpl->len();
}

unsigned long RVNGString::size() const
{
	return m_stringImpl->size();
}

bool RVNGString::empty() const
{
	return m_stringImpl->empty();
}

RVNGString &RVNGString::operator=(const RVNGString &stringBuf)
{
	m_stringImpl->m_buf = stringBuf.m_stringImpl->m_buf;
	return *this;
}

RVNGString &RVNGString::operator=(const char *s)
{
	if (s)
		m_stringImpl->m_buf = s;
	else
		clear();
	return *this;
}

bool RVNGString::operator==(const char *str) const
{
	if (!str)
		return false;
	return (m_stringImpl->m_buf == str);
}

bool RVNGString::operator==(const RVNGString &str) const
{
	return (m_stringImpl->m_buf == str.m_stringImpl->m_buf);
}

bool RVNGString::operator<(const char *str) const
{
	if (!str)
		return false;
	return (m_stringImpl->m_buf < str);
}

bool RVNGString::operator<(const RVNGString &str) const
{
	return (m_stringImpl->m_buf < str.m_stringImpl->m_buf);
}

RVNGString::Iter::Iter(const RVNGString &str) :
	m_stringImpl(new RVNGStringImpl),
	m_pos(0),
	m_curChar(0)
{
	m_stringImpl->m_buf = str.cstr();
}

RVNGString::Iter::~Iter()
{
	delete [] m_curChar;
	delete m_stringImpl;
}

void RVNGString::Iter::rewind()
{
	m_pos = (-1);
}

bool RVNGString::Iter::next()
{
	int len = (int) m_stringImpl->m_buf.length();

	if (m_pos == (-1))
		m_pos++;
	else if (m_pos < len)
	{
		m_pos+=(int)(librvng_utf8_next_char(&(m_stringImpl->m_buf.c_str()[m_pos])) -
		             &(m_stringImpl->m_buf.c_str()[m_pos]));
	}

	if (m_pos < len)
		return true;
	return false;
}

bool RVNGString::Iter::last()
{
	if (m_pos >= m_stringImpl->len())
		return true;
	return false;
}

const char *RVNGString::Iter::operator()() const
{
	if (m_pos == (-1)) return 0;

	if (m_curChar) delete [] m_curChar;
	m_curChar = 0;
	int charLength =(int)(librvng_utf8_next_char(&(m_stringImpl->m_buf.c_str()[m_pos])) -
	                      &(m_stringImpl->m_buf.c_str()[m_pos]));
	m_curChar = new char[charLength+1];
	for (int i=0; i<charLength; i++)
		m_curChar[i] = m_stringImpl->m_buf[size_t(m_pos+i)];
	m_curChar[charLength]='\0';

	return m_curChar;
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
