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

#ifndef WPXBINARYDATA_H
#define WPXBINARYDATA_H

#include "WPXString.h"
#include <stdio.h>

class WPXBinaryDataImpl;
class WPXInputStream;

class WPXBinaryData
{
public:
	WPXBinaryData();
	WPXBinaryData(const WPXBinaryData &);
	WPXBinaryData(const unsigned char *buffer, const size_t bufferSize);
	WPXBinaryData(const WPXInputStream *input, const size_t inputSize);
	~WPXBinaryData();

	void append(const WPXBinaryData &data);
	void append(const unsigned char *buffer, const size_t bufferSize);
	void append(const unsigned char c);
	void clear();
	
	const size_t size() const;
	const unsigned char *getDataBuffer() const;
	const WPXString getBase64Data() const;
	const WPXInputStream *getDataStream() const;

	WPXBinaryData& operator=(const WPXBinaryData &);

private:
	WPXBinaryDataImpl *m_binaryDataImpl;
};
#endif
