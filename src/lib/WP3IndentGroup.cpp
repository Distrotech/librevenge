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

#include "WP3IndentGroup.h"
#include "WP3FileStructure.h"
#include "WP3Listener.h"
#include "libwpd_internal.h"

WP3IndentGroup::WP3IndentGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID) :
	WP3FixedLengthGroup(groupID),
	m_subGroup(0),
	m_offset(0.0)
{
	_read(input, encryption);
}

void WP3IndentGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	m_subGroup = readU8(input, encryption);
	m_offset = fixedPointToDouble(readU32(input, encryption, true));
}

void WP3IndentGroup::parse(WP3Listener *listener)
{
	switch (m_subGroup)
	{
	case WP3_INDENT_GROUP_LEFT_INDENT:
		listener->leftIndent(m_offset);
		break;
	case WP3_INDENT_GROUP_LEFT_RIGHT_INDENT:
		listener->leftRightIndent(m_offset);
		break;
	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
