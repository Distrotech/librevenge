#ifndef WPXSTREAM_H
#define WPXSTREAM_H
#include <glib.h>

enum WPX_SEEK_TYPE
{
	WPX_SEEK_CUR,
	WPX_SEEK_SET
};

class WPXInputStream
{
public:
	WPXInputStream(bool supportsOLE) :
		m_supportsOLE(supportsOLE) {}
	virtual ~WPXInputStream() {}

	virtual bool isOLEStream() = 0;
	virtual WPXInputStream * getDocumentOLEStream() = 0;

	const virtual guint8 *read(size_t numBytes) = 0;
	virtual int seek(long offset, WPX_SEEK_TYPE seekType) = 0;
	virtual long tell() = 0;
	virtual bool atEOS() = 0;

private:
	bool m_supportsOLE;
};
#endif
