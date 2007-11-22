/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXSubDocument.h"
#include "WP3Parser.h"
#include "libwpd_internal.h"
#include "WPXListener.h"
#include <string.h> 

WPXSubDocument::WPXSubDocument() :
	m_stream(0),
	m_streamData(0)
{
}

WPXSubDocument::WPXSubDocument(WPXInputStream *input, WPXEncryption *encryption, const unsigned dataSize) :
	m_stream(0),
	m_streamData(new uint8_t[dataSize])
{
	unsigned i=0;
	for (; i<dataSize; i++)
	{
		if (input->atEOS())
			break;
		m_streamData[i] = readU8(input, encryption);
	}
	m_stream = new WPXMemoryInputStream(m_streamData, i);
}

WPXSubDocument::WPXSubDocument(uint8_t * streamData, const unsigned dataSize) :
	m_stream(0),
	m_streamData(0)
{
	if (streamData)
		m_stream = new WPXMemoryInputStream(streamData, dataSize);
}

WPXSubDocument::~WPXSubDocument()
{
	if (m_stream)
		delete m_stream;
	if (m_streamData)
		delete [] m_streamData;
}
