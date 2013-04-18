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
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP5TableEOPGroup.h"
#include "WP5FileStructure.h"
#include "WP5Listener.h"
#include "libwpd_internal.h"

WP5TableEOPGroup::WP5TableEOPGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup()
{
	_read(input, encryption);
}

WP5TableEOPGroup::~WP5TableEOPGroup()
{
}

void WP5TableEOPGroup::_readContents(WPXInputStream * /* input */, WPXEncryption * /* encryption */)
{
	switch (getSubGroup())
	{
	case WP5_TABLE_EOP_GROUP_BEGINNING_OF_ROW_AT_EOP:
		break;
	case WP5_TABLE_EOP_GROUP_TABLE_OFF_AT_EOP:
		break;
	case WP5_TABLE_EOP_GROUP_BEGINNING_OF_ROW_AT_HARD_EOP:
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP5TableEOPGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Table EOP group\n"));

	switch (getSubGroup())
	{
	case WP5_TABLE_EOP_GROUP_BEGINNING_OF_ROW_AT_EOP:
		listener->insertRow(0, true, false);
		break;
	case WP5_TABLE_EOP_GROUP_TABLE_OFF_AT_EOP:
		listener->endTable();
		break;
	case WP5_TABLE_EOP_GROUP_BEGINNING_OF_ROW_AT_HARD_EOP:
		listener->insertRow(0, true, false);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
