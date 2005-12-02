#include "WPXMemoryStream.h"
#include "libwpd_internal.h"


WPXMemoryInputStream::WPXMemoryInputStream(uint8_t *data, size_t size) :
	WPXInputStream(false),
	m_offset(0),
	m_size(size),
	m_data(data),
	m_tmpBuf(NULL)
{
}

WPXMemoryInputStream::~WPXMemoryInputStream()
{
	delete [] m_tmpBuf;
	delete [] m_data;
}

const uint8_t * WPXMemoryInputStream::read(size_t numBytes, size_t &numBytesRead)
{
	delete [] m_tmpBuf;
	int numBytesToRead;

	if ((m_offset+numBytes) < m_size)
		numBytesToRead = numBytes;
	else
		numBytesToRead = m_size - m_offset;
	
	numBytesRead = numBytesToRead; // about as paranoid as we can be..

	if (numBytesToRead == 0)
		return NULL;

	m_tmpBuf = new uint8_t[numBytesToRead];
	for (size_t i=0; i<numBytesToRead; i++)
	{
		m_tmpBuf[i] = m_data[m_offset];
		m_offset++;
	}

	return m_tmpBuf;
}

int WPXMemoryInputStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_CUR)
		m_offset += offset;
	else if (seekType == WPX_SEEK_SET)
		m_offset = offset;

	if (m_offset < 0)
		m_offset = 0;
	else if (m_offset >= m_size)
		m_offset = m_size;

	return 0;
}

long WPXMemoryInputStream::tell()
{
	return m_offset;
}

bool WPXMemoryInputStream::atEOS()
{
	if (m_offset == m_size) 
		return true; 

	return false;
}
