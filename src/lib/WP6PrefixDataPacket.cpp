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

#include "WP6PrefixDataPacket.h"
#include "WP6PrefixIndice.h"
#include "WP6GeneralTextPacket.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6FillStylePacket.h"
#include "WP6DefaultInitialFontPacket.h"
#include "WP6ExtendedDocumentSummaryPacket.h"
#include "WP6OutlineStylePacket.h"
#include "WP6FileStructure.h" 
#include "libwpd.h"
#include "libwpd_internal.h"

WP6PrefixDataPacket::WP6PrefixDataPacket(WPXInputStream * /* input */)
{
}

WP6PrefixDataPacket * WP6PrefixDataPacket::constructPrefixDataPacket(WPXInputStream * input, WP6PrefixIndice *prefixIndice)
{	       
	switch (prefixIndice->getType())
	{
	case WP6_INDEX_HEADER_INITIAL_FONT:			
		return new WP6DefaultInitialFontPacket(input, prefixIndice->getID(), 
						       prefixIndice->getDataOffset(), 
						       prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_GENERAL_WORDPERFECT_TEXT:
		return new WP6GeneralTextPacket(input, prefixIndice->getID(), 
						prefixIndice->getDataOffset(), 
						prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_DESIRED_FONT_DESCRIPTOR_POOL:
		return new WP6FontDescriptorPacket(input, prefixIndice->getID(), 
						   prefixIndice->getDataOffset(), 
						   prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_FILL_STYLE:
		return new WP6FillStylePacket(input, prefixIndice->getID(), 
					      prefixIndice->getDataOffset(), 
					      prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY:
		return new WP6ExtendedDocumentSummaryPacket(input, prefixIndice->getID(), 
							    prefixIndice->getDataOffset(), 
							    prefixIndice->getDataSize());
	case WP6_INDEX_HEADER_OUTLINE_STYLE:
		return new WP6OutlineStylePacket(input, prefixIndice->getID(), 
						 prefixIndice->getDataOffset(), 
						 prefixIndice->getDataSize());
	default:
		return NULL;
	}
}

void WP6PrefixDataPacket::_read(WPXInputStream *input, uint32_t dataOffset, uint32_t dataSize)
{
	if (!dataSize)
		return;

	input->seek(dataOffset, WPX_SEEK_SET);

	_readContents(input);

	// assert that we haven't surpassed the size of the packet?
}
