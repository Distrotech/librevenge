/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#include "WP5TableEOPGroup.h"
#include "WP5FileStructure.h"
#include "libwpd_internal.h"

WP5TableEOPGroup::WP5TableEOPGroup(WPXInputStream *input) :
	WP5VariableLengthGroup()
{
	_read(input);
}

WP5TableEOPGroup::~WP5TableEOPGroup()
{
}

void WP5TableEOPGroup::_readContents(WPXInputStream *input)
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
