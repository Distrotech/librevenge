/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include <stdlib.h>
#include <glib.h>
#include "WPXStream.h"

class WPXHeader
{
 public:	
	WPXHeader(WPXInputStream *input, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion, guint16 documentEncryption);
	virtual ~WPXHeader();

	static WPXHeader * constructHeader(WPXInputStream *input);
		
	const guint32 getDocumentOffset() const { return m_documentOffset; }
	const guint8 getProductType() const { return m_productType; }
	const guint8 getFileType() const { return m_fileType; }
	const guint8 getMajorVersion() const { return m_majorVersion; }
	const guint8 getMinorVersion() const { return m_minorVersion; }
	const guint16 getDocumentEncryption() const { return m_documentEncryption; }

 private:	
	guint32 m_documentOffset;
	guint8 m_productType;
	guint8 m_fileType;
	guint8 m_majorVersion;
	guint8 m_minorVersion;
 	guint16 m_documentEncryption;		
};

#endif /* WPXHEADER_H */
