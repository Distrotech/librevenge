/* libwpd
 * Copyright (C) 2004 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2005 Net Integration Technologies (http://www.net-itech.com)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXString.h"
#include "libwpd_internal.h"

#include <string>
#include <stdarg.h>
#include <stdio.h>

#define FIRST_BUF_SIZE 128
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

static int g_static_utf8_strlen (const char *p);
static int g_static_unichar_to_utf8 (uint32_t c,  char *outbuf);

static const int8_t g_static_utf8_skip_data[256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

#define UTF8_LENGTH(Char)              \
  ((Char) < 0x80 ? 1 :                 \
   ((Char) < 0x800 ? 2 :               \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :          \
      ((Char) < 0x4000000 ? 5 : 6)))))
#define g_static_utf8_next_char(p) (char *)((p) + g_static_utf8_skip_data[*((uint8_t *)p)])

class WPXStringImpl
{
public:
	WPXStringImpl() : m_buf() {}
	std::string m_buf;
};

WPXString::~WPXString()
{
	delete m_stringImpl;
}

WPXString::WPXString() :
	m_stringImpl(new WPXStringImpl)
{
	m_stringImpl->m_buf.reserve(FIRST_BUF_SIZE);
}

WPXString::WPXString(const WPXString &stringBuf, bool escapeXML) :
	m_stringImpl(new WPXStringImpl) 
{
	if (escapeXML)
	{
		int tmpLen = stringBuf.m_stringImpl->m_buf.length();
		m_stringImpl->m_buf.reserve(2*tmpLen);
		const char *p = stringBuf.cstr();
		const char *end = p + tmpLen; 
		while (p != end)
		{
			const char *next = g_static_utf8_next_char(p);

			switch (*p)
			{
			case '&':
				append("&amp;");
				break;
			case '<':
				append("&lt;");
				break;
			case '>':
				append("&gt;");
				break;
			case '\'':
				append("&apos;");
				break;
			case '"':
				append("&quot;");
				break;
			default:
				while (p != next) 
				{		
					append((*p));
					p++;
				}
				break;
			}

			p = next;
		}
	}
	else
		m_stringImpl->m_buf = stringBuf.m_stringImpl->m_buf;
}

WPXString::WPXString(const char *str) :
	m_stringImpl(new WPXStringImpl)
{
	m_stringImpl->m_buf = std::string(str);
}

const char * WPXString::cstr() const
{
    return m_stringImpl->m_buf.c_str();
}

void WPXString::sprintf(const char *format, ...)
{
	va_list args;

	int bufsize = FIRST_BUF_SIZE;
	char firstBuffer[FIRST_BUF_SIZE];
	char * buf = firstBuffer;

	while(true)
	{
			va_start(args, format);
			int outsize = vsnprintf(buf, bufsize, format, args);
			va_end(args);

			if ((outsize == -1) || (outsize == bufsize) || (outsize == bufsize - 1))
				bufsize = bufsize * 2;
			else if (outsize > bufsize)
				bufsize = outsize + 2;
			else
				break;

			if (buf != firstBuffer)
			{
				delete [] buf;
				buf = new char[bufsize];
			}
	}

	clear();
	append(buf);
	if (buf != firstBuffer)
		delete [] buf;
}

void WPXString::append(const WPXString &s)
{
	m_stringImpl->m_buf.append(s.m_stringImpl->m_buf);
}

void WPXString::append(const char *s)
{
	m_stringImpl->m_buf.append(s);
}

void WPXString::append(const char c)
{
	m_stringImpl->m_buf.append(1, c);
}

void WPXString::clear()
{
	m_stringImpl->m_buf.erase(m_stringImpl->m_buf.begin(), m_stringImpl->m_buf.end());
}

int WPXString::len() const
{ 
	return g_static_utf8_strlen(cstr()); 
}

WPXString& WPXString::operator=(const WPXString &stringBuf)
{
	m_stringImpl->m_buf = stringBuf.m_stringImpl->m_buf;
	return *this;
}

WPXString& WPXString::operator=(const char *s)
{
	m_stringImpl->m_buf = std::string(s);
	return *this;
}

bool WPXString::operator==(const char *str) const
{
	return (m_stringImpl->m_buf == str);
}

bool WPXString::operator==(const WPXString &str) const
{
	return (m_stringImpl->m_buf == str.m_stringImpl->m_buf);
}

bool WPXString::operator!() const 
{
	return (m_stringImpl->m_buf.length() == 0);
}

WPXString::Iter::Iter(const WPXString &str) :
	m_stringImpl(new WPXStringImpl),
	m_pos(0),
	m_curChar(0)
{
	m_stringImpl->m_buf = str.cstr();
}

WPXString::Iter::~Iter()
{
	delete [] m_curChar;
	delete m_stringImpl;
}

void WPXString::Iter::rewind()
{
    m_pos = (-1);
}

bool WPXString::Iter::next()
{
	int len = m_stringImpl->m_buf.length();

	if (m_pos == (-1)) 
		m_pos++;
	else if (m_pos < len)
	{
		m_pos+=(int32_t) (g_static_utf8_next_char(&(m_stringImpl->m_buf.c_str()[m_pos])) - 
				  &(m_stringImpl->m_buf.c_str()[m_pos]));
	}

	if (m_pos < len)
		return true;
	return false;
}

bool WPXString::Iter::last()
{
	if (m_pos >= g_static_utf8_strlen(m_stringImpl->m_buf.c_str()))
		return true;
	return false;
}

const char * WPXString::Iter::operator()() const
{ 
	if (m_pos == (-1)) return 0; 

	if (m_curChar) delete [] m_curChar; m_curChar = 0;
	int32_t charLength =(int32_t) (g_static_utf8_next_char(&(m_stringImpl->m_buf.c_str()[m_pos])) - 
				       &(m_stringImpl->m_buf.c_str()[m_pos]));
	m_curChar = new char[charLength+1];
	for (int i=0; i<charLength; i++)
		m_curChar[i] = m_stringImpl->m_buf[m_pos+i];
	m_curChar[charLength]='\0';

	return m_curChar;
}

/* Various utf8/ucs4 routines, some stolen from glib */

void appendUCS4(WPXString &str, uint32_t ucs4)
{
	int charLength = g_static_unichar_to_utf8(ucs4, 0);
	char *utf8 = new char[charLength+1];
	utf8[charLength] = '\0';
	g_static_unichar_to_utf8(ucs4, utf8);
	str.append(utf8);

	delete[] utf8;
}

/*
 * g_static_unichar_to_utf8:
 *
 * stolen from glib 2.4.1
 *
 * @c: a ISO10646 character code
 * @outbuf: output buffer, must have at least 6 bytes of space.
 *       If %0, the length will be computed and returned
 *       and nothing will be written to @outbuf.
 * 
 * Converts a single character to UTF-8.
 * 
 * Return value: number of bytes written
 */
int
g_static_unichar_to_utf8 (uint32_t c,
			  char   *outbuf)
{
	uint8_t len = 1;    
	uint8_t first = 0;
    
	if (c < 0x80)
	{
		first = 0;
		len = 1;
	}
	else if (c < 0x800)
	{
		first = 0xc0;
		len = 2;
	}
	else if (c < 0x10000)
	{
		first = 0xe0;
		len = 3;
	}
	else if (c < 0x200000)
	{
		first = 0xf0;
		len = 4;
	}
	else if (c < 0x4000000)
	{
		first = 0xf8;
		len = 5;
	}
	else
	{
		first = 0xfc;
		len = 6;
	}
    
	if (outbuf)
	{
		for (uint8_t i = (uint8_t)(len - 1); i > 0; --i)
		{
			outbuf[i] = (char)((c & 0x3f) | 0x80);
			c >>= 6;
		}
		outbuf[0] = (char)(c | first);
	}
    
	return len;
}

/*
 * g_static_utf8_strlen:
 * @p: pointer to the start of a UTF-8 encoded string.
 * 
 * Returns the length of the string in characters.
 *
 * Return value: the length of the string in characters
 */
int
g_static_utf8_strlen (const char *p)
{
	long len = 0;
	if (!p)
		return 0;

	while (*p)
	{
		p = g_static_utf8_next_char (p);
		++len;
	}

	return len;
}

