#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H
#include "WPXStream.h"

class WPXMemoryInputStream : public WPXInputStream
{
public:
	WPXMemoryInputStream(uint8_t *data, size_t size);
	virtual ~WPXMemoryInputStream();

	virtual bool isOLEStream() { return false; }
	virtual WPXInputStream * getDocumentOLEStream() { return NULL; }
	
	const virtual uint8_t *read(size_t numBytes, size_t &numBytesRead);
	virtual int seek(long offset, WPX_SEEK_TYPE seekType);
	virtual long tell();
	virtual bool atEOS();

private:
	long m_offset;
	size_t m_size;
	uint8_t *m_data;
	uint8_t *m_tmpBuf;
};

#endif
