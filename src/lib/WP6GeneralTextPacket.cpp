/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
#include <string.h>
#include <gsf/gsf-input-memory.h>

#include "WP6GeneralTextPacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6GeneralTextPacket::WP6GeneralTextPacket(WPXInputStream *input, int id, guint32 dataOffset, guint32 dataSize) 
	: WP6PrefixDataPacket(input)
{	
	_read(input, dataOffset, dataSize);
}

WP6GeneralTextPacket::~WP6GeneralTextPacket()
{
	g_object_unref(G_OBJECT(m_stream));
	delete [] m_blockSizes;
}

void WP6GeneralTextPacket::_readContents(WPXInputStream *input)
{
	guint16 m_numTextBlocks = readU16(input);
	guint32 m_firstTextBlockOffset = readU32(input);

	if (m_numTextBlocks < 1)
		throw ParseException();
	
	m_blockSizes = new guint32[m_numTextBlocks];
	int totalSize = 0;
	int i;

	for(i=0; i<m_numTextBlocks; i++)
	{
		m_blockSizes[i] = readU32(input);
		totalSize += m_blockSizes[i];
	}	

	// we have to use glib's allocation function because libgsf disposes of the data
	guint8 *streamData = new guint8[totalSize];
	int streamPos = 0;
	for(i=0; i<m_numTextBlocks; i++) 
	{
		for (int j=0; j<m_blockSizes[i]; j++)
		{
			streamData[streamPos] = readU8(input);
			streamPos++;
		}
	}

	m_stream = new WPXMemoryInputStream(streamData, totalSize);
}

void WP6GeneralTextPacket::parse(WP6HLListener *listener) const
{
	m_stream->seek(0, WPX_SEEK_SET);
	WP6Parser::parseDocument(m_stream, listener);
}
