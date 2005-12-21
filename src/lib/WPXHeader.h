/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
 
#ifndef WPXHEADER_H
#define WPXHEADER_H

#include <stdlib.h>
#include "WPXStream.h"

class WPXHeader
{
 public:	
	WPXHeader(WPXInputStream *input, uint32_t documentOffset, uint8_t productType, uint8_t fileType, uint8_t majorVersion, uint8_t minorVersion, uint16_t documentEncryption);
	virtual ~WPXHeader();

	static WPXHeader * constructHeader(WPXInputStream *input);
		
	const uint32_t getDocumentOffset() const { return m_documentOffset; }
	const uint8_t getProductType() const { return m_productType; }
	const uint8_t getFileType() const { return m_fileType; }
	const uint8_t getMajorVersion() const { return m_majorVersion; }
	const uint8_t getMinorVersion() const { return m_minorVersion; }
	const uint16_t getDocumentEncryption() const { return m_documentEncryption; }

 private:	
	uint32_t m_documentOffset;
	uint8_t m_productType;
	uint8_t m_fileType;
	uint8_t m_majorVersion;
	uint8_t m_minorVersion;
 	uint16_t m_documentEncryption;		
};

#endif /* WPXHEADER_H */
