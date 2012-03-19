/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell, Inc. (http://www.novell.com)
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

#include <limits>
#include <string.h>

using namespace libwpd;

class WPXFileStreamPrivate
{
public:
	WPXFileStreamPrivate();
	~WPXFileStreamPrivate();
	FILE *file;
	unsigned long streamSize;
	unsigned char *readBuffer;
	unsigned long readBufferLength;
	unsigned long readBufferPos;
private:
	WPXFileStreamPrivate(const WPXFileStreamPrivate &);
	WPXFileStreamPrivate &operator=(const WPXFileStreamPrivate &);
};

class WPXStringStreamPrivate
{
public:
	WPXStringStreamPrivate(const unsigned char *data, unsigned dataSize);
	~WPXStringStreamPrivate();
	std::vector<unsigned char> buffer;
	volatile long offset;
private:
	WPXStringStreamPrivate(const WPXStringStreamPrivate &);
	WPXStringStreamPrivate &operator=(const WPXStringStreamPrivate &);
};

WPXFileStreamPrivate::WPXFileStreamPrivate() :
	file(0),
	streamSize(0),
	readBuffer(0),
	readBufferLength(0),
	readBufferPos(0)
{
}

WPXFileStreamPrivate::~WPXFileStreamPrivate()
{
	if (file)
		fclose(file);
	if (readBuffer)
		delete [] readBuffer;
}

WPXStringStreamPrivate::WPXStringStreamPrivate(const unsigned char *data, unsigned dataSize) :
	buffer(dataSize),
	offset(0)

{
	memcpy(&buffer[0], data, dataSize);
}

WPXStringStreamPrivate::~WPXStringStreamPrivate()
{
}

WPXFileStream::WPXFileStream(const char *filename) :
	WPXInputStream(),
	d(new WPXFileStreamPrivate)
{
	d->file = fopen( filename, "rb" );
	fseek(d->file, 0, SEEK_END);
	d->streamSize = ftell(d->file);

	if (d->streamSize == (unsigned long)-1)
		d->streamSize = 0;
	// preventing possible unsigned/signed issues later by truncating the file
	if (d->streamSize > (std::numeric_limits<unsigned long>::max)() / 2)
		d->streamSize = (std::numeric_limits<unsigned long>::max)() / 2;
	fseek(d->file, 0, SEEK_SET );
}

WPXFileStream::~WPXFileStream()
{
	delete d;
}

#define BUFFER_MAX 65536

const unsigned char *WPXFileStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	numBytesRead = 0;

	if (numBytes == 0 || /* atEOS() || */ numBytes > (std::numeric_limits<unsigned long>::max)()/2
	        || ferror(d->file))
		return 0;

	// can we read from the buffer?
	if (d->readBuffer && (d->readBufferPos + numBytes > d->readBufferPos)
	        && (d->readBufferPos + numBytes <= d->readBufferLength))
	{
		const unsigned char *pTmp = d->readBuffer + d->readBufferPos;
		d->readBufferPos += numBytes;
		numBytesRead = numBytes;
		return pTmp;
	}

	// hmm, we cannot: go back by the bytes we read ahead && invalidate the buffer
	if (d->readBuffer)
	{
		fseek(d->file, (unsigned long)ftell(d->file) - d->readBufferLength, SEEK_SET);
		fseek(d->file, d->readBufferPos, SEEK_CUR);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	unsigned long curpos = tell();
	if (curpos == (unsigned long)-1)  // tellg() returned ERROR
		return 0;

	if ( (curpos + numBytes < curpos) /*overflow*/ ||
	        (curpos + numBytes >= d->streamSize) ) /*reading more than available*/
	{
		numBytes = d->streamSize - curpos;
	}

	if (numBytes < BUFFER_MAX)
	{
		if (BUFFER_MAX < d->streamSize - curpos)
			d->readBufferLength = BUFFER_MAX;
		else /* BUFFER_MAX >= d->streamSize - curpos */
			d->readBufferLength = d->streamSize - curpos;
	}
	else
		d->readBufferLength = numBytes;

	fseek(d->file, d->readBufferLength, SEEK_CUR);
	fseek(d->file, curpos, SEEK_SET);

	d->readBuffer = new unsigned char[d->readBufferLength];
	fread((void *)(d->readBuffer), 1, d->readBufferLength, d->file);

	d->readBufferPos = 0;
	if (!d->readBufferLength)
		return 0;

	numBytesRead = numBytes;

	d->readBufferPos += numBytesRead;
	return const_cast<const unsigned char *>( d->readBuffer );
}

long WPXFileStream::tell()
{
	return ferror(d->file) ? -1L : (long)((long)ftell(d->file) - d->readBufferLength + d->readBufferPos);
}

int WPXFileStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_CUR)
		offset += tell();

	if (offset < 0)
		offset = 0;
	if (offset > (long)d->streamSize)
		offset = (long)d->streamSize;

	if (!ferror(d->file) && offset < ftell(d->file) && (unsigned long)offset >= (unsigned long)ftell(d->file) - d->readBufferLength)
	{
		d->readBufferPos = offset + d->readBufferLength - ftell(d->file);
		return 0;
	}

	if (d->readBuffer) // seeking outside of the buffer, so invalidate the buffer
	{
		fseek(d->file, (unsigned long)ftell(d->file) - d->readBufferLength, SEEK_SET);
		fseek(d->file, d->readBufferPos, SEEK_CUR);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	if(!ferror(d->file))
	{
		fseek(d->file, offset, SEEK_SET);
		return (int) ((long)ftell(d->file) == -1) ;
	}
	else
		return -1;
}

bool WPXFileStream::atEOS()
{
	return (tell() >= (long)d->streamSize);
}

bool WPXFileStream::isOLEStream()
{
	if (ferror(d->file))
		return false;
	seek(0, WPX_SEEK_CUR);

	unsigned long numBytesRead;
	const unsigned char *buffer = read(d->streamSize, numBytesRead);
	seek(0, WPX_SEEK_SET);
	if (numBytesRead != d->streamSize)
		return false;

	std::vector<unsigned char> buf(d->streamSize);
	memcpy(&buf[0], buffer, d->streamSize);

	Storage tmpStorage( buf );
	if (tmpStorage.isOLEStream())
		return true;
	return false;
}

WPXInputStream *WPXFileStream::getDocumentOLEStream(const char *name)
{
	if (ferror(d->file))
		return (WPXInputStream *)0;

	seek(0, WPX_SEEK_SET);

	unsigned long numBytesRead;
	const unsigned char *buffer = read(d->streamSize, numBytesRead);
	seek(0, WPX_SEEK_SET);
	if (numBytesRead != d->streamSize)
		return (WPXInputStream *)0;

	std::vector<unsigned char> buf(d->streamSize);
	memcpy(&buf[0], buffer, d->streamSize);

	Storage tmpStorage( buf );
	Stream tmpStream( &tmpStorage, name );
	if (tmpStorage.result() != Storage::Ok  || !tmpStream.size())
		return (WPXInputStream *)0;

	std::vector<unsigned char> tmpBuf(tmpStream.size());

	unsigned long tmpLength = tmpStream.read(&tmpBuf[0], tmpStream.size());

	// sanity check
	if (tmpLength != tmpStream.size())
		/* something went wrong here and we do not trust the
		   resulting buffer */
		return (WPXInputStream *)0;

	return new WPXStringStream(&tmpBuf[0], tmpLength);
}

WPXStringStream::WPXStringStream(const unsigned char *data, const unsigned int dataSize) :
	WPXInputStream(),
	d(new WPXStringStreamPrivate(data, dataSize))
{
}

WPXStringStream::~WPXStringStream()
{
	delete d;
}

const unsigned char *WPXStringStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	numBytesRead = 0;

	if (numBytes == 0)
		return 0;

	int numBytesToRead;

	if ((d->offset+numBytes) < d->buffer.size())
		numBytesToRead = numBytes;
	else
		numBytesToRead = d->buffer.size() - d->offset;

	numBytesRead = numBytesToRead; // about as paranoid as we can be..

	if (numBytesToRead == 0)
		return 0;

	long oldOffset = d->offset;
	d->offset += numBytesToRead;

	return &d->buffer[oldOffset];

}

long WPXStringStream::tell()
{
	return d->offset;
}

int WPXStringStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_CUR)
		d->offset += offset;
	else if (seekType == WPX_SEEK_SET)
		d->offset = offset;

	if (d->offset < 0)
	{
		d->offset = 0;
		return 1;
	}
	if ((long)d->offset > (long)d->buffer.size())
	{
		d->offset = d->buffer.size();
		return 1;
	}

	return 0;
}

bool WPXStringStream::atEOS()
{
	if ((long)d->offset >= (long)d->buffer.size())
		return true;

	return false;
}

bool WPXStringStream::isOLEStream()
{
	if (d->buffer.empty())
		return false;

	Storage tmpStorage( d->buffer );
	if (tmpStorage.isOLEStream())
	{
		seek(0, WPX_SEEK_SET);
		return true;
	}
	seek(0, WPX_SEEK_SET);
	return false;
}

WPXInputStream *WPXStringStream::getDocumentOLEStream(const char *name)
{
	if (!d->buffer.empty())
		return 0;

	Storage tmpStorage( d->buffer );
	Stream tmpStream( &tmpStorage, name );
	if (tmpStorage.result() != Storage::Ok  || !tmpStream.size())
		return (WPXInputStream *)0;

	std::vector<unsigned char> buf(tmpStream.size());
	unsigned long tmpLength;
	tmpLength = tmpStream.read(&buf[0], tmpStream.size());

	// sanity check
	if (tmpLength != tmpStream.size())
		/* something went wrong here and we do not trust the
		   resulting buffer */
		return (WPXInputStream *)0;

	return new WPXStringStream(&buf[0], tmpLength);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
