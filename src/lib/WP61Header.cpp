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
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP61Header.h"
#include "WP6FileStructure.h"
#include "libwpd_internal.h"

WP61Header::WP61Header(WPXInputStream *input, WPXEncryption *encryption, uint32_t documentOffset, uint8_t productType,
                       uint8_t fileType, uint8_t majorVersion, uint8_t minorVersion, uint16_t documentEncryption) :
	WP6Header(input, encryption, documentOffset, productType, fileType, majorVersion, minorVersion, documentEncryption),
	m_documentSize(0)
{
	input->seek(WP6_HEADER_DOCUMENT_SIZE_OFFSET, WPX_SEEK_SET);
	m_documentSize = readU32(input, encryption);

	WPD_DEBUG_MSG(("WordPerfect: Document End Position = 0x%x \n",(int)m_documentSize));

	_readIndexInformation(input, encryption);
}

WP61Header::~WP61Header()
{

}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
