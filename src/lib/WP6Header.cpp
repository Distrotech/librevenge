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

#include "libwpd.h"
#include "WP6Header.h"
#include "WP6FileStructure.h" 
#include "libwpd_internal.h"

WP6Header::WP6Header(GsfInput * input)
	:	WPXHeader(input)
{
	guint16 documentEncrypted;

	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_HEADER_ENCRYPTION_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	m_documentEncryption = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_HEADER_INDEX_HEADER_POINTER_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	m_indexHeaderOffset = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_HEADER_DOCUMENT_SIZE_OFFSET - gsf_input_tell(input), G_SEEK_CUR));
	m_documentSize = *(const guint32 *)gsf_input_read(input, sizeof(guint32), NULL);

	WPD_DEBUG_MSG(("WordPerfect: Index Header Position = %i \n",(int)m_indexHeaderOffset));
	WPD_DEBUG_MSG(("WordPerfect: Document Pointer = %i \n",(int)getDocumentOffset()));
	WPD_DEBUG_MSG(("WordPerfect: Document End Position = %i \n",(int)m_documentSize));
	WPD_DEBUG_MSG(("WordPerfect: Document Encryption = %i \n",(int)m_documentEncryption));

	// TODO:
	
	/* we do not handle encrypted documents */
	/*if (m_documentEncryption != 0)
	  return FALSE;*/
	
	/* sanity check */
	/*if (documentOffset > m_iDocumentSize)
	  return FALSE;*/


	// read the Index Header (Header #0)
	// skip the Flags = 2 and the Reserved byte = 0
	guint16 numIndices;
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, m_indexHeaderOffset + WP6_INDEX_HEADER_NUM_INDICES_POSITION, G_SEEK_SET));
	m_numPrefixIndices = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	WPD_DEBUG_MSG(("WordPerfect: Number of Index Headers = %d \n",numIndices));

	// ignore the 10 reserved bytes that follow (jump to the offset of the Index Header #1, where we can resume parsing)
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, m_indexHeaderOffset + WP6_INDEX_HEADER_INDICES_POSITION, G_SEEK_SET));
}

WP6Header::~WP6Header()
{

}
