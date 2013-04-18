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

#include "WP6SubDocument.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6SubDocument::WP6SubDocument(uint8_t *streamData, const unsigned dataSize) :
	WPXSubDocument(streamData, dataSize)
{
}

void WP6SubDocument::parse(WP6Listener *listener) const
{
	WPXMemoryInputStream *tmpStream = getStream();
	if (!tmpStream)
		return;
	tmpStream->seek(0, WPX_SEEK_SET);
	WP6Parser::parseDocument(tmpStream, 0, listener);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
