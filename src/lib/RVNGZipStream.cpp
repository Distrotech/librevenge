/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2012 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include <string>
#include <string.h>
#include <stdio.h>
#include <zlib.h>
#include "RVNGZipStream.h"
#include <librevenge-stream/librevenge-stream.h>

namespace librevenge
{

namespace
{
class StreamException
{
};

struct LocalFileHeader
{
	unsigned short min_version;
	unsigned short general_flag;
	unsigned short compression;
	unsigned short lastmod_time;
	unsigned short lastmod_date;
	unsigned crc32;
	unsigned compressed_size;
	unsigned uncompressed_size;
	unsigned short filename_size;
	unsigned short extra_field_size;
	std::string filename;
	std::string extra_field;
	LocalFileHeader()
		: min_version(0), general_flag(0), compression(0), lastmod_time(0), lastmod_date(0),
		  crc32(0), compressed_size(0), uncompressed_size(0), filename_size(0), extra_field_size(0),
		  filename(), extra_field() {}
	~LocalFileHeader() {}
};

struct CentralDirectoryEntry
{
	unsigned short creator_version;
	unsigned short min_version;
	unsigned short general_flag;
	unsigned short compression;
	unsigned short lastmod_time;
	unsigned short lastmod_date;
	unsigned crc32;
	unsigned compressed_size;
	unsigned uncompressed_size;
	unsigned short filename_size;
	unsigned short extra_field_size;
	unsigned short file_comment_size;
	unsigned short disk_num;
	unsigned short internal_attr;
	unsigned external_attr;
	unsigned offset;
	std::string filename;
	std::string extra_field;
	std::string file_comment;
	CentralDirectoryEntry()
		: creator_version(0), min_version(0), general_flag(0), compression(0), lastmod_time(0),
		  lastmod_date(0), crc32(0), compressed_size(0), uncompressed_size(0), filename_size(0),
		  extra_field_size(0), file_comment_size(0), disk_num(0), internal_attr(0),
		  external_attr(0), offset(0), filename(), extra_field(), file_comment() {}
	~CentralDirectoryEntry() {}
};

struct CentralDirectoryEnd
{
	unsigned short disk_num;
	unsigned short cdir_disk;
	unsigned short disk_entries;
	unsigned short cdir_entries;
	unsigned cdir_size;
	unsigned cdir_offset;
	unsigned short comment_size;
	std::string comment;
	CentralDirectoryEnd()
		: disk_num(0), cdir_disk(0), disk_entries(0), cdir_entries(0),
		  cdir_size(0), cdir_offset(0), comment_size(0), comment() {}
	~CentralDirectoryEnd() {}
};

#define CDIR_ENTRY_SIG 0x02014b50
#define LOC_FILE_HEADER_SIG 0x04034b50
#define CDIR_END_SIG 0x06054b50

static unsigned short getShort(RVNGInputStream *input)
{
	unsigned long numBytesRead = 0;
	const unsigned char *ret = input->read(2, numBytesRead);
	if (numBytesRead != 2)
		throw StreamException();
	return (unsigned short)(ret[0]|((unsigned short)ret[1]<<8));
}

static unsigned getInt(RVNGInputStream *input)
{
	unsigned long numBytesRead = 0;
	const unsigned char *ret = input->read(4, numBytesRead);
	if (numBytesRead != 4)
		throw StreamException();
	return (unsigned)(ret[0]|((unsigned)ret[1]<<8)|((unsigned)ret[2]<<16)|((unsigned)ret[3]<<24));
}

static bool readCentralDirectoryEnd(RVNGInputStream *input, CentralDirectoryEnd &end)
{
	try
	{
		unsigned signature = getInt(input);
		if (signature != CDIR_END_SIG)
			return false;

		end.disk_num = getShort(input);
		end.cdir_disk = getShort(input);
		end.disk_entries = getShort(input);
		end.cdir_entries = getShort(input);
		end.cdir_size = getInt(input);
		end.cdir_offset = getInt(input);
		end.comment_size = getShort(input);

		end.comment.clear();
		if (end.comment_size)
		{
			// check if the comment field size seems coherent
			long pos=input->tell();
			if (input->seek(long(end.comment_size), librevenge::RVNG_SEEK_CUR)!=0)
				return false;
			input->seek(pos, librevenge::RVNG_SEEK_SET);

			unsigned long read;
			const unsigned char *data=input->read((unsigned long) end.comment_size, read);
			if (!data || read!=(unsigned long) end.comment_size)
				return false;
			end.comment.append((const char *) data, size_t(end.comment_size));
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

static bool readCentralDirectoryEntry(RVNGInputStream *input, CentralDirectoryEntry &entry)
{
	try
	{
		unsigned signature = getInt(input);
		if (signature != CDIR_ENTRY_SIG)
			return false;

		entry.creator_version = getShort(input);
		entry.min_version = getShort(input);
		entry.general_flag = getShort(input);
		entry.compression = getShort(input);
		entry.lastmod_time = getShort(input);
		entry.lastmod_date = getShort(input);
		entry.crc32 = getInt(input);
		entry.compressed_size = getInt(input);
		entry.uncompressed_size = getInt(input);
		entry.filename_size = getShort(input);
		entry.extra_field_size = getShort(input);
		entry.file_comment_size = getShort(input);
		entry.disk_num = getShort(input);
		entry.internal_attr = getShort(input);
		entry.external_attr = getInt(input);
		entry.offset = getInt(input);

		// check if the field sizes seem coherent
		long pos=input->tell();
		if (input->seek(long(entry.filename_size+entry.extra_field_size+entry.file_comment_size),
		                librevenge::RVNG_SEEK_CUR)!=0)
			return false;
		input->seek(pos, librevenge::RVNG_SEEK_SET);

		entry.filename.clear();
		if (entry.filename_size)
		{
			unsigned long read;
			const unsigned char *data=input->read((unsigned long) entry.filename_size, read);
			if (!data || read!=(unsigned long) entry.filename_size)
				return false;
			entry.filename.append((const char *) data, size_t(entry.filename_size));
		}
		entry.extra_field.clear();
		if (entry.extra_field_size)
		{
			unsigned long read;
			const unsigned char *data=input->read((unsigned long) entry.extra_field_size, read);
			if (!data || read!=(unsigned long) entry.extra_field_size)
				return false;
			entry.extra_field.append((const char *) data, size_t(entry.extra_field_size));
		}
		entry.file_comment.clear();
		if (entry.file_comment_size)
		{
			unsigned long read;
			const unsigned char *data=input->read((unsigned long) entry.file_comment_size, read);
			if (!data || read!=(unsigned long) entry.file_comment_size)
				return false;
			entry.file_comment.append((const char *) data, size_t(entry.file_comment_size));
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

static bool readLocalFileHeader(RVNGInputStream *input, LocalFileHeader &header)
{
	try
	{
		unsigned signature = getInt(input);
		if (signature != LOC_FILE_HEADER_SIG)
			return false;

		header.min_version = getShort(input);
		header.general_flag = getShort(input);
		header.compression = getShort(input);
		header.lastmod_time = getShort(input);
		header.lastmod_date = getShort(input);
		header.crc32 = getInt(input);
		header.compressed_size = getInt(input);
		header.uncompressed_size = getInt(input);
		header.filename_size = getShort(input);
		header.extra_field_size = getShort(input);
		// check if the field sizes seem coherent
		long pos=input->tell();
		if (input->seek(long(header.filename_size+header.extra_field_size), librevenge::RVNG_SEEK_CUR)!=0)
			return false;
		input->seek(pos, librevenge::RVNG_SEEK_SET);
		header.filename.clear();
		if (header.filename_size)
		{
			unsigned long read;
			const unsigned char *data=input->read((unsigned long) header.filename_size, read);
			if (!data || read!=(unsigned long) header.filename_size)
				return false;
			header.filename.append((const char *) data, size_t(header.filename_size));
		}
		header.extra_field.clear();
		if (header.extra_field_size)
		{
			unsigned long read;
			const unsigned char *data=input->read((unsigned long) header.extra_field_size, read);
			if (!data || read!=(unsigned long) header.extra_field_size)
				return false;
			header.extra_field.append((const char *) data, size_t(header.extra_field_size));
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

static bool areHeadersConsistent(const LocalFileHeader &header, const CentralDirectoryEntry &entry)
{
	if (header.min_version != entry.min_version)
		return false;
	if (header.general_flag != entry.general_flag)
		return false;
	if (header.compression != entry.compression)
		return false;
	if (!(header.general_flag & 0x08))
	{
		if (header.crc32 != entry.crc32)
			return false;
		if (header.compressed_size != entry.compressed_size)
			return false;
		if (header.uncompressed_size != entry.uncompressed_size)
			return false;
	}
	return true;
}

static bool findCentralDirectoryEnd(RVNGInputStream *input)
{
	try
	{
		input->seek(0, RVNG_SEEK_END);
		long size = input->tell();

		// CentralDirectoryEnd is CDIR_END_SIG:4+at least 18 other bytes
		if (size < 22) return false;
		if (input->seek(size>1024 ? size-1024 : 0, RVNG_SEEK_SET))
			return false;
		long pos=input->tell();
		long toCheck=(size-18)-pos;
		unsigned long numBytesRead = 0;
		unsigned char const *ret =
		    input->read((unsigned long) toCheck, numBytesRead);
		if (!ret || long(numBytesRead)!=toCheck)
			return false;
		unsigned const sigRev=
		    ((CDIR_END_SIG&0xFF)<<24)|
		    ((CDIR_END_SIG&0xFF00)<<8)|
		    ((CDIR_END_SIG&0xFF0000)>>8)|
		    ((CDIR_END_SIG&0xFF000000)>>24);
		unsigned signature=0;
		for (long p=0; p < toCheck; p++)
		{
			signature=((signature&0xFFFFFF)<<8)|*(ret++);
			if (signature == sigRev)
			{
				input->seek(pos+p-3, RVNG_SEEK_SET);
				return true;
			}
		}

	}
	catch (...)
	{
		return false;
	}
	return false;
}

static bool findDataStream(RVNGInputStream *input, CentralDirectoryEntry &entry, const char *name)
{
	size_t name_size = strlen(name);
	if (!findCentralDirectoryEnd(input))
		return false;
	CentralDirectoryEnd end;
	if (!readCentralDirectoryEnd(input, end))
		return false;
	input->seek(end.cdir_offset, RVNG_SEEK_SET);
	while (!input->isEnd() && (unsigned)input->tell() < end.cdir_offset + end.cdir_size)
	{
		if (!readCentralDirectoryEntry(input, entry))
			return false;
		if (name_size == entry.filename_size && entry.filename == name)
			break;
	}
	if (name_size != entry.filename_size)
		return false;
	if (entry.filename != name)
		return false;
	input->seek(entry.offset, RVNG_SEEK_SET);
	LocalFileHeader header;
	if (!readLocalFileHeader(input, header))
		return false;
	if (!areHeadersConsistent(header, entry))
		return false;
	return true;
}

static std::vector<std::string> getSubStreamNamesInZip(RVNGInputStream *input, bool all=false)
{
	std::vector<std::string> res;
	if (!input || !findCentralDirectoryEnd(input))
		return res;
	CentralDirectoryEnd end;
	if (!readCentralDirectoryEnd(input, end))
		return res;
	input->seek(long(end.cdir_offset), RVNG_SEEK_SET);
	while (!input->isEnd() && (unsigned)input->tell() < end.cdir_offset + end.cdir_size)
	{
		CentralDirectoryEntry entry;
		if (!readCentralDirectoryEntry(input, entry))
			break;
		if (!entry.filename_size || (!all && entry.filename[entry.filename.size()-1]=='/')) continue;
		// A file name with a \0 char in it is likely broken. Anyway, our char*-based
		// interface cannot handle such names, so we just ignore them.
		if (entry.filename.find('\0') != std::string::npos)
			continue;
		res.push_back(entry.filename);
	}
	return res;
}
} // anonymous namespace

bool RVNGZipStream::isZipFile(RVNGInputStream *input)
{
	// look for central directory end
	if (!findCentralDirectoryEnd(input))
		return false;
	CentralDirectoryEnd end;
	if (!readCentralDirectoryEnd(input, end))
		return false;
	input->seek(end.cdir_offset, RVNG_SEEK_SET);
	// read first entry in the central directory
	CentralDirectoryEntry entry;
	if (!readCentralDirectoryEntry(input, entry))
		return false;
	input->seek(entry.offset, RVNG_SEEK_SET);
	// read the local file header and compare with the central directory information
	LocalFileHeader header;
	if (!readLocalFileHeader(input, header))
		return false;
	if (!areHeadersConsistent(header, entry))
		return false;
	return true;
}

std::vector<std::string> RVNGZipStream::getSubStreamNamesList(RVNGInputStream *input)
{
	return getSubStreamNamesInZip(input,false);
}

RVNGInputStream *RVNGZipStream::getSubstream(RVNGInputStream *input, const char *name)
{
	CentralDirectoryEntry entry;
	if (!findDataStream(input, entry, name))
		return 0;
	if (!entry.compressed_size)
		return 0;
	unsigned long numBytesRead = 0;
	unsigned char *compressedData = const_cast<unsigned char *>(input->read(entry.compressed_size, numBytesRead));
	if (numBytesRead != entry.compressed_size)
		return 0;
	if (!entry.compression)
		return new RVNGStringStream(compressedData, (unsigned)numBytesRead);
	else
	{
		int ret;
		z_stream strm;

		/* allocate inflate state */
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		ret = inflateInit2(&strm,-MAX_WBITS);
		if (ret != Z_OK)
			return 0;

		strm.avail_in = (unsigned)numBytesRead;
		strm.next_in = (Bytef *)compressedData;

		std::vector<unsigned char>data(entry.uncompressed_size);

		strm.avail_out = entry.uncompressed_size;
		strm.next_out = reinterpret_cast<Bytef *>(&data[0]);
		ret = inflate(&strm, Z_FINISH);
		switch (ret)
		{
		case Z_NEED_DICT:
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			(void)inflateEnd(&strm);
			data.clear();
			return 0;
		default:
			break;
		}
		(void)inflateEnd(&strm);
		return new RVNGStringStream(&data[0], (unsigned)data.size());
	}
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
