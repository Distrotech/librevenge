/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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

#include "WP6PrefixDataPacket.h"
#include "WP6PrefixIndice.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"
#include "WP6FileStructure.h" 
#include "libwpd.h"
#include "libwpd_internal.h"

WP6PrefixDataPacket::WP6PrefixDataPacket(FILE * stream, int id)
	: m_id(id)
{
}

WP6PrefixDataPacket * WP6PrefixDataPacket::constructPrefixDataPacket(FILE * stream, WP6PrefixIndice *prefixIndice)
{	       
	switch (prefixIndice->getType())
	{
		case WP6_INDEX_HEADER_INITIAL_FONT:			
			return new WP6DefaultInitialFontPacket(stream, prefixIndice->getID(), 
							   prefixIndice->getDataOffset(), 
							   prefixIndice->getDataSize());
		case WP6_INDEX_HEADER_DESIRED_FONT_DESCRIPTOR_POOL:
			return new WP6FontDescriptorPacket(stream, prefixIndice->getID(), 
							   prefixIndice->getDataOffset(), 
							   prefixIndice->getDataSize());
		case WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DESCRIPTOR_POOL:
			//return new WP6DocumentInformationDescriptorPacket(stream, flags);
		case WP6_INDEX_HEADER_OUTLINE_STYLE:
			//return new WP6OutlineStylePacket(stream, flags);
		default:
			return NULL;
	}
}

void WP6PrefixDataPacket::_read(FILE *stream, guint32 dataOffset, guint32 dataSize)
{
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, dataOffset, SEEK_SET));

	_readContents(stream);

	// assert that we haven't surpassed the size of the packet?
}
