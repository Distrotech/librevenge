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

#include <stdio.h>
#include <stdlib.h>
#include "UT_libwpd2.h"
#include "WP6PrefixPacket.h"
#include "WP6UnsupportedPrefixPacket.h"
#include "WP6FileStructure.h" 

WP6PrefixPacket::WP6PrefixPacket(FILE * stream, guint8 flags)
	: 	m_flags(flags),
 		m_useCount(0),
 		m_hideCount(0),
 		m_dataSize(0),
 		m_dataOffset(0),
		m_hasChildren(FALSE)
{
}

WP6PrefixPacket * WP6PrefixPacket::constructPrefixPacket(FILE * stream)
{
	guint8 flags;
	guint8 type;
	
	// Read the packet flags first
	WPD_CHECK_FILE_READ_ERROR(fread(&flags, sizeof(guint8), 1, stream), 1);
	// Now, we can read the packet type, which we need to select which packet to create
	WPD_CHECK_FILE_READ_ERROR(fread(&type, sizeof(guint8), 1, stream), 1);
	
	switch (type)
	{
		case WP6_INDEX_HEADER_DESIRED_FONT_DESCRIPTOR_POOL:
			//return new WP6FontDescriptorPacket(stream, flags);
		case WP6_INDEX_HEADER_EXTENDED_DOCUMENT_SUMMARY_DESCRIPTOR_POOL:
			//return new WP6DocumentInformationDescriptorPacket(stream, flags);
		case WP6_INDEX_HEADER_OUTLINE_STYLE:
			//return new WP6OutlineStylePacket(stream, flags);
		default:
			return new WP6UnsupportedPrefixPacket(stream, flags);
	}
}

void WP6PrefixPacket::_read(FILE *stream)
{
	WPD_CHECK_FILE_READ_ERROR(fread(&m_useCount, sizeof(guint16), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_hideCount, sizeof(guint16), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_dataSize, sizeof(guint32), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_dataOffset, sizeof(guint32), 1, stream), 1);
	
	if (m_flags & WP6_INDEX_HEADER_ELEMENT_CHILD_PACKET_BIT)
		m_hasChildren = true;

	_readContents(stream);
}
