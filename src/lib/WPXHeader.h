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
 
#ifndef WPXHEADER_H
#define WPXHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

class WPXHeader
{
public:	
	WPXHeader(FILE * stream, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion);
	~WPXHeader() {};
		
	virtual gboolean parse() {};
		
	guint32 m_iDocumentOffset;
	guint8 m_iProductType;
	guint8 m_iFileType;
	guint8 m_iMajorVersion;
	guint8 m_iMinorVersion;
		
protected:
	FILE * m_pStream;
};

#endif /* WPXHEADER_H */
