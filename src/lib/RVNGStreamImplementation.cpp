/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
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
 */

#include <limits>
#include <string>
#include <vector>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <librevenge-stream/librevenge-stream.h>
#include "RVNGOLEStream.h"
#include "RVNGZipStream.h"

#ifndef S_ISREG
#define S_ISREG(x) (((x) & S_IFMT) == S_IFREG)
#endif

namespace librevenge
{

enum RVNGStreamType { UNKNOWN, FLAT, OLE2, ZIP };

class RVNGFileStreamPrivate
{
public:
	RVNGFileStreamPrivate();
	~RVNGFileStreamPrivate();
	FILE *file;
	unsigned long streamSize;
	unsigned char *readBuffer;
	unsigned long readBufferLength;
	unsigned long readBufferPos;
	RVNGStreamType streamType;
	std::vector<std::string> streamNameList;
private:
	RVNGFileStreamPrivate(const RVNGFileStreamPrivate &);
	RVNGFileStreamPrivate &operator=(const RVNGFileStreamPrivate &);
};

class RVNGStringStreamPrivate
{
public:
	RVNGStringStreamPrivate(const unsigned char *data, unsigned dataSize);
	~RVNGStringStreamPrivate();
	std::vector<unsigned char> buffer;
	volatile long offset;
	RVNGStreamType streamType;
	std::vector<std::string> streamNameList;
private:
	RVNGStringStreamPrivate(const RVNGStringStreamPrivate &);
	RVNGStringStreamPrivate &operator=(const RVNGStringStreamPrivate &);
};

RVNGFileStreamPrivate::RVNGFileStreamPrivate() :
	file(0),
	streamSize(0),
	readBuffer(0),
	readBufferLength(0),
	readBufferPos(0),
	streamType(UNKNOWN),
	streamNameList()
{
}

RVNGFileStreamPrivate::~RVNGFileStreamPrivate()
{
	if (file)
		fclose(file);
	if (readBuffer)
		delete [] readBuffer;
}

RVNGStringStreamPrivate::RVNGStringStreamPrivate(const unsigned char *data, unsigned dataSize) :
	buffer(dataSize),
	offset(0),
	streamType(UNKNOWN),
	streamNameList()

{
	memcpy(&buffer[0], data, dataSize);
}

RVNGStringStreamPrivate::~RVNGStringStreamPrivate()
{
}

RVNGFileStream::RVNGFileStream(const char *filename) :
	RVNGInputStream(),
	d(new RVNGFileStreamPrivate())
{
	d->file = fopen(filename, "rb");
	if (!d->file || ferror(d->file))
	{
		delete d;
		d = 0;
		return;
	}

	struct stat status;
	const int retval = stat(filename, &status);
	if ((0 != retval) || !S_ISREG(status.st_mode))
	{
		delete d;
		d = 0;
		return;
	}

	fseek(d->file, 0, SEEK_END);

	d->streamSize = (unsigned long) ftell(d->file);
	if (d->streamSize == (unsigned long)-1)
		d->streamSize = 0;
	// preventing possible unsigned/signed issues later by truncating the file
	if (d->streamSize > (std::numeric_limits<unsigned long>::max)() / 2)
		d->streamSize = (std::numeric_limits<unsigned long>::max)() / 2;
	fseek(d->file, 0, SEEK_SET);
}

RVNGFileStream::~RVNGFileStream()
{
	if (d)
		delete d;
}

#define BUFFER_MAX 65536

const unsigned char *RVNGFileStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	numBytesRead = 0;

	if (!d)
		return 0;

	if (numBytes == 0 || numBytes > (std::numeric_limits<unsigned long>::max)()/2
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
		fseek(d->file, (long)ftell(d->file) - (long)d->readBufferLength, SEEK_SET);
		fseek(d->file, (long)d->readBufferPos, SEEK_CUR);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	unsigned long curpos = (unsigned long) tell();
	if (curpos == (unsigned long)-1)  // tellg() returned ERROR
		return 0;

	if ((curpos + numBytes < curpos) /*overflow*/ ||
	        (curpos + numBytes >= d->streamSize))  /*reading more than available*/
	{
		numBytes = d->streamSize - curpos;
	}

	if (numBytes == 0)
		return 0;

	if (numBytes < BUFFER_MAX)
	{
		if (BUFFER_MAX < d->streamSize - curpos)
			d->readBufferLength = BUFFER_MAX;
		else /* BUFFER_MAX >= d->streamSize - curpos */
			d->readBufferLength = d->streamSize - curpos;
	}
	else
		d->readBufferLength = numBytes;

	fseek(d->file, (long) d->readBufferLength, SEEK_CUR);
	fseek(d->file, (long) curpos, SEEK_SET);

	d->readBuffer = new unsigned char[d->readBufferLength];
	unsigned long tmpNumBytes;
	if ((tmpNumBytes = fread((void *)(d->readBuffer), 1, d->readBufferLength, d->file)) != d->readBufferLength)
		d->readBufferLength = tmpNumBytes;

	d->readBufferPos = 0;
	if (!d->readBufferLength)
		return 0;

	numBytesRead = numBytes;

	d->readBufferPos += numBytesRead;
	return const_cast<const unsigned char *>(d->readBuffer);
}

long RVNGFileStream::tell()
{
	if (!d)
		return -1L;
	return ferror(d->file) ? -1L : (long)ftell(d->file) - (long)d->readBufferLength + (long) d->readBufferPos;
}

int RVNGFileStream::seek(long offset, RVNG_SEEK_TYPE seekType)
{
	if (!d)
		return -1;
	if (seekType == RVNG_SEEK_CUR)
		offset += tell();
	if (seekType == RVNG_SEEK_END)
		offset += (long)d->streamSize;

	if (offset < 0)
		offset = 0;
	if (offset > (long)d->streamSize)
		offset = (long)d->streamSize;

	if (!ferror(d->file) && offset < ftell(d->file) && (unsigned long)offset >= (unsigned long)ftell(d->file) - d->readBufferLength)
	{
		d->readBufferPos = (unsigned long)(offset + (long) d->readBufferLength - (long) ftell(d->file));
		return 0;
	}

	if (d->readBuffer) // seeking outside of the buffer, so invalidate the buffer
	{
		fseek(d->file, (long)ftell(d->file) - (long)d->readBufferLength, SEEK_SET);
		fseek(d->file, (long) d->readBufferPos, SEEK_CUR);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	if (!ferror(d->file))
	{
		fseek(d->file, offset, SEEK_SET);
		return (int)((long)ftell(d->file) == -1) ;
	}
	else
		return -1;
}

bool RVNGFileStream::isEnd()
{
	if (!d)
		return true;
	return (tell() >= (long)d->streamSize);
}

bool RVNGFileStream::isStructured()
{
	if (!d)
		return false;
	if (ferror(d->file))
		return false;
	if (d->streamType == UNKNOWN)
	{
		seek(0, RVNG_SEEK_SET);

		// Check whether it is OLE2 storage
		Storage tmpStorage(this);
		if (tmpStorage.isStructured())
		{
			d->streamType = OLE2;
			d->streamNameList = tmpStorage.getSubStreamNamesList();
			seek(0, RVNG_SEEK_SET);
			return true;
		}
		seek(0, RVNG_SEEK_SET);
		if (RVNGZipStream::isZipFile(this))
		{
			d->streamType = ZIP;
			d->streamNameList = RVNGZipStream::getSubStreamNamesList(this);
			seek(0, RVNG_SEEK_SET);
			return true;
		}
		d->streamType = FLAT;
		return false;
	}
	else if (d->streamType == FLAT)
		return false;
	else
		return true;
}

unsigned RVNGFileStream::subStreamCount()
{
	if (!isStructured()||!d) return 0;
	return (unsigned) d->streamNameList.size();
}

const char *RVNGFileStream::subStreamName(unsigned id)
{
	if (!isStructured() ||!d || id>=(unsigned) d->streamNameList.size())
		return 0;
	return d->streamNameList[size_t(id)].c_str();
}

bool RVNGFileStream::existsSubStream(const char *name)
{
	if (!name || !d)
		return false;
	if (ferror(d->file))
		return false;
	if (d->streamType == UNKNOWN && !isStructured())
		return false;
	if (d->streamType == FLAT)
		return false;
	for (std::vector<std::string>::size_type i = 0; i < d->streamNameList.size(); ++i)
	{
		if (d->streamNameList[i] == name)
			return true;
	}
	return false;
}

RVNGInputStream *RVNGFileStream::getSubStreamById(unsigned id)
{
	return getSubStreamByName(subStreamName(id));
}

RVNGInputStream *RVNGFileStream::getSubStreamByName(const char *name)
{
	if (!name || !d)
		return 0;
	if (ferror(d->file))
		return 0;
	if (d->streamType == UNKNOWN && !isStructured())
		return 0;
	if (d->streamType == OLE2)
	{
		seek(0, RVNG_SEEK_SET);
		Storage tmpStorage(this);
		Stream tmpStream(&tmpStorage, name);
		if (tmpStorage.result() != Storage::Ok  || !tmpStream.size())
			return (RVNGInputStream *)0;

		std::vector<unsigned char> buf(tmpStream.size());
		unsigned long tmpLength;
		tmpLength = tmpStream.read(&buf[0], tmpStream.size());

		// sanity check
		if (tmpLength != tmpStream.size())
			/* something went wrong here and we do not trust the
			   resulting buffer */
			return (RVNGInputStream *)0;

		return new RVNGStringStream(&buf[0], (unsigned)tmpLength);
	}
	else if (d->streamType == ZIP)
	{
		seek(0, RVNG_SEEK_SET);
		return RVNGZipStream::getSubstream(this, name);
	}
	return 0;
}

RVNGStringStream::RVNGStringStream(const unsigned char *data, const unsigned int dataSize) :
	RVNGInputStream(),
	d(new RVNGStringStreamPrivate(data, dataSize))
{
}

RVNGStringStream::~RVNGStringStream()
{
	delete d;
}

const unsigned char *RVNGStringStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	numBytesRead = 0;

	if (numBytes == 0)
		return 0;

	long numBytesToRead;

	if ((unsigned long)d->offset+numBytes < d->buffer.size())
		numBytesToRead = (long) numBytes;
	else
		numBytesToRead = (long) d->buffer.size() - d->offset;

	numBytesRead = (unsigned long) numBytesToRead; // about as paranoid as we can be..

	if (numBytesToRead == 0)
		return 0;

	long oldOffset = d->offset;
	d->offset += numBytesToRead;

	return &d->buffer[size_t(oldOffset)];

}

long RVNGStringStream::tell()
{
	return d->offset;
}

int RVNGStringStream::seek(long offset, RVNG_SEEK_TYPE seekType)
{
	if (seekType == RVNG_SEEK_CUR)
		d->offset += offset;
	else if (seekType == RVNG_SEEK_SET)
		d->offset = offset;
	else if (seekType == RVNG_SEEK_END)
		d->offset = offset+(long) d->buffer.size();

	if (d->offset < 0)
	{
		d->offset = 0;
		return -1;
	}
	if ((long)d->offset > (long)d->buffer.size())
	{
		d->offset = (long) d->buffer.size();
		return -1;
	}

	return 0;
}

bool RVNGStringStream::isEnd()
{
	if ((long)d->offset >= (long)d->buffer.size())
		return true;

	return false;
}

bool RVNGStringStream::isStructured()
{
	if (d->buffer.empty())
		return false;

	if (d->streamType == UNKNOWN)
	{
		seek(0, RVNG_SEEK_SET);

		// Check whether it is OLE2 storage
		Storage tmpStorage(this);
		if (tmpStorage.isStructured())
		{
			d->streamType = OLE2;
			d->streamNameList = tmpStorage.getSubStreamNamesList();
			return true;
		}
		seek(0, RVNG_SEEK_SET);
		if (RVNGZipStream::isZipFile(this))
		{
			d->streamType = ZIP;
			d->streamNameList = RVNGZipStream::getSubStreamNamesList(this);
			return true;
		}
		d->streamType = FLAT;
		return false;
	}
	else if (d->streamType == FLAT)
		return false;
	else
		return true;
}

unsigned RVNGStringStream::subStreamCount()
{
	if (!isStructured()||!d) return 0;
	return (unsigned) d->streamNameList.size();
}

const char *RVNGStringStream::subStreamName(unsigned id)
{
	if (!isStructured() ||!d || id>=(unsigned) d->streamNameList.size())
		return 0;
	return d->streamNameList[size_t(id)].c_str();
}

bool RVNGStringStream::existsSubStream(const char *name)
{
	if (!name || !d)
		return false;
	if (d->buffer.empty())
		return false;
	if (d->streamType == UNKNOWN && !isStructured())
		return false;
	if (d->streamType == FLAT)
		return false;
	for (std::vector<std::string>::size_type i = 0; i < d->streamNameList.size(); ++i)
	{
		if (d->streamNameList[i] == name)
			return true;
	}
	return false;
}

RVNGInputStream *RVNGStringStream::getSubStreamById(unsigned id)
{
	return getSubStreamByName(subStreamName(id));
}

RVNGInputStream *RVNGStringStream::getSubStreamByName(const char *name)
{
	if (!name || d->buffer.empty())
		return 0;
	if (d->streamType == UNKNOWN && !isStructured())
		return 0;

	if (d->streamType == OLE2)
	{
		seek(0, RVNG_SEEK_SET);
		Storage tmpStorage(this);
		Stream tmpStream(&tmpStorage, name);
		if (tmpStorage.result() != Storage::Ok  || !tmpStream.size())
			return (RVNGInputStream *)0;

		std::vector<unsigned char> buf(tmpStream.size());
		unsigned long tmpLength;
		tmpLength = tmpStream.read(&buf[0], tmpStream.size());

		// sanity check
		if (tmpLength != tmpStream.size())
			/* something went wrong here and we do not trust the
			   resulting buffer */
			return (RVNGInputStream *)0;

		return new RVNGStringStream(&buf[0], (unsigned)tmpLength);
	}
	else if (d->streamType == ZIP)
		return RVNGZipStream::getSubstream(this, name);
	return 0;
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
