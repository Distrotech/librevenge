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
#include <string.h>
#include "WPXHeader.h"
#include "WP6FileStructure.h"
#include "libwpd.h"
#include "libwpd_internal.h"


WPXHeader::WPXHeader(FILE *stream)
{
	char fileMagic[4];
	/* check the magic */
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, WP6_HEADER_MAGIC_OFFSET - ftell(stream), SEEK_CUR));
	WPD_CHECK_FILE_READ_ERROR(fread(fileMagic, sizeof(char), 3, stream), 3);
	fileMagic[3] = '\0';
	
	if ( strcmp(fileMagic, "WPC") )
	{
		WPD_DEBUG_MSG(("WordPerfect: File magic is not equal to \"WPC\"!\n"));
		throw FileException(); // FIXME: this is not a file exception!!
	}
	
	/* get the document pointer */
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, WP6_HEADER_DOCUMENT_POINTER_OFFSET - ftell(stream), SEEK_CUR));
	WPD_CHECK_FILE_READ_ERROR(fread(&m_documentOffset, sizeof(guint32), 1, stream), 1);

	/* get information on product types, file types, versions */
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, WP6_HEADER_PRODUCT_TYPE_OFFSET - ftell(stream), SEEK_CUR));
	WPD_CHECK_FILE_READ_ERROR(fread(&m_productType, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_fileType, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_majorVersion, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_minorVersion, sizeof(guint8), 1, stream), 1);
	
	WPD_DEBUG_MSG(("WordPerfect: Product Type: %i File Type: %i Major Version: %i Minor Version: %i\n", 
					m_productType, m_fileType, 
					m_majorVersion, m_minorVersion));		
}

