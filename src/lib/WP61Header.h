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

#ifndef _WP61HEADER_H
#define _WP61HEADER_H
#include "WP6Header.h"

class WP61Header : public WP6Header
{
public:
	WP61Header(WPXInputStream *input, WPXEncryption *encryption, uint32_t documentOffset, uint8_t productType,
	           uint8_t fileType, uint8_t majorVersion, uint8_t minorVersion, uint16_t documentEncryption);
	~WP61Header();

	uint32_t getDocumentSize() const
	{
		return m_documentSize;
	}

private:
	uint32_t m_documentSize;
};
#endif /* _WP61HEADER_H  */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
