/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "libwpd.h"
#include "WP5Header.h"
//include "WP6FileStructure.h" 
#include "libwpd_internal.h"

WP5Header::WP5Header(WPXInputStream * input, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion, guint16 documentEncryption) :
	WPXHeader(input, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption)
{
	// nothing to do here really...
}

void WP5Header::_readndexInformation(WPXInputStream *input)
{
/*	// read the Index Header (Header #0)
	// skip the Flags = 2 and the Reserved byte = 0
	input->seek(m_indexHeaderOffset + WP6_INDEX_HEADER_NUM_INDICES_POSITION, WPX_SEEK_SET);
	m_numPrefixIndices = readU16(input);

	// ignore the 10 reserved bytes that follow (jump to the offset of the Index Header #1, where we can resume parsing)
	input->seek(m_indexHeaderOffset + WP6_INDEX_HEADER_INDICES_POSITION, WPX_SEEK_SET);*/
}
