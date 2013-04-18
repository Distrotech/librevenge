/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
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

#include "WP42DefineColumnsGroup.h"
#include "WP42FileStructure.h"
#include "libwpd_internal.h"

WP42DefineColumnsGroup::WP42DefineColumnsGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP42MultiByteFunctionGroup(group),
	m_groupId(group),
	m_numColumns(0),
	m_isParallel(false),
	m_columnsDefinition()
{
	_read(input, encryption);
}

WP42DefineColumnsGroup::~WP42DefineColumnsGroup()
{
}

void WP42DefineColumnsGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	uint8_t maxNumColumns = 0;
	switch (m_groupId)
	{
	case WP42_DEFINE_COLUMNS_OLD_GROUP:
		input->seek(11, WPX_SEEK_CUR);
		maxNumColumns = 5;
		break;
	case WP42_DEFINE_COLUMNS_NEW_GROUP:
		input->seek(49, WPX_SEEK_CUR);
		maxNumColumns = 24;
		break;
	default:
		return;
	}
	uint8_t tmpNumColumns = readU8(input, encryption);
	m_numColumns = tmpNumColumns & 0x7F;
	if (m_numColumns > maxNumColumns)
		m_numColumns = maxNumColumns;
	m_isParallel = ((tmpNumColumns & 0x80) != 0);
	for (uint8_t i = 0; i<2*m_numColumns; i++)
		m_columnsDefinition.push_back(readU8(input, encryption));
}

void WP42DefineColumnsGroup::parse(WP42Listener * /* listener */)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an DefineColumns group\n"));
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
