/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef _WP5HEADER_H
#define _WP5HEADER_H

#include "WPXHeader.h"

class WP5Header : public WPXHeader
{
 public:
	WP5Header(WPXInputStream * input, uint32_t documentOffset, uint8_t productType, uint8_t fileType, uint8_t majorVersion, uint8_t minorVersion, uint16_t documentEncryption);
	virtual ~WP5Header() {};
	
	const uint16_t getIndexHeaderOffset() const { return m_indexHeaderOffset; }
	const uint16_t getNumPrefixIndices() const { return m_numPrefixIndices; }
	const uint16_t getDocumentEncryption() const { return m_documentEncryption; }

protected:
	void _readndexInformation(WPXInputStream *input);
		
private:
	uint16_t m_indexHeaderOffset;
	uint16_t m_numPrefixIndices;
	uint16_t m_documentEncryption;
};
#endif /* _WP5HEADER_H  */
