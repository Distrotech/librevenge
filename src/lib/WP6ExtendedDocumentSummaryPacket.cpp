/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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
#include <string.h>
#include <limits>

#include "WP6ExtendedDocumentSummaryPacket.h"
#include "libwpd_internal.h"

WP6ExtendedDocumentSummaryPacket::WP6ExtendedDocumentSummaryPacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, uint32_t dataOffset, uint32_t dataSize) :
	WP6PrefixDataPacket(input, encryption),
	m_dataSize(dataSize),
	m_streamData(0),
	m_stream(0)
{	
	if (dataSize > 0)
		_read(input, encryption, dataOffset, dataSize);
}

WP6ExtendedDocumentSummaryPacket::~WP6ExtendedDocumentSummaryPacket()
{
	if (m_stream)
		DELETEP(m_stream);
	if (m_streamData)
		delete [] m_streamData;
}

void WP6ExtendedDocumentSummaryPacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	if (m_dataSize <= 0)
		return;
	if (m_dataSize > ((std::numeric_limits<uint32_t>::max)() / 2))
		m_dataSize = ((std::numeric_limits<uint32_t>::max)() / 2);
	m_streamData = new uint8_t[m_dataSize];
	for(unsigned i=0; i<(unsigned)m_dataSize; i++)
		m_streamData[i] = readU8(input, encryption);
	
	m_stream = new WPXMemoryInputStream(m_streamData, (unsigned long)m_dataSize);
}

void WP6ExtendedDocumentSummaryPacket::parse(WP6Listener *listener) const
{
	if (!m_stream)
		return;
	uint16_t groupLength = 0;

	for (unsigned i=0; i < (unsigned)m_dataSize && !m_stream->atEOS(); i+=groupLength)
	{
		try
		{
			groupLength = readU16(m_stream, 0);
		}
		catch (FileException)
		{
			return;
		}
		if ((groupLength == 0) || m_stream->atEOS())
			return;
		uint16_t tagID = readU16(m_stream, 0);
		if (m_stream->atEOS())
			return;
		if (m_stream->seek(2, WPX_SEEK_CUR))
			return;

		WPXString name;
		uint16_t wpChar = 0;
		if (!m_stream->atEOS())
			wpChar = readU16(m_stream, 0);
		for (; wpChar != 0 && !m_stream->atEOS(); wpChar = readU16(m_stream, 0))
		{
			uint8_t character = (uint8_t)(wpChar & 0x00FF);
			uint8_t characterSet = (uint8_t)((wpChar >> 8) & 0x00FF);
			const uint16_t *chars;
			int len = extendedCharacterWP6ToUCS2(character,
						      characterSet, &chars);
			for (int j = 0; j < len; j++)
				appendUCS4(name, (uint32_t)chars[j]);
		} 
		
		if (tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_CREATION_DATE ||
		    tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DATE_COMPLETED ||
		    tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_RECORDED_DATE ||
		    tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_REVISION_DATE ||
		    tagID == WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_VERSION_DATE)
		{
			try
			{
				uint16_t year = readU16(m_stream, 0);
				uint8_t month = readU8(m_stream, 0);
				uint8_t day = readU8(m_stream, 0);
				uint8_t hour = readU8(m_stream, 0);
				uint8_t minute = readU8(m_stream, 0);
				uint8_t second = readU8(m_stream, 0);
				uint8_t dayOfWeek = readU8(m_stream, 0);
				uint8_t timeZone = readU8(m_stream, 0);
				uint8_t unused = readU8(m_stream, 0);
				if (month > 0 && day > 0 && year >= 1900)
					listener->setDate(tagID, year, month, day, hour, minute, second, dayOfWeek, timeZone, unused);
			}
			catch (FileException)
			{
				return;
			}
		}
		else
		{
			WPXString data;
			if (!m_stream->atEOS())
				wpChar = readU16(m_stream, 0);
			for (; wpChar != 0 && !m_stream->atEOS(); wpChar = readU16(m_stream, 0))
			{				
				uint8_t character = (uint8_t)(wpChar & 0x00FF);
				uint8_t characterSet = (uint8_t)((wpChar >> 8) & 0x00FF);
				const uint16_t *chars;
				int len = extendedCharacterWP6ToUCS2(character,
						      characterSet, &chars);
				for (int j = 0; j < len; j++)
					appendUCS4(data, (uint32_t)chars[j]);
			}
			if (data.len())
				listener->setExtendedInformation(tagID, data);
		}
		m_stream->seek((i+groupLength), WPX_SEEK_SET);
	}
}
