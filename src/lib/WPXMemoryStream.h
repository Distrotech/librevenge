#ifndef MEMORYSTREAM_H
#define MEMORYSTREAM_H
#include "WPXStream.h"

class WPXMemoryInputStream : public WPXInputStream
{
public:
	WPXMemoryInputStream(guint8 *data, size_t size);
	virtual ~WPXMemoryInputStream();

	virtual bool isOLEStream() { return false; }
	virtual WPXInputStream * getDocumentOLEStream() { return NULL; }
	
	const virtual guint8 *read(size_t numBytes);
	virtual int seek(long offset, WPX_SEEK_TYPE seekType);
	virtual long tell();
	virtual bool atEOS();

private:
	long m_offset;
	size_t m_size;
	guint8 *m_data;
	guint8 *m_tmpBuf;
};

#endif
