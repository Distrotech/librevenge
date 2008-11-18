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

#include "WP60Header.h"
#include "WP6FileStructure.h" 
#include "libwpd_internal.h"

WP6Header::WP6Header(WPXInputStream * input, WPXEncryption *encryption, uint32_t documentOffset, uint8_t productType,
	uint8_t fileType, uint8_t majorVersion, uint8_t minorVersion, uint16_t documentEncryption) :
	WPXHeader(input, encryption, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption),
	m_indexHeaderOffset(0),
	m_numPrefixIndices(0)
{
	input->seek(WP6_HEADER_INDEX_HEADER_POINTER_OFFSET, WPX_SEEK_SET);
	m_indexHeaderOffset = readU16(input, encryption);

	// according to the WP6.0 specs, if the index header offset variable is less than 16, it is 16
	// ASSUMPTION: this also goes for WP6.1
	if (m_indexHeaderOffset < 16)
		m_indexHeaderOffset = 16;

	// FIXME: we do not handle encrypted documents
	if (getDocumentEncryption() != 0)
		throw UnsupportedEncryptionException();

	WPD_DEBUG_MSG(("WordPerfect: Index Header Position = 0x%x \n",(int)m_indexHeaderOffset));
}

void WP6Header::_readIndexInformation(WPXInputStream *input, WPXEncryption *encryption)
{
	// read the Index Header (Header #0)
	// skip the Flags = 2 and the Reserved byte = 0
	input->seek(m_indexHeaderOffset + WP6_INDEX_HEADER_NUM_INDICES_POSITION, WPX_SEEK_SET);
	m_numPrefixIndices = readU16(input, encryption);

	// ignore the 10 reserved bytes that follow (jump to the offset of the Index Header #1, where we can resume parsing)
	input->seek(m_indexHeaderOffset + WP6_INDEX_HEADER_INDICES_POSITION, WPX_SEEK_SET);
}
