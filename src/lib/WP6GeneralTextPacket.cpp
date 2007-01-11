/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP6GeneralTextPacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6GeneralTextPacket::WP6GeneralTextPacket(WPXInputStream *input, int /* id */, uint32_t dataOffset, uint32_t dataSize): 
	WP6PrefixDataPacket(input),
	m_subDocument(0)
{	
	_read(input, dataOffset, dataSize);
}

WP6GeneralTextPacket::~WP6GeneralTextPacket()
{
	if (m_subDocument)
		delete m_subDocument;
}

void WP6GeneralTextPacket::_readContents(WPXInputStream *input)
{
	long startPosition = input->tell();
	uint16_t numTextBlocks = readU16(input);
	input->seek(4, WPX_SEEK_CUR);

	if (numTextBlocks < 1)
	{
		WPD_DEBUG_MSG(("WordPerfect: Number of text blocks is %i\n", numTextBlocks));
		return; // m_subDocument will be 0
	}
	
	uint32_t *blockSizes = new uint32_t[numTextBlocks];
	int totalSize = 0;
	unsigned int i;

	for(i=0; i<numTextBlocks; i++)
	{
		if ((input->tell() - startPosition + 4) > getDataSize() || input->atEOS())
			throw FileException();
		blockSizes[i] = readU32(input);
		totalSize += blockSizes[i];
	}	

	if (totalSize <= 0)
	{
		WPD_DEBUG_MSG(("WordPerfect: The total size of the text is %i\n", totalSize));
		if (blockSizes)
			delete [] blockSizes;
		return; // m_subDocument will be 0
	}
	uint8_t *streamData = new uint8_t[totalSize];
	int streamPos = 0;
	for(i=0; i<numTextBlocks; i++) 
	{
		if ((input->tell() - startPosition + blockSizes[i]) > getDataSize() || input->atEOS())
			throw FileException();
		for (unsigned int j=0; j<blockSizes[i]; j++)
		{
			streamData[streamPos] = readU8(input);
			streamPos++;
		}
	}
	if (blockSizes)
		delete [] blockSizes;

	m_subDocument = new WP6SubDocument(streamData, totalSize);
}

void WP6GeneralTextPacket::parse(WP6Listener *listener) const
{
	if (m_subDocument)
		m_subDocument->parse(listener);
}
