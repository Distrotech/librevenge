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

#include "WPXStreamImplementation.h"
#include "WPXOLEStream.h"
#include "libwpd.h"

#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace libwpd;

class WPXFileStreamPrivate
{
public:
	WPXFileStreamPrivate();
	~WPXFileStreamPrivate();
	std::fstream file;
	std::stringstream buffer;
	unsigned long streamSize;
	uint8_t *buf;
};

class WPXStringStreamPrivate
{
public:
	WPXStringStreamPrivate(const std::string str);
	~WPXStringStreamPrivate();
	std::stringstream buffer;
	unsigned long streamSize;
	uint8_t *buf;
};

WPXFileStreamPrivate::WPXFileStreamPrivate() :
	file(),
	buffer(std::ios::binary | std::ios::in | std::ios::out),
	streamSize(0),
	buf(0)
{	
}

WPXFileStreamPrivate::~WPXFileStreamPrivate()
{
	if (buf)
		delete [] buf;
}

WPXStringStreamPrivate::WPXStringStreamPrivate(const std::string str) :
	buffer(str, std::ios::binary | std::ios::in),
	streamSize(0),
	buf(0)
{
}

WPXStringStreamPrivate::~WPXStringStreamPrivate()
{
	if (buf)
		delete [] buf;
}

WPXFileStream::WPXFileStream(const char* filename) :
	WPXInputStream(true)
{
	d = new WPXFileStreamPrivate;
	
	d->file.open( filename, std::ios::binary | std::ios::in );
	d->file.seekg( 0, std::ios::end );
	d->streamSize = (d->file.good() ? (unsigned long)d->file.tellg() : (unsigned long)-1L);
	if (d->streamSize == (unsigned long)-1) // tellg() returned ERROR
		d->streamSize = 0;
	 // preventing possible unsigned/signed issues later by truncating the file
	if (d->streamSize > (std::numeric_limits<unsigned long>::max)() / 2)
		d->streamSize = (std::numeric_limits<unsigned long>::max)() / 2;
	d->file.seekg( 0, std::ios::beg );
}

WPXFileStream::~WPXFileStream()
{
	delete d;
}

const uint8_t *WPXFileStream::read(size_t numBytes, size_t &numBytesRead)
{
	numBytesRead = 0;
	
	if (numBytes < 0 || atEOS() || numBytes > (std::numeric_limits<unsigned long>::max)()/2)
		return 0;

	unsigned long curpos = d->file.tellg();
	if (curpos == (unsigned long)-1)  // tellg() returned ERROR
		return 0;

	if ( (curpos + numBytes < curpos) /*overflow*/ ||
		(curpos + numBytes > d->streamSize) ) /*reading more than available*/
	{
		numBytes = d->streamSize - curpos;
	}

	if (d->buf)
		delete [] d->buf;
	d->buf = new uint8_t[numBytes];

	if(d->file.good())
	{
		d->file.read((char *)(d->buf), numBytes); 
		numBytesRead = (long)d->file.tellg() - curpos;
	}
	
	return d->buf;
}

long WPXFileStream::tell()
{
	return d->file.good() ? (long)d->file.tellg() : -1L;
}

int WPXFileStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_SET)
	{
		if (offset < 0)
			offset = 0;
		if (offset > (long)d->streamSize)
			offset = (long)d->streamSize;
	}

	if (seekType == WPX_SEEK_CUR)
	{
		if (tell() + offset < 0)
			offset = -tell();
		if (tell() + offset > (long)d->streamSize)
			offset = d->streamSize - tell();
	}

	if(d->file.good())
	{
		d->file.seekg(offset, ((seekType == WPX_SEEK_SET) ? std::ios::beg : std::ios::cur));
		return (int) ((long)d->file.tellg() == -1) ;
	}
	else
		return -1;
}

bool WPXFileStream::atEOS()
{
	return (d->file.tellg() >= (long)d->streamSize);
}

bool WPXFileStream::isOLEStream()
{
	if (d->buffer.str().empty())
		d->buffer << d->file.rdbuf();
	Storage tmpStorage( d->buffer );
	if (tmpStorage.isOLEStream())
	{
		seek(0, WPX_SEEK_SET);
		return true;
	}
	seek(0, WPX_SEEK_SET);
	return false;
}

WPXInputStream* WPXFileStream::getDocumentOLEStream(const char * name)
{
	if (d->buffer.str().empty())
		d->buffer << d->file.rdbuf();
	Storage *tmpStorage = new Storage( d->buffer );
	Stream tmpStream( tmpStorage, name );
	if (!tmpStorage || (tmpStorage->result() != Storage::Ok)  || !tmpStream.size())
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream*)0;
	}
	
	if (d->buf)
		delete [] d->buf;
	d->buf = new uint8_t[tmpStream.size()];

	unsigned long tmpLength;
	tmpLength = tmpStream.read((unsigned char *)(d->buf), tmpStream.size());

	// sanity check
	if (tmpLength > tmpStream.size() || tmpLength < tmpStream.size())
	/* something went wrong here and we do not trust the
	   resulting buffer */
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream*)0;
	}

	delete tmpStorage;
	return new WPXStringStream((const char *)(d->buf), tmpLength);
}

WPXInputStream* WPXFileStream::getDocumentOLEStream()
{
	return getDocumentOLEStream("PerfectOffice_MAIN");
}

WPXStringStream::WPXStringStream(const char *data, const unsigned int dataSize) :
	WPXInputStream(true)
{
	d = new WPXStringStreamPrivate(std::string(data, dataSize));
	d->buffer.seekg( 0, std::ios::end );
	d->streamSize = (d->buffer.good() ? (unsigned long)d->buffer.tellg() : (unsigned long)-1L);
	if (d->streamSize == (unsigned long)-1)
		d->streamSize = 0;
	 // preventing possible unsigned/signed issues later by truncating the file
	if (d->streamSize > (std::numeric_limits<unsigned long>::max)() / 2)
		d->streamSize = (std::numeric_limits<unsigned long>::max)() / 2;
	d->buffer.seekg( 0, std::ios::beg );
}

WPXStringStream::~WPXStringStream()
{
	delete d;
}

const uint8_t *WPXStringStream::read(size_t numBytes, size_t &numBytesRead)
{
	numBytesRead = 0;
	
	if (numBytes < 0 || atEOS() || numBytes > (std::numeric_limits<unsigned long>::max)()/2)
		return 0;

	unsigned long curpos = d->buffer.tellg();
	if (curpos == (unsigned long)-1) //tellg() returned ERROR
		return 0;

	if ( (curpos + numBytes < curpos) /*overflow*/ ||
		(curpos + numBytes > d->streamSize) ) /*reading more than available*/
	{
		numBytes = d->streamSize - curpos;
	}

	if (d->buf)
		delete [] d->buf;
	d->buf = new uint8_t[numBytes];

	if(d->buffer.good())
	{
		d->buffer.read((char *)(d->buf), numBytes); 
		numBytesRead = (long)d->buffer.tellg() - curpos;
	}
	
	return d->buf;
}

long WPXStringStream::tell()
{
	return d->buffer.good() ? (long)d->buffer.tellg() : -1L;
}

int WPXStringStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_SET)
	{
		if (offset < 0)
			offset = 0;
		if (offset > (long)d->streamSize)
			offset = d->streamSize;
	}

	if (seekType == WPX_SEEK_CUR)
	{
		if (tell() + offset < 0)
			offset = -tell();
		if (tell() + offset > (long)d->streamSize)
			offset = d->streamSize - tell();
	}

	if(d->buffer.good())
	{
		d->buffer.seekg(offset, ((seekType == WPX_SEEK_SET) ? std::ios::beg : std::ios::cur));
		return (int) ( (long)d->buffer.tellg() == -1);
	}
	else
		return -1;
}

bool WPXStringStream::atEOS()
{
	return (d->buffer.tellg() >= (long)d->streamSize);
}

bool WPXStringStream::isOLEStream()
{
	Storage tmpStorage( d->buffer );
	if (tmpStorage.isOLEStream())
	{
		seek(0, WPX_SEEK_SET);
		return true;
	}
	seek(0, WPX_SEEK_SET);
	return false;
}

WPXInputStream* WPXStringStream::getDocumentOLEStream(const char * name)
{
	Storage *tmpStorage = new Storage( d->buffer );
	Stream tmpStream( tmpStorage, name );
	if (!tmpStorage || (tmpStorage->result() != Storage::Ok)  || !tmpStream.size())
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream*)0;
	}
	
	if (d->buf)
		delete [] d->buf;
	d->buf = new uint8_t[tmpStream.size()];
	unsigned long tmpLength;
	tmpLength = tmpStream.read((unsigned char *)(d->buf), tmpStream.size());

	// sanity check
	if (tmpLength > tmpStream.size() || tmpLength < tmpStream.size())
	/* something went wrong here and we do not trust the
	   resulting buffer */
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream*)0;
	}

	delete tmpStorage;
	return new WPXStringStream((const char *)(d->buf), tmpLength);
}

WPXInputStream* WPXStringStream::getDocumentOLEStream()
{
	return getDocumentOLEStream("PerfectOffice_MAIN");
}
