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
#include "WP6FileStructure.h"
#include "libwpd.h"
#include "libwpd_internal.h"


WPXHeader::WPXHeader(GsfInput *input)
{
	gchar fileMagic[4];
	/* check the magic */
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_HEADER_MAGIC_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	for (int i=0; i<3; i++)
		fileMagic[i] = *(const gchar *)gsf_input_read(input, sizeof(guint8), NULL);
	fileMagic[3] = '\0';
	
	if ( strcmp(fileMagic, "WPC") )
	{
		WPD_DEBUG_MSG(("WordPerfect: File magic is not equal to \"WPC\"!\n"));
		throw FileException(); // FIXME: this is not a file exception!!
	}
	
	/* get the document pointer */
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_HEADER_DOCUMENT_POINTER_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	m_documentOffset = *(const guint32 *)gsf_input_read(input, sizeof(guint32), NULL);

	/* get information on product types, file types, versions */
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_HEADER_PRODUCT_TYPE_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	m_productType = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
	m_fileType = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
	m_majorVersion = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
	m_minorVersion = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
	
	WPD_DEBUG_MSG(("WordPerfect: Product Type: %i File Type: %i Major Version: %i Minor Version: %i\n", 
					m_productType, m_fileType, 
					m_majorVersion, m_minorVersion));		
}

