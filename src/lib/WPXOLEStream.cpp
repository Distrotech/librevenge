/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* POLE - Portable C++ library to access OLE Storage
   Copyright (C) 2002-2005 Ariya Hidayat <ariya@kde.org>

   Performance optimization: Dmitry Fedorov
   Copyright 2009 <www.bioimage.ucsb.edu> <www.dimin.net>

   Fix for more than 236 mbat block entries : Michel Boudinot
   Copyright 2010 <Michel.Boudinot@inaf.cnrs-gif.fr>

   Version: 0.4

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   * Neither the name of the authors nor the names of its contributors may be
     used to endorse or promote products derived from this software without
     specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
   THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 This file taken from libwpg WPGOLEStream.cpp 1.5 Thu Aug 17 21:21:30 2006
*/

#include <sstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "WPXOLEStream.h"
#include "WPXStream.h"
#include <string.h>

namespace libwpd
{
enum { Avail = 0xffffffff, Eof = 0xfffffffe, Bat = 0xfffffffd, MetaBat = 0xfffffffc, NotFound=0xfffffff0 };

class Header
{
public:
	unsigned char m_magic[8];       // signature, or magic identifier
	unsigned m_revision;         // the file revision
	unsigned m_num_bat;          // blocks allocated for big bat
	unsigned m_start_dirent;     // starting block for directory info
	unsigned m_threshold;        // switch from small to big file (usually 4K)
	unsigned m_start_sbat;       // starting block index to store small bat
	unsigned m_num_sbat;         // blocks allocated for small bat
	unsigned m_shift_sbat;          // sbat->blockSize = 1 << m_shift_sbat
	unsigned m_size_sbat;		// the small block size, default 0x40
	unsigned m_shift_bbat;          // bbat->blockSize = 1 << m_shift_bbat
	unsigned m_size_bbat;		// the big block size, default 0x200
	unsigned m_start_mbat;       // starting block to store meta bat
	unsigned m_num_mbat;         // blocks allocated for meta bat
	unsigned long m_blocks_bbat[109];

	Header();
	void compute_block_size()
	{
		m_size_bbat = 1 << m_shift_bbat;
		m_size_sbat = 1 << m_shift_sbat;
	}
	bool valid_signature() const
	{
		for( unsigned i = 0; i < 8; i++ )
			if (m_magic[i] != s_ole_magic[i]) return false;
		return true;
	}
	bool valid();
	void load( const unsigned char *buffer, unsigned long size );
	void save( unsigned char *buffer );
protected:
	static const unsigned char s_ole_magic[];
};

const unsigned char Header::s_ole_magic[] =
{ 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1 };

class AllocTable
{
public:
	unsigned blockSize;
	AllocTable();
	void clear();
	unsigned long count();
	void resize( unsigned long newsize );
	void set( unsigned long index, unsigned long val );
	std::vector<unsigned long> follow( unsigned long start );
	unsigned long operator[](unsigned long index );
	void load( const unsigned char *buffer, unsigned len );
private:
	std::vector<unsigned long> data;
	AllocTable( const AllocTable & );
	AllocTable &operator=( const AllocTable & );
};

class DirEntry
{
public:
	DirEntry() : valid(false), name(), dir(false), size(0), start(0),
		prev(0), next(0), child(0) {}
	bool valid;            // false if invalid (should be skipped)
	std::string name;      // the name, not in unicode anymore
	bool dir;              // true if directory
	unsigned long size;    // size (not valid if directory)
	unsigned long start;   // starting block
	unsigned prev;         // previous sibling
	unsigned next;         // next sibling
	unsigned child;        // first child
};

class DirTree
{
public:
	static const unsigned End;
	DirTree();
	void clear();
	unsigned entryCount();
	DirEntry *entry( unsigned index );
	DirEntry *entry( const std::string &name );
	unsigned find_child( unsigned index, const std::string &name );
	void load( unsigned char *buffer, unsigned len );
private:
	std::vector<DirEntry> entries;
	DirTree( const DirTree & );
	DirTree &operator=( const DirTree & );
};

class StorageIO
{
public:
	Storage *storage;         // owner
	WPXInputStream *input;
	int result;               // result of operation

	Header m_header;           // storage header
	DirTree *dirtree;         // directory tree
	AllocTable *bbat;         // allocation table for big blocks
	AllocTable *sbat;         // allocation table for small blocks

	std::vector<unsigned long> sb_blocks; // blocks for "small" files

	StorageIO( Storage *storage, WPXInputStream *is );
	~StorageIO();

	bool isOLEStream();
	void load();

	unsigned long loadBigBlocks( std::vector<unsigned long> blocks, unsigned char *buffer, unsigned long maxlen );

	unsigned long loadBigBlock( unsigned long block, unsigned char *buffer, unsigned long maxlen );

	unsigned long loadSmallBlocks( std::vector<unsigned long> blocks, unsigned char *buffer, unsigned long maxlen );

	unsigned long loadSmallBlock( unsigned long block, unsigned char *buffer, unsigned long maxlen );

	StreamIO *streamIO( const std::string &name );

private:
	// no copy or assign
	StorageIO( const StorageIO & );
	StorageIO &operator=( const StorageIO & );

};

class StreamIO
{
public:
	StorageIO *io;
	DirEntry *entry;
	std::string fullName;
	bool eof;
	bool fail;

	StreamIO( StorageIO *io, DirEntry *entry );
	~StreamIO();
	unsigned long size();
	unsigned long tell();
	unsigned long read( unsigned char *data, unsigned long maxlen );
	unsigned long read( unsigned long pos, unsigned char *data, unsigned long maxlen );


private:
	std::vector<unsigned long> blocks;

	// no copy or assign
	StreamIO( const StreamIO & );
	StreamIO &operator=( const StreamIO & );

	// pointer for read
	unsigned long m_pos;

	// simple cache system to speed-up getch()
	std::vector<unsigned char> cache_data;
	unsigned long cache_size;
	unsigned long cache_pos;
	void updateCache();
};

} // namespace libwpd

static inline unsigned short readU16( const unsigned char *ptr )
{
	return (unsigned short)(ptr[0]+(ptr[1]<<8));
}

static inline unsigned readU32( const unsigned char *ptr )
{
	return (unsigned)(ptr[0]+(ptr[1]<<8)+(ptr[2]<<16)+(ptr[3]<<24));
}

static inline void writeU16( unsigned char *ptr, unsigned long data )
{
	ptr[0] = (unsigned char)(data & 0xff);
	ptr[1] = (unsigned char)((data >> 8) & 0xff);
}

static inline void writeU32( unsigned char *ptr, unsigned long data )
{
	ptr[0] = (unsigned char)(data & 0xff);
	ptr[1] = (unsigned char)((data >> 8) & 0xff);
	ptr[2] = (unsigned char)((data >> 16) & 0xff);
	ptr[3] = (unsigned char)((data >> 24) & 0xff);
}

// =========== Header ==========

libwpd::Header::Header() :
	m_revision(0x3e), m_num_bat(0), m_start_dirent(0), m_threshold(4096),
	m_start_sbat(Eof), m_num_sbat(0), m_shift_sbat(6), m_size_sbat(0),
	m_shift_bbat(9), m_size_bbat(0),
	m_start_mbat(Eof), m_num_mbat(0)
{
	for( unsigned i = 0; i < 8; i++ )
		m_magic[i] = s_ole_magic[i];
	for( unsigned j=0; j<109; j++ )
		m_blocks_bbat[j] = Avail;
	compute_block_size();
}

bool libwpd::Header::valid()
{
	if( m_threshold != 4096 ) return false;
	if( m_num_bat == 0 ) return false;
	if( (m_num_bat > 109) && (m_num_bat > (m_num_mbat * (m_size_bbat/4-1)) + 109)) return false;
	if( (m_num_bat < 109) && (m_num_mbat != 0) ) return false;
	if( m_shift_sbat > m_shift_bbat ) return false;
	if( m_shift_bbat <= 6 ) return false;
	if( m_shift_bbat >=31 ) return false;

	return true;
}

void libwpd::Header::load( const unsigned char *buffer, unsigned long size )
{
	if (size < 512)
		return;
	m_revision = (unsigned) readU16(buffer+0x18);
	m_shift_bbat      = (unsigned int) readU16( buffer + 0x1e );
	m_shift_sbat      = (unsigned int) readU16( buffer + 0x20 );
	m_num_bat      = (unsigned int) readU32( buffer + 0x2c );
	m_start_dirent = (unsigned int) readU32( buffer + 0x30 );
	m_threshold    = (unsigned int) readU32( buffer + 0x38 );
	m_start_sbat   = (unsigned int) readU32( buffer + 0x3c );
	m_num_sbat     = (unsigned int) readU32( buffer + 0x40 );
	m_start_mbat   = (unsigned int) readU32( buffer + 0x44 );
	m_num_mbat     = (unsigned int) readU32( buffer + 0x48 );

	for( unsigned i = 0; i < 8; i++ )
		m_magic[i] = buffer[i];
	for( unsigned j=0; j<109; j++ )
		m_blocks_bbat[j] = readU32( buffer + 0x4C+j*4 );
	compute_block_size();
}

void libwpd::Header::save( unsigned char *buffer )
{
	memset( buffer, 0, 0x4c );
	memcpy( buffer, s_ole_magic, 8 );        // ole signature
	writeU32( buffer + 8, 0 );              // unknown
	writeU32( buffer + 12, 0 );             // unknown
	writeU32( buffer + 16, 0 );             // unknown
	writeU16( buffer + 24, m_revision);
	writeU16( buffer + 26, 3 );             // version ?
	writeU16( buffer + 28, 0xfffe );        // unknown
	writeU16( buffer + 0x1e, m_shift_bbat );
	writeU16( buffer + 0x20, m_shift_sbat );
	writeU32( buffer + 0x2c, m_num_bat );
	writeU32( buffer + 0x30, m_start_dirent );
	writeU32( buffer + 0x38, m_threshold );
	writeU32( buffer + 0x3c, m_start_sbat );
	writeU32( buffer + 0x40, m_num_sbat );
	writeU32( buffer + 0x44, m_start_mbat );
	writeU32( buffer + 0x48, m_num_mbat );

	for( unsigned i=0; i<109; i++ )
		writeU32( buffer + 0x4C+i*4, m_blocks_bbat[i] );
}


// =========== AllocTable ==========


libwpd::AllocTable::AllocTable() :
	blockSize(4096),
	data()
{
	// initial size
	resize( 128 );
}

unsigned long libwpd::AllocTable::count()
{
	return data.size();
}

void libwpd::AllocTable::resize( unsigned long newsize )
{
	size_t oldsize = data.size();
	data.resize( newsize );
	if( newsize > oldsize )
		for( size_t i = oldsize; i<newsize; i++ )
			data[i] = Avail;
}

unsigned long libwpd::AllocTable::operator[]( unsigned long index )
{
	unsigned long result;
	result = data[index];
	return result;
}

void libwpd::AllocTable::set( unsigned long index, unsigned long value )
{
	if( index >= count() ) resize( index + 1);
	data[ index ] = value;
}

// TODO: optimize this with better search
static bool already_exist(const std::vector<unsigned long>& chain,
unsigned long item)
{
	for(unsigned i = 0; i < chain.size(); i++)
		if(chain[i] == item) return true;

	return false;
}

// follow
std::vector<unsigned long> libwpd::AllocTable::follow( unsigned long start )
{
	std::vector<unsigned long> chain;

	if( start >= count() ) return chain;

	unsigned long p = start;
	while( p < count() )
	{
		if( p == (unsigned long)Eof ) break;
		if( p == (unsigned long)Bat ) break;
		if( p == (unsigned long)MetaBat ) break;
		if( already_exist(chain, p) ) break;
		chain.push_back( p );
		if( data[p] >= count() ) break;
		p = data[ p ];
	}

	return chain;
}

void libwpd::AllocTable::load( const unsigned char *buffer, unsigned len )
{
	resize( len / 4 );
	for( unsigned i = 0; i < count(); i++ )
		set( i, readU32( buffer + i*4 ) );
}

// =========== DirTree ==========

const unsigned libwpd::DirTree::End = 0xffffffff;

libwpd::DirTree::DirTree() :
	entries()
{
	clear();
}

void libwpd::DirTree::clear()
{
	// leave only root entry
	entries.resize( 1 );
	entries[0].valid = true;
	entries[0].name = "Root Entry";
	entries[0].dir = true;
	entries[0].size = 0;
	entries[0].start = End;
	entries[0].prev = End;
	entries[0].next = End;
	entries[0].child = End;
}

unsigned libwpd::DirTree::entryCount()
{
	return (unsigned)entries.size();
}

libwpd::DirEntry *libwpd::DirTree::entry( unsigned index )
{
	if( index >= entryCount() ) return (libwpd::DirEntry *) 0;
	return &entries[ index ];
}

// given a fullname (e.g "/ObjectPool/_1020961869"), find the entry
libwpd::DirEntry *libwpd::DirTree::entry( const std::string &name )
{

	if( !name.length() ) return (libwpd::DirEntry *)0;

	// quick check for "/" (that's root)
	if( name == "/" ) return entry( 0 );

	// split the names, e.g  "/ObjectPool/_1020961869" will become:
	// "ObjectPool" and "_1020961869"
	std::list<std::string> names;
	std::string::size_type start = 0, end = 0;
	if( name[0] == '/' ) start++;
	while( start < name.length() )
	{
		end = name.find_first_of( '/', start );
		if( end == std::string::npos ) end = name.length();
		names.push_back( name.substr( start, end-start ) );
		start = end+1;
	}

	// start from root
	int index = 0 ;

	// trace one by one
	std::list<std::string>::iterator it;

	for( it = names.begin(); it != names.end(); ++it )
	{
		// dima: performace optimisation of the previous
		unsigned child = find_child( index, *it );
		// traverse to the child
		if( child > 0 ) index = child;
		else return (libwpd::DirEntry *)0;
	}

	return entry( index );
}

static unsigned dirtree_find_sibling( libwpd::DirTree *dirtree, unsigned index, const std::string &name )
{

	unsigned count = dirtree->entryCount();
	libwpd::DirEntry *e = dirtree->entry( index );
	if (!e || !e->valid) return 0;
	if (e->name == name) return index;

	if (e->next>0 && e->next<count)
	{
		unsigned r = dirtree_find_sibling( dirtree, e->next, name );
		if (r>0) return r;
	}

	if (e->prev>0 && e->prev<count)
	{
		unsigned r = dirtree_find_sibling( dirtree, e->prev, name );
		if (r>0) return r;
	}

	return 0;
}

unsigned libwpd::DirTree::find_child( unsigned index, const std::string &name )
{

	unsigned count = entryCount();
	libwpd::DirEntry *p = entry( index );
	if (p && p->valid && p->child < count )
		return dirtree_find_sibling( this, p->child, name );

	return 0;
}

void libwpd::DirTree::load( unsigned char *buffer, unsigned size )
{
	entries.clear();

	for( unsigned i = 0; i < size/128; i++ )
	{
		unsigned p = i * 128;

		// parse name of this entry, which stored as Unicode 16-bit
		std::string name;
		int name_len = readU16( buffer + 0x40+p );
		if( name_len > 64 ) name_len = 64;
		for( int j=0; ( buffer[j+p]) && (j<name_len); j+= 2 )
			name.append( 1, buffer[j+p] );

		// would be < 32 if first char in the name isn't printable
		// first char isn't printable ? remove it...
		if( buffer[p] < 32 )
			name.erase( 0,1 );

		// 2 = file (aka stream), 1 = directory (aka storage), 5 = root
		unsigned type = buffer[ 0x42 + p];

		libwpd::DirEntry e;
		e.valid = true;
		e.name = name;
		e.start = readU32( buffer + 0x74+p );
		e.size = readU32( buffer + 0x78+p );
		e.prev = readU32( buffer + 0x44+p );
		e.next = readU32( buffer + 0x48+p );
		e.child = readU32( buffer + 0x4C+p );
		e.dir = ( type!=2 );

		// sanity checks
		if( (type != 2) && (type != 1 ) && (type != 5 ) ) e.valid = false;
		if( name_len < 1 ) e.valid = false;

		entries.push_back( e );
	}
}

// =========== StorageIO ==========

libwpd::StorageIO::StorageIO( libwpd::Storage *st, WPXInputStream *is ) :
	storage(st),
	input( is ),
	result(libwpd::Storage::Ok),
	m_header(),
	dirtree(new libwpd::DirTree()),
	bbat(new libwpd::AllocTable()),
	sbat(new libwpd::AllocTable()),
	sb_blocks()
{
	bbat->blockSize = 1 << m_header.m_shift_bbat;
	sbat->blockSize = 1 << m_header.m_shift_sbat;
}

libwpd::StorageIO::~StorageIO()
{
	delete sbat;
	delete bbat;
	delete dirtree;
}

bool libwpd::StorageIO::isOLEStream()
{
	load();
	return (result == libwpd::Storage::Ok);
}

void libwpd::StorageIO::load()
{
	std::vector<unsigned long> blocks;

	// load header
	unsigned long numBytesRead = 0;
	const unsigned char *buf = input->read(512, numBytesRead);

	result = libwpd::Storage::NotOLE;
	if (numBytesRead < 512)
		return;

	m_header.load( buf, numBytesRead );

	// check OLE magic id
	if (!m_header.valid_signature())
		return;

	// sanity checks
	result = libwpd::Storage::BadOLE;
	if( !m_header.valid() ) return;
	if( m_header.m_threshold != 4096 ) return;

	// important block size
	bbat->blockSize = m_header.m_size_bbat;
	sbat->blockSize = m_header.m_size_sbat;

	// find blocks allocated to store big bat
	// the first 109 blocks are in header, the rest in meta bat
	blocks.clear();
	blocks.resize( m_header.m_num_bat );
	for( unsigned j = 0; j < 109; j++ )
		if( j >= m_header.m_num_bat ) break;
		else blocks[j] = m_header.m_blocks_bbat[j];
	if( (m_header.m_num_bat > 109) && (m_header.m_num_mbat > 0) )
	{
		std::vector<unsigned char> buffer2( bbat->blockSize );
		unsigned k = 109;
		unsigned long sector;
		for( unsigned r = 0; r < m_header.m_num_mbat; r++ )
		{
			if(r == 0) // 1st meta bat location is in file header.
				sector = m_header.m_start_mbat;
			else      // next meta bat location is the last current block value.
				sector = blocks[--k];
			loadBigBlock( sector, &buffer2[0], bbat->blockSize );
			for( unsigned s=0; s < bbat->blockSize; s+=4 )
			{
				if( k >= m_header.m_num_bat ) break;
				else  blocks[k++] = readU32( &buffer2[s] );
			}
		}
	}

	// load big bat
	if( blocks.size()*bbat->blockSize > 0 )
	{
		std::vector<unsigned char> buffer( blocks.size()*bbat->blockSize );
		loadBigBlocks( blocks, &buffer[0], buffer.size() );
		bbat->load( &buffer[0], (unsigned)buffer.size() );
	}

	// load small bat
	blocks.clear();
	blocks = bbat->follow( m_header.m_start_sbat );
	if( blocks.size()*bbat->blockSize > 0 )
	{
		std::vector<unsigned char> buffer( blocks.size()*bbat->blockSize );
		loadBigBlocks( blocks, &buffer[0], buffer.size() );
		sbat->load( &buffer[0], (unsigned)buffer.size() );
	}

	// load directory tree
	blocks.clear();
	blocks = bbat->follow( m_header.m_start_dirent );
	std::vector<unsigned char> buffer(blocks.size()*bbat->blockSize);
	loadBigBlocks( blocks, &buffer[0], buffer.size() );
	dirtree->load( &buffer[0], (unsigned)buffer.size() );
	unsigned sb_start = readU32( &buffer[0x74] );

	// fetch block chain as data for small-files
	sb_blocks = bbat->follow( sb_start ); // small files

	// so far so good
	result = libwpd::Storage::Ok;
}

libwpd::StreamIO *libwpd::StorageIO::streamIO( const std::string &name )
{
	load();

	// sanity check
	if( !name.length() ) return (libwpd::StreamIO *)0;

	// search in the entries
	libwpd::DirEntry *entry = dirtree->entry( name );
	if( !entry ) return (libwpd::StreamIO *)0;
	if( entry->dir ) return (libwpd::StreamIO *)0;

	libwpd::StreamIO *res = new libwpd::StreamIO( this, entry );
	res->fullName = name;

	return res;
}

unsigned long libwpd::StorageIO::loadBigBlocks( std::vector<unsigned long> blocks,
unsigned char *data, unsigned long maxlen )
{
	// sentinel
	if( !data ) return 0;
	if( blocks.size() < 1 ) return 0;
	if( maxlen == 0 ) return 0;

	// read block one by one, seems fast enough
	unsigned long bytes = 0;
	for( unsigned long i=0; (i < blocks.size() ) & ( bytes<maxlen ); i++ )
	{
		unsigned long block = blocks[i];
		unsigned long pos =  bbat->blockSize * ( block+1 );
		unsigned long p = (bbat->blockSize < maxlen-bytes) ? bbat->blockSize : maxlen-bytes;

		input->seek(pos, WPX_SEEK_SET);
		unsigned long numBytesRead = 0;
		const unsigned char *buf = input->read(p, numBytesRead);
		memcpy(data+bytes, buf, numBytesRead);
		bytes += numBytesRead;
	}

	return bytes;
}

unsigned long libwpd::StorageIO::loadBigBlock( unsigned long block,
unsigned char *data, unsigned long maxlen )
{
	// sentinel
	if( !data ) return 0;

	// wraps call for loadBigBlocks
	std::vector<unsigned long> blocks;
	blocks.resize( 1 );
	blocks[ 0 ] = block;

	return loadBigBlocks( blocks, data, maxlen );
}

// return number of bytes which has been read
unsigned long libwpd::StorageIO::loadSmallBlocks( std::vector<unsigned long> blocks,
unsigned char *data, unsigned long maxlen )
{
	// sentinel
	if( !data ) return 0;
	if( blocks.size() < 1 ) return 0;
	if( maxlen == 0 ) return 0;

	// our own local buffer
	std::vector<unsigned char> tmpBuf( bbat->blockSize );

	// read small block one by one
	unsigned long bytes = 0;
	for( unsigned long i=0; ( i<blocks.size() ) & ( bytes<maxlen ); i++ )
	{
		unsigned long block = blocks[i];

		// find where the small-block exactly is
		unsigned long pos = block * sbat->blockSize;
		unsigned long bbindex = pos / bbat->blockSize;
		if( bbindex >= sb_blocks.size() ) break;

		loadBigBlock( sb_blocks[ bbindex ], &tmpBuf[0], bbat->blockSize );

		// copy the data
		unsigned long offset = pos % bbat->blockSize;
		unsigned long p = (maxlen-bytes < bbat->blockSize-offset ) ? maxlen-bytes :  bbat->blockSize-offset;
		p = (sbat->blockSize<p ) ? sbat->blockSize : p;
		memcpy( data + bytes, &tmpBuf[offset], p );
		bytes += p;
	}

	return bytes;
}

unsigned long libwpd::StorageIO::loadSmallBlock( unsigned long block,
unsigned char *data, unsigned long maxlen )
{
	// sentinel
	if( !data ) return 0;

	// wraps call for loadSmallBlocks
	std::vector<unsigned long> blocks;
	blocks.resize( 1 );
	blocks.assign( 1, block );

	return loadSmallBlocks( blocks, data, maxlen );
}

// =========== StreamIO ==========

libwpd::StreamIO::StreamIO( libwpd::StorageIO *s, libwpd::DirEntry *e) :
	io(s),
	entry(e),
	fullName(),
	eof(false),
	fail(false),
	blocks(),
	m_pos(0),
	cache_data(),
	cache_size(4096),
	cache_pos(0)
{
	if( entry->size >= io->m_header.m_threshold )
		blocks = io->bbat->follow( entry->start );
	else
		blocks = io->sbat->follow( entry->start );

	// prepare cache
	cache_data = std::vector<unsigned char>(cache_size);
	updateCache();
}

// FIXME tell parent we're gone
libwpd::StreamIO::~StreamIO()
{
}

unsigned long libwpd::StreamIO::tell()
{
	return m_pos;
}

unsigned long libwpd::StreamIO::read( unsigned long pos, unsigned char *data, unsigned long maxlen )
{
	// sanity checks
	if( !data ) return 0;
	if( maxlen == 0 ) return 0;

	unsigned long totalbytes = 0;

	if ( entry->size < io->m_header.m_threshold )
	{
		// small file
		unsigned long index = pos / io->sbat->blockSize;

		if( index >= blocks.size() ) return 0;

		std::vector<unsigned char> buf( io->sbat->blockSize );
		unsigned long offset = pos % io->sbat->blockSize;
		while( totalbytes < maxlen )
		{
			if( index >= blocks.size() ) break;
			io->loadSmallBlock( blocks[index], &buf[0], io->bbat->blockSize );
			unsigned long count = io->sbat->blockSize - offset;
			if( count > maxlen-totalbytes ) count = maxlen-totalbytes;
			memcpy( data+totalbytes, &buf[offset], count );
			totalbytes += count;
			offset = 0;
			index++;
		}
	}
	else
	{
		// big file
		unsigned long index = pos / io->bbat->blockSize;

		if( index >= blocks.size() ) return 0;

		std::vector<unsigned char> buf( io->bbat->blockSize );
		unsigned long offset = pos % io->bbat->blockSize;
		while( totalbytes < maxlen )
		{
			if( index >= blocks.size() ) break;
			io->loadBigBlock( blocks[index], &buf[0], io->bbat->blockSize );
			unsigned long count = io->bbat->blockSize - offset;
			if( count > maxlen-totalbytes ) count = maxlen-totalbytes;
			memcpy( data+totalbytes, &buf[offset], count );
			totalbytes += count;
			index++;
			offset = 0;
		}
	}

	return totalbytes;
}

unsigned long libwpd::StreamIO::read( unsigned char *data, unsigned long maxlen )
{
	unsigned long bytes = read( tell(), data, maxlen );
	m_pos += bytes;
	return bytes;
}

void libwpd::StreamIO::updateCache()
{
	// sanity check
	if( cache_data.empty() ) return;

	cache_pos = m_pos - ( m_pos % cache_size );
	unsigned long bytes = cache_size;
	if( cache_pos + bytes > entry->size ) bytes = entry->size - cache_pos;
	cache_size = read( cache_pos, &cache_data[0], bytes );
}


// =========== Storage ==========

libwpd::Storage::Storage( WPXInputStream *is ) :
	io(0)
{
	io = new StorageIO( this, is );
}

libwpd::Storage::~Storage()
{
	delete io;
}

int libwpd::Storage::result()
{
	return io->result;
}

bool libwpd::Storage::isOLEStream()
{
	return io->isOLEStream();
}

// =========== Stream ==========

libwpd::Stream::Stream( libwpd::Storage *storage, const std::string &name ) :
	io(storage->io->streamIO( name ))
{
}

// FIXME tell parent we're gone
libwpd::Stream::~Stream()
{
	delete io;
}

unsigned long libwpd::Stream::size()
{
	return io ? io->entry->size : 0;
}

unsigned long libwpd::Stream::read( unsigned char *data, unsigned long maxlen )
{
	return io ? io->read( data, maxlen ) : 0;
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
