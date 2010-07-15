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
#include <string.h>

namespace libwpd
{

class Header
{
  public:
    unsigned char id[8];       // signature, or magic identifier
    unsigned b_shift;          // bbat->blockSize = 1 << b_shift
    unsigned s_shift;          // sbat->blockSize = 1 << s_shift
    unsigned num_bat;          // blocks allocated for big bat
    unsigned dirent_start;     // starting block for directory info
    unsigned threshold;        // switch from small to big file (usually 4K)
    unsigned sbat_start;       // starting block index to store small bat
    unsigned num_sbat;         // blocks allocated for small bat
    unsigned mbat_start;       // starting block to store meta bat
    unsigned num_mbat;         // blocks allocated for meta bat
    unsigned long bb_blocks[109];
    
    Header();
    bool valid();
    void load( const unsigned char* buffer );
    void save( unsigned char* buffer );
    void debug();
};

class AllocTable
{
  public:
    static const unsigned Eof;
    static const unsigned Avail;
    static const unsigned Bat;    
    static const unsigned MetaBat;    
    unsigned blockSize;
    AllocTable();
    void clear();
    unsigned long count();
    void resize( unsigned long newsize );
    void preserve( unsigned long n );
    void set( unsigned long index, unsigned long val );
    unsigned unused();
    void setChain( std::vector<unsigned long> );
    std::vector<unsigned long> follow( unsigned long start );
    unsigned long operator[](unsigned long index );
    void load( const unsigned char* buffer, unsigned len );
    void save( unsigned char* buffer );
  private:
    std::vector<unsigned long> data;
    AllocTable( const AllocTable& );
    AllocTable& operator=( const AllocTable& );
};

class DirEntry
{
  public:
    DirEntry() : valid(false), name(), dir(false), size(0), start(0),
    	prev(0), next(0), child(0) {};
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
    DirEntry* entry( unsigned index );
    DirEntry* entry( const std::string& name );
    int parent( unsigned index );
    std::string fullName( unsigned index );
    std::vector<unsigned> children( unsigned index );
  unsigned find_child( unsigned index, const std::string& name );
    void load( unsigned char* buffer, unsigned len );
    void save( unsigned char* buffer );
  private:
    std::vector<DirEntry> entries;
    DirTree( const DirTree& );
    DirTree& operator=( const DirTree& );
};

class StorageIO
{
  public:
    Storage* storage;         // owner
    std::stringstream buf;
    int result;               // result of operation
    unsigned long bufsize;    // size of the buffer
    
    Header* header;           // storage header 
    DirTree* dirtree;         // directory tree
    AllocTable* bbat;         // allocation table for big blocks
    AllocTable* sbat;         // allocation table for small blocks
    
    std::vector<unsigned long> sb_blocks; // blocks for "small" files
       
    std::list<Stream*> streams;

    StorageIO( Storage* storage, const std::stringstream &memorystream );
    ~StorageIO();
    
    bool isOLEStream();
    void load();

    unsigned long loadBigBlocks( std::vector<unsigned long> blocks, unsigned char* buffer, unsigned long maxlen );

    unsigned long loadBigBlock( unsigned long block, unsigned char* buffer, unsigned long maxlen );

    unsigned long loadSmallBlocks( std::vector<unsigned long> blocks, unsigned char* buffer, unsigned long maxlen );

    unsigned long loadSmallBlock( unsigned long block, unsigned char* buffer, unsigned long maxlen );
    
    StreamIO* streamIO( const std::string& name ); 

  private:  
    // no copy or assign
    StorageIO( const StorageIO& );
    StorageIO& operator=( const StorageIO& );

};

class StreamIO
{
  public:
    StorageIO* io;
    DirEntry* entry;
    std::string fullName;
    bool eof;
    bool fail;

    StreamIO( StorageIO* io, DirEntry* entry );
    ~StreamIO();
    unsigned long size();
    unsigned long tell();
    int getch();
    unsigned long read( unsigned char* data, unsigned long maxlen );
    unsigned long read( unsigned long pos, unsigned char* data, unsigned long maxlen );


  private:
    std::vector<unsigned long> blocks;

    // no copy or assign
    StreamIO( const StreamIO& );
    StreamIO& operator=( const StreamIO& );

    // pointer for read
    unsigned long m_pos;

    // simple cache system to speed-up getch()
    unsigned char* cache_data;
    unsigned long cache_size;
    unsigned long cache_pos;
    void updateCache();
};

} // namespace libwpd

static inline unsigned long readU16( const unsigned char* ptr )
{
  return ptr[0]+(ptr[1]<<8);
}

static inline unsigned long readU32( const unsigned char* ptr )
{
  return ptr[0]+(ptr[1]<<8)+(ptr[2]<<16)+(ptr[3]<<24);
}

static const unsigned char wpsole_magic[] =
 { 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1 };


// =========== Header ==========

libwpd::Header::Header() :
	b_shift(9),
	s_shift(6),
	num_bat(0),
	dirent_start(0),
	threshold(4096),
	sbat_start(0),
	num_sbat(0),
	mbat_start(0),
	num_mbat(0)
{
  for( unsigned i = 0; i < 8; i++ )
    id[i] = wpsole_magic[i];
  for( unsigned j=0; j<109; j++ )
    bb_blocks[j] = libwpd::AllocTable::Avail;
}

bool libwpd::Header::valid()
{
  if( threshold != 4096 ) return false;
  if( num_bat == 0 ) return false;
  if( (num_bat > 109) && (num_bat > (num_mbat * 127) + 109)) return false;
  if( (num_bat < 109) && (num_mbat != 0) ) return false;
  if( s_shift > b_shift ) return false;
  if( b_shift <= 6 ) return false;
  if( b_shift >=31 ) return false;

  return true;
}

void libwpd::Header::load( const unsigned char* buffer )
{
  b_shift      = readU16( buffer + 0x1e );
  s_shift      = readU16( buffer + 0x20 );
  num_bat      = readU32( buffer + 0x2c );
  dirent_start = readU32( buffer + 0x30 );
  threshold    = readU32( buffer + 0x38 );
  sbat_start   = readU32( buffer + 0x3c );
  num_sbat     = readU32( buffer + 0x40 );
  mbat_start   = readU32( buffer + 0x44 );
  num_mbat     = readU32( buffer + 0x48 );

  for( unsigned i = 0; i < 8; i++ )
    id[i] = buffer[i];
  for( unsigned j=0; j<109; j++ )
    bb_blocks[j] = readU32( buffer + 0x4C+j*4 );
}


 
// =========== AllocTable ==========

const unsigned libwpd::AllocTable::Avail = 0xffffffff;
const unsigned libwpd::AllocTable::Eof = 0xfffffffe;
const unsigned libwpd::AllocTable::Bat = 0xfffffffd;
const unsigned libwpd::AllocTable::MetaBat = 0xfffffffc;

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
  unsigned oldsize = data.size();
  data.resize( newsize );
  if( newsize > oldsize )
    for( unsigned i = oldsize; i<newsize; i++ )
      data[i] = Avail;
}

// make sure there're still free blocks
void libwpd::AllocTable::preserve( unsigned long n )
{
  std::vector<unsigned long> pre;
  for( unsigned i=0; i < n; i++ )
    pre.push_back( unused() );
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

void libwpd::AllocTable::setChain( std::vector<unsigned long> chain )
{
  if( chain.size() )
  {
    for( unsigned i=0; i<chain.size()-1; i++ )
      set( chain[i], chain[i+1] );
    set( chain[ chain.size()-1 ], AllocTable::Eof );
  }
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

unsigned libwpd::AllocTable::unused()
{
  // find first available block
  for( unsigned i = 0; i < data.size(); i++ )
    if( data[i] == Avail )
      return i;
  
  // completely full, so enlarge the table
  unsigned block = data.size();
  resize( data.size()+10 );
  return block;      
}

void libwpd::AllocTable::load( const unsigned char* buffer, unsigned len )
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
  return entries.size();
}

libwpd::DirEntry* libwpd::DirTree::entry( unsigned index )
{
  if( index >= entryCount() ) return (libwpd::DirEntry*) 0;
  return &entries[ index ];
}

int libwpd::DirTree::parent( unsigned index )
{
  // brute-force, basically we iterate for each entries, find its children
  // and check if one of the children is 'index'
  for( unsigned j=0; j<entryCount(); j++ )
  {
    std::vector<unsigned> chi = children( j );
    for( unsigned i=0; i<chi.size();i++ )
      if( chi[i] == index )
        return j;
  }
        
  return -1;
}

std::string libwpd::DirTree::fullName( unsigned index )
{
  // don't use root name ("Root Entry"), just give "/"
  if( index == 0 ) return "/";

  std::string result = entry( index )->name;
  result.insert( 0,  "/" );
  int p = parent( index );
  DirEntry * _entry = 0;

  std::vector<int> seens;
  seens.push_back(p);

  while( p > 0 )
  {
    _entry = entry( p );
    if (_entry->dir && _entry->valid)
    {
      result.insert( 0,  _entry->name);
      result.insert( 0,  "/" );
    }

    p = parent(p);
    if (p < 0) break;

    bool ok = true;
    // sanity check
    for (int i = 0; i < int(seens.size()); i++) {
      if (seens[i] == p) { ok = false; break; }
    }
    if (!ok) break;
    seens.push_back(p);
  }
  return result;
}

// given a fullname (e.g "/ObjectPool/_1020961869"), find the entry
libwpd::DirEntry* libwpd::DirTree::entry( const std::string& name )
{

   if( !name.length() ) return (libwpd::DirEntry*)0;
 
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
     unsigned child = 0;
     // dima: performace optimisation of the previous
     child = find_child( index, *it );
     // traverse to the child
     if( child > 0 ) index = child;
     else return (libwpd::DirEntry*)0;
   }

   return entry( index );
}

// helper function: recursively find siblings of index
static void dirtree_find_siblings( libwpd::DirTree* dirtree, std::vector<unsigned>& result, 
  unsigned index )
{
  libwpd::DirEntry* e = dirtree->entry( index );
  if( !e ) return;
  if( !e->valid ) return;

  // prevent infinite loop  
  for( unsigned i = 0; i < result.size(); i++ )
    if( result[i] == index ) return;

  // add myself    
  result.push_back( index );
  
  // visit previous sibling, don't go infinitely
  unsigned prev = e->prev;
  if( ( prev > 0 ) && ( prev < dirtree->entryCount() ) )
  {
    for( unsigned i = 0; i < result.size(); i++ )
      if( result[i] == prev ) prev = 0;
    if( prev ) dirtree_find_siblings( dirtree, result, prev );
  }
    
  // visit next sibling, don't go infinitely
  unsigned next = e->next;
  if( ( next > 0 ) && ( next < dirtree->entryCount() ) )
  {
    for( unsigned i = 0; i < result.size(); i++ )
      if( result[i] == next ) next = 0;
    if( next ) dirtree_find_siblings( dirtree, result, next );
  }
}

static unsigned dirtree_find_sibling( libwpd::DirTree* dirtree, unsigned index, const std::string& name ) {

  unsigned count = dirtree->entryCount();
  libwpd::DirEntry* e = dirtree->entry( index );
  if (!e || !e->valid) return 0;
  if (e->name == name) return index;

  if (e->next>0 && e->next<count) {
    unsigned r = dirtree_find_sibling( dirtree, e->next, name );
    if (r>0) return r;
  }

  if (e->prev>0 && e->prev<count) {
    unsigned r = dirtree_find_sibling( dirtree, e->prev, name );
    if (r>0) return r;
  }

  return 0;
}

unsigned libwpd::DirTree::find_child( unsigned index, const std::string& name ) {
  
  unsigned count = entryCount();
  libwpd::DirEntry* p = entry( index );
  if (p && p->valid && p->child < count )
    return dirtree_find_sibling( this, p->child, name );

  return 0;
}

std::vector<unsigned> libwpd::DirTree::children( unsigned index )
{
  std::vector<unsigned> result;
  
  DirEntry* e = entry( index );
  if( e ) if( e->valid && e->child < entryCount() )
    dirtree_find_siblings( this, result, e->child );
    
  return result;
}

void libwpd::DirTree::load( unsigned char* buffer, unsigned size )
{
  entries.clear();
  
  for( unsigned i = 0; i < size/128; i++ )
  {
    unsigned p = i * 128;
    
    // would be < 32 if first char in the name isn't printable
    unsigned prefix = 32;
    
    // parse name of this entry, which stored as Unicode 16-bit
    std::string name;
    int name_len = readU16( buffer + 0x40+p );
    if( name_len > 64 ) name_len = 64;
    for( int j=0; ( buffer[j+p]) && (j<name_len); j+= 2 )
      name.append( 1, buffer[j+p] );
      
    // first char isn't printable ? remove it...
    if( buffer[p] < 32 )
    { 
      prefix = buffer[0]; 
      name.erase( 0,1 ); 
    }
    
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

libwpd::StorageIO::StorageIO( libwpd::Storage* st, const std::stringstream &memorystream ) :
	storage(st),
	buf( memorystream.str(), std::ios::binary | std::ios::in ),
	result(libwpd::Storage::Ok),
	bufsize(0),
	header(new libwpd::Header()),
	dirtree(new libwpd::DirTree()),
	bbat(new libwpd::AllocTable()),
	sbat(new libwpd::AllocTable()),
	sb_blocks(),
	streams()
{
  bbat->blockSize = 1 << header->b_shift;
  sbat->blockSize = 1 << header->s_shift;
}

libwpd::StorageIO::~StorageIO()
{
  delete sbat;
  delete bbat;
  delete dirtree;
  delete header;

  std::list<libwpd::Stream*>::iterator it;
  for( it = streams.begin(); it != streams.end(); ++it )
    delete *it;
}

bool libwpd::StorageIO::isOLEStream()
{
  load();
  return (result == libwpd::Storage::Ok);
}

void libwpd::StorageIO::load()
{
  unsigned char* buffer = 0;
  unsigned long buflen = 0;
  std::vector<unsigned long> blocks;
  
  // find size of input file
  buf.seekg( 0, std::ios::end );
  bufsize = buf.tellg();

  // load header
  buffer = new unsigned char[512];
  buf.seekg( 0 ); 
  buf.read( (char*)buffer, 512 );
  header->load( buffer );
  delete[] buffer;

  // check OLE magic id
  result = libwpd::Storage::NotOLE;
  for( unsigned i=0; i<8; i++ )
    if( header->id[i] != wpsole_magic[i] )
      return;
 
  // sanity checks
  result = libwpd::Storage::BadOLE;
  if( !header->valid() ) return;
  if( header->threshold != 4096 ) return;

  // important block size
  bbat->blockSize = 1 << header->b_shift;
  sbat->blockSize = 1 << header->s_shift;
  
  // find blocks allocated to store big bat
  // the first 109 blocks are in header, the rest in meta bat
  blocks.clear();
  blocks.resize( header->num_bat );
  for( unsigned j = 0; j < 109; j++ )
    if( j >= header->num_bat ) break;
    else blocks[j] = header->bb_blocks[j];
  if( (header->num_bat > 109) && (header->num_mbat > 0) )
  {
    unsigned char* buffer2 = new unsigned char[ bbat->blockSize ];
    unsigned k = 109;
    unsigned sector;
    for( unsigned r = 0; r < header->num_mbat; r++ )
    {
      if(r == 0) // 1st meta bat location is in file header.
        sector = header->mbat_start;
      else      // next meta bat location is the last current block value.
        sector = blocks[--k];
      loadBigBlock( sector, buffer2, bbat->blockSize );
      for( unsigned s=0; s < bbat->blockSize; s+=4 )
      {
        if( k >= header->num_bat ) break;
        else  blocks[k++] = readU32( buffer2 + s );
      }
      /* 
      loadBigBlock( header->mbat_start+r, buffer2, bbat->blockSize );
      for( unsigned s=0; s < bbat->blockSize; s+=4 )
      {
        if( k >= header->num_bat ) break;
        else  blocks[k++] = readU32( buffer2 + s );
      }  
      */
     }    
    delete[] buffer2;
  }

  // load big bat
  buflen = blocks.size()*bbat->blockSize;
  if( buflen > 0 )
  {
    buffer = new unsigned char[ buflen ];  
    loadBigBlocks( blocks, buffer, buflen );
    bbat->load( buffer, buflen );
    delete[] buffer;
  }  

  // load small bat
  blocks.clear();
  blocks = bbat->follow( header->sbat_start );
  buflen = blocks.size()*bbat->blockSize;
  if( buflen > 0 )
  {
    buffer = new unsigned char[ buflen ];  
    loadBigBlocks( blocks, buffer, buflen );
    sbat->load( buffer, buflen );
    delete[] buffer;
  }  
  
  // load directory tree
  blocks.clear();
  blocks = bbat->follow( header->dirent_start );
  buflen = blocks.size()*bbat->blockSize;
  buffer = new unsigned char[ buflen ];  
  loadBigBlocks( blocks, buffer, buflen );
  dirtree->load( buffer, buflen );
  unsigned sb_start = readU32( buffer + 0x74 );
  delete[] buffer;
  
  // fetch block chain as data for small-files
  sb_blocks = bbat->follow( sb_start ); // small files
  
  // so far so good
  result = libwpd::Storage::Ok;
}

libwpd::StreamIO* libwpd::StorageIO::streamIO( const std::string& name )
{
  load();

  // sanity check
  if( !name.length() ) return (libwpd::StreamIO*)0;

  // search in the entries
  libwpd::DirEntry* entry = dirtree->entry( name );
  if( !entry ) return (libwpd::StreamIO*)0;
  if( entry->dir ) return (libwpd::StreamIO*)0;

  libwpd::StreamIO* res = new libwpd::StreamIO( this, entry );
  res->fullName = name;
  
  return res;
}

unsigned long libwpd::StorageIO::loadBigBlocks( std::vector<unsigned long> blocks,
  unsigned char* data, unsigned long maxlen )
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
    if( pos + p > bufsize ) p = bufsize - pos;
    buf.seekg( pos );
    buf.read( (char*)data + bytes, p );
    bytes += p;
  }

  return bytes;
}

unsigned long libwpd::StorageIO::loadBigBlock( unsigned long block,
  unsigned char* data, unsigned long maxlen )
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
  unsigned char* data, unsigned long maxlen )
{
  // sentinel
  if( !data ) return 0;
  if( blocks.size() < 1 ) return 0;
  if( maxlen == 0 ) return 0;

  // our own local buffer
  unsigned char* tmpBuf = new unsigned char[ bbat->blockSize ];

  // read small block one by one
  unsigned long bytes = 0;
  for( unsigned long i=0; ( i<blocks.size() ) & ( bytes<maxlen ); i++ )
  {
    unsigned long block = blocks[i];

    // find where the small-block exactly is
    unsigned long pos = block * sbat->blockSize;
    unsigned long bbindex = pos / bbat->blockSize;
    if( bbindex >= sb_blocks.size() ) break;

    loadBigBlock( sb_blocks[ bbindex ], tmpBuf, bbat->blockSize );

    // copy the data
    unsigned offset = pos % bbat->blockSize;
    unsigned long p = (maxlen-bytes < bbat->blockSize-offset ) ? maxlen-bytes :  bbat->blockSize-offset;
    p = (sbat->blockSize<p ) ? sbat->blockSize : p;
    memcpy( data + bytes, tmpBuf + offset, p );
    bytes += p;
  }
  
  delete[] tmpBuf;

  return bytes;
}

unsigned long libwpd::StorageIO::loadSmallBlock( unsigned long block,
  unsigned char* data, unsigned long maxlen )
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

libwpd::StreamIO::StreamIO( libwpd::StorageIO* s, libwpd::DirEntry* e) :
	io(s),
	entry(e),
	fullName(),
	eof(false),
	fail(false),
	blocks(),
	m_pos(0),
	cache_data(0),
	cache_size(4096),
	cache_pos(0)
{
  if( entry->size >= io->header->threshold ) 
    blocks = io->bbat->follow( entry->start );
  else
    blocks = io->sbat->follow( entry->start );

  // prepare cache
  cache_data = new unsigned char[cache_size];
  updateCache();
}

// FIXME tell parent we're gone
libwpd::StreamIO::~StreamIO()
{
  delete[] cache_data;  
}

unsigned long libwpd::StreamIO::tell()
{
  return m_pos;
}

int libwpd::StreamIO::getch()
{
  // past end-of-file ?
  if( m_pos > entry->size ) return -1;

  // need to update cache ?
  if( !cache_size || ( m_pos < cache_pos ) ||
    ( m_pos >= cache_pos + cache_size ) )
      updateCache();

  // something bad if we don't get good cache
  if( !cache_size ) return -1;

  int data = cache_data[m_pos - cache_pos];
  m_pos++;

  return data;
}

unsigned long libwpd::StreamIO::read( unsigned long pos, unsigned char* data, unsigned long maxlen )
{
  // sanity checks
  if( !data ) return 0;
  if( maxlen == 0 ) return 0;

  unsigned long totalbytes = 0;
  
  if ( entry->size < io->header->threshold )
  {
    // small file
    unsigned long index = pos / io->sbat->blockSize;

    if( index >= blocks.size() ) return 0;

    unsigned char* buf = new unsigned char[ io->sbat->blockSize ];
    unsigned long offset = pos % io->sbat->blockSize;
    while( totalbytes < maxlen )
    {
      if( index >= blocks.size() ) break;
      io->loadSmallBlock( blocks[index], buf, io->bbat->blockSize );
      unsigned long count = io->sbat->blockSize - offset;
      if( count > maxlen-totalbytes ) count = maxlen-totalbytes;
      memcpy( data+totalbytes, buf + offset, count );
      totalbytes += count;
      offset = 0;
      index++;
    }
    delete[] buf;

  }
  else
  {
    // big file
    unsigned long index = pos / io->bbat->blockSize;
    
    if( index >= blocks.size() ) return 0;
    
    unsigned char* buf = new unsigned char[ io->bbat->blockSize ];
    unsigned long offset = pos % io->bbat->blockSize;
    while( totalbytes < maxlen )
    {
      if( index >= blocks.size() ) break;
      io->loadBigBlock( blocks[index], buf, io->bbat->blockSize );
      unsigned long count = io->bbat->blockSize - offset;
      if( count > maxlen-totalbytes ) count = maxlen-totalbytes;
      memcpy( data+totalbytes, buf + offset, count );
      totalbytes += count;
      index++;
      offset = 0;
    }
    delete [] buf;

  }

  return totalbytes;
}

unsigned long libwpd::StreamIO::read( unsigned char* data, unsigned long maxlen )
{
  unsigned long bytes = read( tell(), data, maxlen );
  m_pos += bytes;
  return bytes;
}

void libwpd::StreamIO::updateCache()
{
  // sanity check
  if( !cache_data ) return;

  cache_pos = m_pos - ( m_pos % cache_size );
  unsigned long bytes = cache_size;
  if( cache_pos + bytes > entry->size ) bytes = entry->size - cache_pos;
  cache_size = read( cache_pos, cache_data, bytes );
}


// =========== Storage ==========

libwpd::Storage::Storage( const std::stringstream &memorystream ) :
	io(NULL)
{
	io = new StorageIO( this, memorystream );
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

libwpd::Stream::Stream( libwpd::Storage* storage, const std::string& name ) :
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

unsigned long libwpd::Stream::read( unsigned char* data, unsigned long maxlen )
{
  return io ? io->read( data, maxlen ) : 0;
}
