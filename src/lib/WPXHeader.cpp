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

#include <gsf/gsf-input.h>
#include <stdlib.h>
#include <string.h>
#include "WPXHeader.h"
#include "WP5Header.h"
#include "WP6Header.h"
#include "WP60Header.h"
#include "WP61Header.h"
#include "WPXFileStructure.h"
#include "libwpd.h"
#include "libwpd_internal.h"
	
WPXHeader::WPXHeader(GsfInput *input, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion, guint16 documentEncryption) :
	m_documentOffset(documentOffset),
	m_productType(productType),
	m_fileType(fileType),
	m_majorVersion(majorVersion),
	m_minorVersion(minorVersion),
	m_documentEncryption(documentEncryption)
{
}

WPXHeader::~WPXHeader()
{
}

WPXHeader * WPXHeader::constructHeader(GsfInput *input)
{
	gchar fileMagic[4];
	/* check the magic */
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WPX_HEADER_MAGIC_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	for (int i=0; i<3 /* FIXME: && not EOF */; i++)
		fileMagic[i] = GSF_LE_GET_GINT8(gsf_input_read(input, sizeof(guint8), NULL));
	fileMagic[3] = '\0';
	
	if ( strcmp(fileMagic, "WPC") )
	{
		WPD_DEBUG_MSG(("WordPerfect: File magic is not equal to \"WPC\"!\n"));
		return NULL;
	}
	
	/* get the document pointer */
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WPX_HEADER_DOCUMENT_POINTER_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	guint32 documentOffset = gsf_le_read_guint32(input);

	/* get information on product types, file types, versions */
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WPX_HEADER_PRODUCT_TYPE_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	guint8 productType = gsf_le_read_guint8(input);
	guint8 fileType = gsf_le_read_guint8(input);
	guint8 majorVersion = gsf_le_read_guint8(input);
	guint8 minorVersion = gsf_le_read_guint8(input);
	
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WPX_HEADER_ENCRYPTION_OFFSET, G_SEEK_SET));
	guint8 documentEncryption = gsf_le_read_guint16(input);		
	
	WPD_DEBUG_MSG(("WordPerfect: Product Type: %i File Type: %i Major Version: %i Minor Version: %i\n", 
					productType, fileType, 
					majorVersion, minorVersion));	
	WPD_DEBUG_MSG(("WordPerfect: Document Encryption = 0x%x \n",(int)documentEncryption));		

	switch (majorVersion)
	{
		case 0x00: // WP5 
			return new WP5Header(input, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption);
		case 0x01: // ???
			WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
			return NULL;
		case 0x02: // WP6
			switch (minorVersion)
			{
				case 0x00:
					return new WP60Header(input, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption);
				default: // assume this header can be parsed by a WP61 header parser
					return new WP61Header(input, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption);
			}
			break;
		default:
			// unhandled file format
			WPD_DEBUG_MSG(("WordPerfect: Unsupported file format.\n"));
			return NULL;
	}

	return NULL;
}
