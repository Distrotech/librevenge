/* libwpd2
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

#include "libwpd.h"
#include "WP61Header.h"
#include "WP6FileStructure.h" 
#include "libwpd_internal.h"

WP61Header::WP61Header(GsfInput * input, guint32 documentOffset,  guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion, guint16 documentEncryption)
	:	WP6Header(input, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption)
{
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_HEADER_DOCUMENT_SIZE_OFFSET, G_SEEK_SET));
	m_documentSize = gsf_le_read_guint32(input);

	WPD_DEBUG_MSG(("WordPerfect: Document End Position = 0x%x \n",(int)m_documentSize));

	_readIndexInformation(input);
}

WP61Header::~WP61Header()
{

}
