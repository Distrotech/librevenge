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
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include <math.h>
#include <ctype.h>
#include "WPXListener.h"
#include "WP6Listener.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP6PrefixData.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"

WP6Listener::WP6Listener() :
	m_prefixData(0)
{
}

const WP6PrefixDataPacket *WP6Listener::getPrefixDataPacket(const int prefixID) const
{
	if (m_prefixData)
		return m_prefixData->getPrefixDataPacket(prefixID);
	else
		return 0;
}

WPXString WP6Listener::getFontNameForPID(const int prefixID) const
{

	const WP6FontDescriptorPacket *fontDescriptorPacket =
	    dynamic_cast<const WP6FontDescriptorPacket *>(getPrefixDataPacket(prefixID));
	if (fontDescriptorPacket)
		return fontDescriptorPacket->getFontName();
	return WPXString();
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
