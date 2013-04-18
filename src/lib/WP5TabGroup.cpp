/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5TabGroup.h"
#include "WP5FileStructure.h"
#include "libwpd_internal.h"
#include "WP5Listener.h"
#include "WPXFileStructure.h"

WP5TabGroup::WP5TabGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP5FixedLengthGroup(groupID),
	m_tabType(0),
	m_tabPosition(0.0)
{
	_read(input, encryption);
}

void WP5TabGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_tabType = readU8(input, encryption);
	input->seek(2, WPX_SEEK_CUR);
	uint16_t tmpTabPosition = readU16(input, encryption);
	m_tabPosition = (double)((double)tmpTabPosition/(double)WPX_NUM_WPUS_PER_INCH);
}

void WP5TabGroup::parse(WP5Listener *listener)
{
	listener->insertTab(m_tabType, m_tabPosition);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
