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
#include "WPXHeader.h"
#include "WP6FileStructure.h"

WPXHeader::WPXHeader(FILE * stream, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion)
{
	m_pStream = stream;
	m_iDocumentOffset = documentOffset;
	m_iProductType = productType;
	m_iFileType = fileType;
	m_iMajorVersion = majorVersion;
	m_iMinorVersion = minorVersion;
	
	WPD_DEBUG_MSG(("WordPerfect: Product Type: %i File Type: %i Major Version: %i Minor Version: %i\n", 
					m_iProductType, m_iFileType, 
					m_iMajorVersion, m_iMinorVersion));		
}

