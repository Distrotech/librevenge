/* libwpd
 * Copyright (C) 2004-2005 William Lachance (wrlach@gmail.com)
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

#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H
#include "WPXStream.h"

class WPXMemoryInputStream : public WPXInputStream
{
public:
	WPXMemoryInputStream(uint8_t *data, size_t size);
	virtual ~WPXMemoryInputStream();

	virtual bool isOLEStream() { return false; }
	virtual WPXInputStream * getDocumentOLEStream() { return 0; }
	
	const virtual uint8_t *read(size_t numBytes, size_t &numBytesRead);
	virtual int seek(long offset, WPX_SEEK_TYPE seekType);
	virtual long tell();
	virtual bool atEOS();

private:
	long m_offset;
	size_t m_size;
	uint8_t *m_data;
	uint8_t *m_tmpBuf;
	WPXMemoryInputStream(const WPXMemoryInputStream&);
	WPXMemoryInputStream& operator=(const WPXMemoryInputStream&);
};

#endif
