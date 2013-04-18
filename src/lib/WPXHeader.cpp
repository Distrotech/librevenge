/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPXHeader.h"
#include "WP3Header.h"
#include "WP5Header.h"
#include "WP6Header.h"
#include "WP60Header.h"
#include "WP61Header.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"

#include <string.h>

WPXHeader::WPXHeader(WPXInputStream * /* input */, WPXEncryption * /* encryption */, uint32_t documentOffset, uint8_t productType,
                     uint8_t fileType, uint8_t majorVersion, uint8_t minorVersion, uint16_t documentEncryption) :
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

WPXHeader *WPXHeader::constructHeader(WPXInputStream *input, WPXEncryption *encryption)
{
	WPD_DEBUG_MSG(("WPXHeader::constructHeader()\n"));

	char fileMagic[4] = { 0, 0, 0, 0 };
	/* check the magic */
	input->seek(WPX_HEADER_MAGIC_OFFSET, WPX_SEEK_SET);
	for (int i=0; i<3 && !input->atEOS(); i++)
		fileMagic[i] = (char)readU8(input, encryption);

	if ( strcmp(fileMagic, "WPC") )
	{
		WPD_DEBUG_MSG(("WordPerfect: File magic is not equal to \"WPC\"!\n"));
		return 0;
	}

	/* get the document pointer */
	input->seek(WPX_HEADER_DOCUMENT_POINTER_OFFSET, WPX_SEEK_SET);
	uint32_t documentOffset = readU32(input, encryption);

	/* get information on product types, file types, versions */
	input->seek(WPX_HEADER_PRODUCT_TYPE_OFFSET, WPX_SEEK_SET);
	uint8_t productType = readU8(input, encryption);
	uint8_t fileType = readU8(input, encryption);
	uint8_t majorVersion = readU8(input, encryption);
	uint8_t minorVersion = readU8(input, encryption);

	input->seek(WPX_HEADER_ENCRYPTION_OFFSET, WPX_SEEK_SET);
	uint16_t documentEncryption = readU16(input, encryption);

	WPD_DEBUG_MSG(("WordPerfect: Document Offset = 0x%x \n",documentOffset));
	WPD_DEBUG_MSG(("WordPerfect: Product Type: %i File Type: %i Major Version: %i Minor Version: %i\n",
	               productType, fileType,
	               majorVersion, minorVersion));
	WPD_DEBUG_MSG(("WordPerfect: Document Encryption = 0x%x \n",(int)documentEncryption));


	switch (fileType)
	{
	case 0x0a:
		WPD_DEBUG_MSG(("WordPerfect: Supported file type: \"WordPerfect Document\"\n"));
		switch (majorVersion)
		{
		case 0x00: // WP5
			documentEncryption = (uint16_t)(((documentEncryption & 0xff00) >> 8) | ((documentEncryption & 0x00ff) << 8));
			return new WP5Header(input, encryption, documentOffset, productType, fileType,
			                     majorVersion, minorVersion, documentEncryption);
		case 0x02: // WP6
			switch (minorVersion)
			{
			case 0x00:
				return new WP60Header(input, encryption, documentOffset, productType, fileType,
				                      majorVersion, minorVersion, documentEncryption);
			default: // assume this header can be parsed by a WP61 header parser
				return new WP61Header(input, encryption, documentOffset, productType, fileType,
				                      majorVersion, minorVersion, documentEncryption);
			}
		default:
			// unhandled file format
			WPD_DEBUG_MSG(("WordPerfect: Unsupported major number: %d\n", majorVersion));
			return 0;
		}
	case 0x2c:
		WPD_DEBUG_MSG(("WordPerfect: Supported file type: \"MAC WP 2.0 document\"\n"));
		switch (majorVersion)
		{
		case 0x02: // WP Mac 2.x
		case 0x03: // WP Mac 3.0-3.5
		case 0x04: // WP Mac 3.5e
			return new WP3Header(input, encryption, documentOffset, productType, fileType,
			                     majorVersion, minorVersion, documentEncryption);
		default:
			// unhandled file format
			WPD_DEBUG_MSG(("WordPerfect: Unsupported major number: %d\n", majorVersion));
			return 0;
		}
	default:
		WPD_DEBUG_MSG(("WordPerfect: Unsupported file type: %d\n", fileType));
		return 0;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
