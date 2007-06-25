/* libwpd
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
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
 * License along with this library; if not, write to the 
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA  02111-1301 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

#ifndef __WPXSTREAMIMPLEMENTATION_H__
#define __WPXSTREAMIMPLEMENTATION_H__

#include "WPXStream.h"

class WPXFileStreamPrivate;

class WPXFileStream: public WPXInputStream
{
public:
	explicit WPXFileStream(const char* filename);
	~WPXFileStream();
	
	const uint8_t *read(size_t numBytes, size_t &numBytesRead);
	long tell();
	int seek(long offset, WPX_SEEK_TYPE seekType);
	bool atEOS();

	bool isOLEStream();
	WPXInputStream *getDocumentOLEStream(const char * name);

private:
	WPXFileStreamPrivate* d;
	WPXFileStream(const WPXFileStream&); // copy is not allowed
	WPXFileStream& operator=(const WPXFileStream&); // assignment is not allowed
};

class WPXStringStreamPrivate;

class WPXStringStream: public WPXInputStream
{
public:
	WPXStringStream(const char *data, const unsigned int dataSize);
	~WPXStringStream();

	const uint8_t *read(size_t numBytes, size_t &numBytesRead);
	long tell();
	int seek(long offset, WPX_SEEK_TYPE seekType);
	bool atEOS();

	bool isOLEStream();
	WPXInputStream *getDocumentOLEStream(const char * name);

private:
	WPXStringStreamPrivate* d;
	WPXStringStream(const WPXStringStream&); // copy is not allowed
	WPXStringStream& operator=(const WPXStringStream&); // assignment is not allowed
};

#endif // __WPXSTREAMIMPLEMENTATION_H__
