#ifndef GSFSTREAM_H
#define GSFSTREAM_H

#include "WPXStream.h"

// This one is needed to prevent the confusion of MSVC6 with
// term allocator It has to come before including first header
// that leads to glib
#ifdef _MSC_VER
# define allocator gallocator
#endif

#include <gsf/gsf-input.h>

#ifdef _MSC_VER
# ifdef allocator
#  undef allocator
# endif
#endif

class GSFInputStream : public WPXInputStream
{
public:
	GSFInputStream(GsfInput *input);
	virtual ~GSFInputStream();

	virtual bool isOLEStream();
	virtual WPXInputStream * getDocumentOLEStream();

	virtual const uint8_t *read(size_t numBytes, size_t &numBytesRead);
	virtual int seek(long offset, WPX_SEEK_TYPE seekType);
	virtual long tell() { return gsf_input_tell(m_input); }
	virtual bool atEOS() { return gsf_input_eof(m_input); }

private:
	GsfInput *m_input;
	GsfInfile *m_ole;
};

#endif
