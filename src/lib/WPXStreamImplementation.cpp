/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "WPXStreamImplementation.h"
#include "WPXOLEStream.h"
#ifdef BUILD_ZIP_STREAM
#include "WPXZipStream.h"
#endif

#include <limits>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef S_ISREG
#define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif

using namespace libwpd;

enum WPXStreamType { UNKNOWN, FLAT, OLE2, ZIP };

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
	WPXStreamType streamType;
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
	WPXStreamType streamType;
private:
	WPXStringStreamPrivate(const WPXStringStreamPrivate &);
	WPXStringStreamPrivate &operator=(const WPXStringStreamPrivate &);
};

WPXFileStreamPrivate::WPXFileStreamPrivate() :
	file(0),
	streamSize(0),
	readBuffer(0),
	readBufferLength(0),
	readBufferPos(0),
	streamType(UNKNOWN)
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
	offset(0),
	streamType(UNKNOWN)

{
	memcpy(&buffer[0], data, dataSize);
}

WPXStringStreamPrivate::~WPXStringStreamPrivate()
{
}

WPXFileStream::WPXFileStream(const char *filename) :
	WPXInputStream(),
	d(new WPXFileStreamPrivate())
{
	d->file = fopen( filename, "rb" );
	if (!d->file || ferror(d->file))
	{
		delete d;
		d = 0;
		return;
	}

	struct stat status;
	stat( filename, &status );
	if ( !S_ISREG(status.st_mode) )
	{
		delete d;
		d = 0;
		return;
	}

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
	if (d)
		delete d;
}

#define BUFFER_MAX 65536

const unsigned char *WPXFileStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	if (!d)
		return 0;
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
	unsigned long tmpNumBytes;
	if ((tmpNumBytes = fread((void *)(d->readBuffer), 1, d->readBufferLength, d->file)) != d->readBufferLength)
		d->readBufferLength = tmpNumBytes;

	d->readBufferPos = 0;
	if (!d->readBufferLength)
		return 0;

	numBytesRead = numBytes;

	d->readBufferPos += numBytesRead;
	return const_cast<const unsigned char *>( d->readBuffer );
}

long WPXFileStream::tell()
{
	if (!d)
		return -1L;
	return ferror(d->file) ? -1L : (long)((long)ftell(d->file) - d->readBufferLength + d->readBufferPos);
}

int WPXFileStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (!d)
		return -1;
	if (seekType == WPX_SEEK_CUR)
		offset += tell();
	if (seekType == WPX_SEEK_END)
		offset += (long)d->streamSize;

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
	if (!d)
		return true;
	return (tell() >= (long)d->streamSize);
}

bool WPXFileStream::isOLEStream()
{
	if (!d)
		return false;
	if (ferror(d->file))
		return false;
	if (d->streamType == UNKNOWN)
	{
		seek(0, WPX_SEEK_SET);

		// Check whether it is OLE2 storage
		Storage tmpStorage( this );
		if (tmpStorage.isOLEStream())
		{
			d->streamType = OLE2;
			return true;
		}
#ifdef BUILD_ZIP_STREAM
		seek(0, WPX_SEEK_SET);
		if (WPXZipStream::isZipFile(this))
		{
			d->streamType = ZIP;
			return true;
		}
#endif
		d->streamType = FLAT;
		return false;
	}
	else if (d->streamType == FLAT)
		return false;
	else
		return true;
}

WPXInputStream *WPXFileStream::getDocumentOLEStream(const char *name)
{
	if (!d)
		return 0;
	if (ferror(d->file))
		return 0;
	if (d->streamType == UNKNOWN && !isOLEStream())
		return 0;
	if (d->streamType == OLE2)
	{
		seek(0, WPX_SEEK_SET);
		Storage tmpStorage( this );
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

		return new WPXStringStream(&buf[0], (unsigned)tmpLength);
	}
#ifdef BUILD_ZIP_STREAM
	else if (d->streamType == ZIP)
		return WPXZipStream::getSubstream(this, name);
#endif
	return 0;
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

	long numBytesToRead;

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
	else if (seekType == WPX_SEEK_END)
		d->offset += d->buffer.size();

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

	if (d->streamType == UNKNOWN)
	{
		seek(0, WPX_SEEK_SET);

		// Check whether it is OLE2 storage
		Storage tmpStorage( this );
		if (tmpStorage.isOLEStream())
		{
			d->streamType = OLE2;
			return true;
		}
#ifdef BUILD_ZIP_STREAM
		seek(0, WPX_SEEK_SET);
		if (WPXZipStream::isZipFile(this))
		{
			d->streamType = ZIP;
			return true;
		}
#endif
		d->streamType = FLAT;
		return false;
	}
	else if (d->streamType == FLAT)
		return false;
	else
		return true;
}

WPXInputStream *WPXStringStream::getDocumentOLEStream(const char *name)
{
	if (d->buffer.empty())
		return 0;
	if (d->streamType == UNKNOWN && !isOLEStream())
		return 0;

	if (d->streamType == OLE2)
	{
		seek(0, WPX_SEEK_SET);
		Storage tmpStorage( this );
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

		return new WPXStringStream(&buf[0], (unsigned)tmpLength);
	}
#ifdef BUILD_ZIP_STREAM
	else if (d->streamType == ZIP)
		return WPXZipStream::getSubstream(this, name);
#endif
	return 0;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
