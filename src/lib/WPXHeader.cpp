/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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
	
WPXHeader::WPXHeader(WPXInputStream *input, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion, guint16 documentEncryption) :
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

WPXHeader * WPXHeader::constructHeader(WPXInputStream *input)
{
	gchar fileMagic[4];
	/* check the magic */
	input->seek(WPX_HEADER_MAGIC_OFFSET - input->tell(), WPX_SEEK_CUR);
	for (int i=0; i<3 /* FIXME: && not EOF */; i++)
		fileMagic[i] = readU8(input);
	fileMagic[3] = '\0';
	
	if ( strcmp(fileMagic, "WPC") )
	{
		WPD_DEBUG_MSG(("WordPerfect: File magic is not equal to \"WPC\"!\n"));
		return NULL;
	}
	
	/* get the document pointer */
	input->seek(WPX_HEADER_DOCUMENT_POINTER_OFFSET - input->tell(), WPX_SEEK_CUR);
	guint32 documentOffset = readU32(input);

	/* get information on product types, file types, versions */
	input->seek(WPX_HEADER_PRODUCT_TYPE_OFFSET - input->tell(), WPX_SEEK_CUR);
	guint8 productType = readU8(input);
	guint8 fileType = readU8(input);
	guint8 majorVersion = readU8(input);
	guint8 minorVersion = readU8(input);
	
	input->seek(WPX_HEADER_ENCRYPTION_OFFSET, WPX_SEEK_SET);
	guint8 documentEncryption = readU16(input);		
	
	WPD_DEBUG_MSG(("WordPerfect: Product Type: %i File Type: %i Major Version: %i Minor Version: %i\n", 
					productType, fileType, 
					majorVersion, minorVersion));	
	WPD_DEBUG_MSG(("WordPerfect: Document Encryption = 0x%x \n",(int)documentEncryption));		


	switch (fileType)
	{
		case 0x0a:
			WPD_DEBUG_MSG(("WordPerect: Supported file type: \"WordPerfect Document\"\n"));
			break;
		default:
			WPD_DEBUG_MSG(("WordPerfect: Unsupported file type: %d\n", fileType));
			return NULL;
	}
	
	switch (majorVersion)
	{
		case 0x00: // WP5 
			return new WP5Header(input, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption);
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
			WPD_DEBUG_MSG(("WordPerfect: Unsupported major number: %d\n", majorVersion));
			return NULL;
	}

	return NULL;
}
