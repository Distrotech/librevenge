/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WPXSUBDOCUMENT_H
#define WPXSUBDOCUMENT_H

#include "WPXMemoryStream.h"
#include "libwpd_types.h"

class WPXListener;
class WPXEncryption;

class WPXSubDocument
{
public:
	WPXSubDocument(WPXInputStream *input, WPXEncryption *encryption, const unsigned dataSize);
	WPXSubDocument(uint8_t *streamData, const unsigned dataSize);
	virtual ~WPXSubDocument();
	WPXMemoryInputStream *getStream() const
	{
		return m_stream;
	}

private:
	WPXMemoryInputStream *m_stream;
	uint8_t *m_streamData;
	WPXSubDocument();
	WPXSubDocument(const WPXSubDocument &);
	WPXSubDocument &operator=(const WPXSubDocument &);

};
#endif /* WPXSUBDOCUMENT_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
