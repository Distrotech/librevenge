/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
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
