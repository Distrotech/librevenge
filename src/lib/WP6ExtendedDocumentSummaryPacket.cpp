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

#include "WP6ExtendedDocumentSummaryPacket.h"
#include "libwpd_internal.h"

WP6ExtendedDocumentSummaryPacket::WP6ExtendedDocumentSummaryPacket(WPXInputStream *input, int id, guint32 dataOffset, guint32 dataSize) 
	: WP6PrefixDataPacket(input),
	  m_dataSize(dataSize)
{	
	_read(input, dataOffset, dataSize);
}

WP6ExtendedDocumentSummaryPacket::~WP6ExtendedDocumentSummaryPacket()
{
	DELETEP(m_stream);
}

void WP6ExtendedDocumentSummaryPacket::_readContents(WPXInputStream *input)
{
	// we have to use glib's allocation function because libgsf disposes of the data
	guint8 *streamData = new guint8[m_dataSize];
	for(int i=0; i<m_dataSize; i++)
		streamData[i] = readU8(input);
	
	m_stream = new WPXMemoryInputStream(streamData, m_dataSize);
}

void WP6ExtendedDocumentSummaryPacket::parse(WP6HLListener *listener) const
{
	int k=0;
	guint16 groupLength = 0;

	for (int i=0; i < m_dataSize; i+=groupLength)
	{
		groupLength = readU16(m_stream);
		guint16 tagID = readU16(m_stream);
		guint16 flags = readU16(m_stream);

		UCSString name;
		for (guint16 wpChar = readU16(m_stream);
		     wpChar != 0; 
		     wpChar = readU16(m_stream))
		{
			guint8 character = (wpChar & 0xFF);
			guint8 characterSet = (wpChar & 0xFF00) >> 8;
			const guint16 *chars;
			int len;
			len = extendedCharacterToUCS2(character,
						      characterSet, &chars);
			for (int j = 0; j < len; j++)
				name.append(chars[j]);
		} 
		
		if (tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_CREATION_DATE ||
		    tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DATE_COMPLETED)
		{
			guint16 year = readU16(m_stream);
			guint8 month = readU8(m_stream);
			guint8 day = readU8(m_stream);
			guint8 hour = readU8(m_stream);
			guint8 minute = readU8(m_stream);
			guint8 second = readU8(m_stream);
			guint8 dayOfWeek = readU8(m_stream);
			guint8 timeZone = readU8(m_stream);
			guint8 unused = readU8(m_stream);
			listener->setDate(year, month, day, hour, minute, second, dayOfWeek, timeZone, unused);
		}
		else
		{
			UCSString data;
			for (guint16 wpChar = readU16(m_stream);
			     wpChar != 0; 
			     wpChar = readU16(m_stream))
			{				
			guint8 character = (wpChar & 0xFF);
			guint8 characterSet = (wpChar & 0xFF00) >> 8;
			const guint16 *chars;
			int len;
			len = extendedCharacterToUCS2(character,
						      characterSet, &chars);
			for (int j = 0; j < len; j++)
				data.append(chars[j]);
			} 
			listener->setExtendedInformation(tagID, data);
		}
		m_stream->seek((i+groupLength), WPX_SEEK_SET);
	}
}
