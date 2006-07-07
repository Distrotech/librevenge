/* libwpd
 * Copyright (C) 2004-2005 William Lachance (william.lachance@sympatico.ca)
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

#ifndef WPXSTREAM_H
#define WPXSTREAM_H
#include <stdio.h>
#include "libwpd_types.h"

enum WPX_SEEK_TYPE
{
	WPX_SEEK_CUR,
	WPX_SEEK_SET
};

class WPXInputStream
{
public:
	WPXInputStream(bool supportsOLE) :
		m_supportsOLE(supportsOLE) {}
	virtual ~WPXInputStream() {}

	virtual bool isOLEStream() = 0;
	virtual WPXInputStream * getDocumentOLEStream() = 0;

	const virtual uint8_t *read(size_t numBytes, size_t &numBytesRead) = 0;
	virtual int seek(long offset, WPX_SEEK_TYPE seekType) = 0;
	virtual long tell() = 0;
	virtual bool atEOS() = 0;

private:
	bool m_supportsOLE;
};
#endif
