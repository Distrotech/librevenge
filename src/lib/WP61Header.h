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

#ifndef _WP61HEADER_H
#define _WP61HEADER_H
#include "WP6Header.h"

class WP61Header : public WP6Header
{
 public:
	WP61Header(GsfInput * input, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion, guint16 documentEncryption);
	virtual ~WP61Header();
	
	const guint32 getDocumentSize() const { return m_documentSize; }
		
 private:
	guint32 m_documentSize;
};
#endif /* _WP61HEADER_H  */
