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

#include "WP5DefinitionGroup.h"
#include "WP5Listener.h"
#include "libwpd_internal.h"

WP5DefinitionGroup_DefineTablesSubGroup::WP5DefinitionGroup_DefineTablesSubGroup(WPXInputStream *input, WPXEncryption *encryption, uint16_t subGroupSize) :
	WP5VariableLengthGroup_SubGroup(),
	m_position(0),
	m_numColumns(0),
	m_leftOffset(0),
	m_leftGutter(0),
	m_rightGutter(0)
{
	long startPosition = input->tell();
	// Skip useless old values to read the old column number
	input->seek(2, WPX_SEEK_CUR);
	m_numColumns = readU16(input, encryption);
	// Skip to new values
	input->seek(20+(5*m_numColumns), WPX_SEEK_CUR);
	// Read the new values
	uint8_t tmpFlags = readU8(input, encryption);
	m_position = tmpFlags & 0x07;
	input->seek(1, WPX_SEEK_CUR);
	m_numColumns = readU16(input, encryption);
	input->seek(4, WPX_SEEK_CUR);
	m_leftGutter = readU16(input, encryption);
	m_rightGutter = readU16(input, encryption);
	input->seek(10, WPX_SEEK_CUR);
	m_leftOffset = readU16(input, encryption);
	int i;
	if ((m_numColumns > 32) || ((input->tell() - startPosition + m_numColumns*5) > (subGroupSize - 4)))
		throw FileException();
	for (i=0; i < m_numColumns; i++)
	{
		if (input->atEOS())
			throw FileException();
		m_columnWidth[i] = readU16(input, encryption);
	}
	for (i=0; i < m_numColumns; i++)
	{
		if (input->atEOS())
			throw FileException();
		m_attributeBits[i] = readU16(input, encryption);
	}
	for (i=0; i < m_numColumns; i++)
	{
		if (input->atEOS())
			throw FileException();
		m_columnAlignment[i] = readU8(input, encryption);
	}
}

void WP5DefinitionGroup_DefineTablesSubGroup::parse(WP5Listener *listener)
{
	// Since there are no nested tables in WP5, a new table definition automatically closes previous table
	listener->endTable();

	listener->defineTable(m_position, m_leftOffset);
	for (int i=0; i < m_numColumns && i < 32; i++)
		listener->addTableColumnDefinition(m_columnWidth[i], m_leftGutter, m_rightGutter, m_attributeBits[i], m_columnAlignment[i]);
	listener->startTable();
}


WP5DefinitionGroup::WP5DefinitionGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup(),
	m_subGroupData(0)
{
	_read(input, encryption);
}

WP5DefinitionGroup::~WP5DefinitionGroup()
{
	if (m_subGroupData)
		delete m_subGroupData;
}

void WP5DefinitionGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch(getSubGroup())
	{
	case WP5_TOP_DEFINITION_GROUP_DEFINE_TABLES:
		m_subGroupData = new WP5DefinitionGroup_DefineTablesSubGroup(input, encryption, getSize());
		break;
	default:
		break;
	}
}

void WP5DefinitionGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Definition group\n"));

	switch(getSubGroup())
	{
	case WP5_TOP_DEFINITION_GROUP_DEFINE_TABLES:
		m_subGroupData->parse(listener);
		break;
	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
