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

#include "WP61Header.h"
#include "WP6FileStructure.h" 
#include "libwpd_internal.h"

WP61Header::WP61Header(WPXInputStream * input, WPXEncryption *encryption, uint32_t documentOffset, uint8_t productType,
	uint8_t fileType, uint8_t majorVersion, uint8_t minorVersion, uint16_t documentEncryption) :
	WP6Header(input, encryption, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption),
	m_documentSize(0)
{
	input->seek(WP6_HEADER_DOCUMENT_SIZE_OFFSET, WPX_SEEK_SET);
	m_documentSize = readU32(input, encryption);

	WPD_DEBUG_MSG(("WordPerfect: Document End Position = 0x%x \n",(int)m_documentSize));

	_readIndexInformation(input, encryption);
}

WP61Header::~WP61Header()
{

}
