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
#include "UT_libwpd2.h"
#include "WPXParser.h"
#include "WP6Parser.h"
#include "WP6FileStructure.h"

WPXParser::WPXParser(FILE * stream, WPXHeader * header)
{
	m_stream = stream;
	m_header = header;	
}

WPXParser * WPXParser::constructParser(FILE * stream)
{
	if (!stream)
	{
		WPD_DEBUG_MSG(("WordPerfect: Stream is NULL!\n"));
		return NULL;
	}

	char fileMagic[4];
		
	guint32 documentOffset;
	guint8 productType;
	guint8 fileType;
	guint8 majorVersion;
	guint8 minorVersion;
	
	/* check the magic */
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, WP6_HEADER_MAGIC_OFFSET - ftell(stream), SEEK_CUR));
	WPD_CHECK_FILE_READ_ERROR(fread(fileMagic, sizeof(char), 3, stream), 3);
	fileMagic[3] = '\0';
	
	if ( strcmp(fileMagic, "WPC") )
	{
		WPD_DEBUG_MSG(("WordPerfect: File magic is not equal to \"WPC\"!\n"));
		return NULL;
	}
	
	/* get the document pointer */
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, WP6_HEADER_DOCUMENT_POINTER_OFFSET - ftell(stream), SEEK_CUR));
	WPD_CHECK_FILE_READ_ERROR(fread(&documentOffset, sizeof(guint32), 1, stream), 1);

	/* get information on product types, file types, versions */
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, WP6_HEADER_PRODUCT_TYPE_OFFSET - ftell(stream), SEEK_CUR));
	WPD_CHECK_FILE_READ_ERROR(fread(&productType, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&fileType, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&majorVersion, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&minorVersion, sizeof(guint8), 1, stream), 1);

	/* chech if this document is a WP6 or higher document */
	if (majorVersion == WP6_EXPECTED_MAJOR_VERSION && 
		fileType == WP6_DOCUMENT_FILE_TYPE)
	{
		WP6Header * header = new WP6Header(stream, documentOffset, productType, fileType, majorVersion, minorVersion);
		return new WP6Parser(stream, header);
	}

	return NULL;
}

