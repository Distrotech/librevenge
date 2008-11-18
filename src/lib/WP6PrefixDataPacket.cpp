/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2007 Fridrich Strba (fridrich_strba@bluewin.ch)
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

#include "WP6PrefixDataPacket.h"
#include "WP6PrefixIndice.h"
#include "WP6CommentAnnotationPacket.h"
#include "WP6GeneralTextPacket.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6FillStylePacket.h"
#include "WP6DefaultInitialFontPacket.h"
#include "WP6ExtendedDocumentSummaryPacket.h"
#include "WP6OutlineStylePacket.h"
#include "WP6FileStructure.h"
#include "WP6GraphicsBoxStylePacket.h"
#include "WP6TableStylePacket.h"
#include "WP6GraphicsFilenamePacket.h"
#include "WP6GraphicsCachedFileDataPacket.h"
#include "libwpd_internal.h"

WP6PrefixDataPacket::WP6PrefixDataPacket(WPXInputStream * /* input */, WPXEncryption * /* encryption */) :
	m_dataSize(0)
{
}

WP6PrefixDataPacket * WP6PrefixDataPacket::constructPrefixDataPacket(WPXInputStream * input, WPXEncryption *encryption, WP6PrefixIndice *prefixIndice)
{	       
	switch (prefixIndice->getType())
	{
	case WP6_INDEX_HEADER_INITIAL_FONT:			
		return new WP6DefaultInitialFontPacket(input, encryption, prefixIndice->getID(), 
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_GENERAL_WORDPERFECT_TEXT:
		return new WP6GeneralTextPacket(input, encryption, prefixIndice->getID(), 
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_DESIRED_FONT_DESCRIPTOR_POOL:
		return new WP6FontDescriptorPacket(input, encryption, prefixIndice->getID(), 
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_FILL_STYLE:
		return new WP6FillStylePacket(input, encryption, prefixIndice->getID(), 
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY:
		return new WP6ExtendedDocumentSummaryPacket(input, encryption, prefixIndice->getID(), 
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_OUTLINE_STYLE:
		return new WP6OutlineStylePacket(input, encryption, prefixIndice->getID(), 
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_GRAPHICS_FILENAME:
		return new WP6GraphicsFilenamePacket(input, encryption, prefixIndice->getID(), prefixIndice->getFlags(), 
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_GRAPHICS_CACHED_FILE_DATA:
		return new WP6GraphicsCachedFileDataPacket(input, encryption, prefixIndice->getID(),
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_GRAPHICS_BOX_STYLE:
		return new WP6GraphicsBoxStylePacket(input, encryption, prefixIndice->getID(),
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_TABLE_STYLE:
		return new WP6TableStylePacket(input, encryption, prefixIndice->getID(),
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_COMMENT_ANNOTATION:
		return new WP6CommentAnnotationPacket(input, encryption, prefixIndice->getID(),
				prefixIndice->getDataOffset(), prefixIndice->getDataSize());
	default:
		return 0;
	}
}

void WP6PrefixDataPacket::_read(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataOffset, uint32_t dataSize)
{
	m_dataSize = dataSize;

	if (!m_dataSize)
		return;

	input->seek(dataOffset, WPX_SEEK_SET);

	_readContents(input, encryption);

	// assert that we haven't surpassed the size of the packet?
}
