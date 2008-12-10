/* libwpd
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WPXBinaryData.h"
#include "WPXMemoryStream.h"
#include "WPXStream.h"
#include "libwpd_internal.h"

#include <vector>
#include <stdarg.h>
#include <stdio.h>

class WPXBinaryDataImpl
{
public:
	WPXBinaryDataImpl() : m_buf(), m_stream(NULL) {}
	~WPXBinaryDataImpl() { if (m_stream) delete m_stream; }
	std::vector<unsigned char> m_buf;
	WPXMemoryInputStream *m_stream;
};

WPXBinaryData::~WPXBinaryData()
{
	delete m_binaryDataImpl;
}

WPXBinaryData::WPXBinaryData() :
	m_binaryDataImpl(new WPXBinaryDataImpl)
{
}

WPXBinaryData::WPXBinaryData(const WPXBinaryData &data) :
	m_binaryDataImpl(new WPXBinaryDataImpl)
{
	m_binaryDataImpl->m_buf = data.m_binaryDataImpl->m_buf;
}

WPXBinaryData::WPXBinaryData(const unsigned char *buffer, const unsigned long bufferSize) :
	m_binaryDataImpl(new WPXBinaryDataImpl)
{
	m_binaryDataImpl->m_buf = std::vector<unsigned char> (bufferSize);
	for (unsigned long i = 0; i < bufferSize; i++)
		m_binaryDataImpl->m_buf[i] = buffer[i];
}

void WPXBinaryData::append(const WPXBinaryData &data)
{
	unsigned long previousSize = m_binaryDataImpl->m_buf.size();
	m_binaryDataImpl->m_buf.reserve(previousSize + data.m_binaryDataImpl->m_buf.size());
	for (unsigned long i = 0; i < data.m_binaryDataImpl->m_buf.size(); i++)
		m_binaryDataImpl->m_buf.push_back(data.m_binaryDataImpl->m_buf[i]);
}

void WPXBinaryData::append(const unsigned char *buffer, const unsigned long bufferSize )
{
	unsigned long previousSize = m_binaryDataImpl->m_buf.size();
	m_binaryDataImpl->m_buf.reserve(previousSize + bufferSize);
	for (unsigned long i = 0; i < bufferSize; i++)
		m_binaryDataImpl->m_buf.push_back(buffer[i]);
}

void WPXBinaryData::append(const unsigned char c)
{
	m_binaryDataImpl->m_buf.push_back(c);
}

void WPXBinaryData::clear()
{
	m_binaryDataImpl->m_buf = std::vector<unsigned char>();
}

unsigned long WPXBinaryData::size() const
{ 
	return (unsigned long)m_binaryDataImpl->m_buf.size(); 
}

WPXBinaryData& WPXBinaryData::operator=(const WPXBinaryData &dataBuf)
{
	m_binaryDataImpl->m_buf = dataBuf.m_binaryDataImpl->m_buf;
	return *this;
}

const unsigned char * WPXBinaryData::getDataBuffer() const
{
	return &(m_binaryDataImpl->m_buf[0]);
}

const WPXString WPXBinaryData::getBase64Data() const
{
	static const char* base64Chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char tempCharsToEncode[3];
	const long len = size();
	long i = 0; unsigned j = 0; long modifiedLen;
	if (len % 3)
		modifiedLen = 3 * ((long)(len / 3) + 1);
	else
		modifiedLen = len;
	
	bool shouldIexit = false;
	WPXString base64;	
	for (; i < modifiedLen; i++)
	{
		if (i < len)
			tempCharsToEncode[j++] = m_binaryDataImpl->m_buf[i];
		else
		{
			tempCharsToEncode[j++] = '\0';
			shouldIexit = true;
		}

		if (shouldIexit)
		{
			if (j == 3)
			{
				base64.append(base64Chars[(tempCharsToEncode[0] & 0xfc) >> 2]);
				base64.append(base64Chars[((tempCharsToEncode[0] & 0x03) << 4) | ((tempCharsToEncode[1] & 0xf0) >> 4)]);
				base64.append(base64Chars[((tempCharsToEncode[1] & 0x0f) << 2) | ((tempCharsToEncode[2] & 0xc0) >> 6)]);
				base64.append('=');
				break;
			}
			if (j == 2)
			{
				base64.append(base64Chars[(tempCharsToEncode[0] & 0xfc) >> 2]);
				base64.append(base64Chars[((tempCharsToEncode[0] & 0x03) << 4) | ((tempCharsToEncode[1] & 0xf0) >> 4)]);
				base64.append('='); base64.append('=');
				break;
			}
		}
		else if (j == 3)
		{
			base64.append(base64Chars[(tempCharsToEncode[0] & 0xfc) >> 2]);
			base64.append(base64Chars[((tempCharsToEncode[0] & 0x03) << 4) | ((tempCharsToEncode[1] & 0xf0) >> 4)]);
			base64.append(base64Chars[((tempCharsToEncode[1] & 0x0f) << 2) | ((tempCharsToEncode[2] & 0xc0) >> 6)]);
			base64.append(base64Chars[tempCharsToEncode[2] & 0x3f]);
			j = 0;
		}
	}
	return base64;
}

const WPXInputStream* WPXBinaryData::getDataStream() const
{
	if (m_binaryDataImpl->m_stream)
		delete (m_binaryDataImpl->m_stream);
	return ((m_binaryDataImpl->m_stream) = new WPXMemoryInputStream(&(m_binaryDataImpl->m_buf[0]), m_binaryDataImpl->m_buf.size()));
}
