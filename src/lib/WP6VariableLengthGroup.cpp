/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP6VariableLengthGroup.h"
#include "WP6PageGroup.h"
#include "WP6CharacterGroup.h"
#include "WP6ColumnGroup.h"
#include "WP6EOLGroup.h"
#include "WP6ParagraphGroup.h"
#include "WP6FootnoteEndnoteGroup.h"
#include "WP6HeaderFooterGroup.h"
#include "WP6DisplayNumberReferenceGroup.h"
#include "WP6StyleGroup.h"
#include "WP6TabGroup.h"
#include "WP6BoxGroup.h"
#include "WP6UnsupportedVariableLengthGroup.h"
#include "WP6SetNumberGroup.h"
#include "WP6NumberingMethodGroup.h"

#include "libwpd_internal.h"


WP6VariableLengthGroup::WP6VariableLengthGroup() :
	m_subGroup(0),
	m_size(0),
	m_flags(0),
	m_numPrefixIDs(0),
	m_prefixIDs(0),
	m_sizeNonDeletable(0),
	m_sizeDeletable(0)
{
}

WP6VariableLengthGroup::~WP6VariableLengthGroup()
{
	if (m_numPrefixIDs > 0 && m_prefixIDs)
		delete [] m_prefixIDs;
}

WP6VariableLengthGroup * WP6VariableLengthGroup::constructVariableLengthGroup(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	switch (groupID)
	{
	case WP6_TOP_NUMBERING_METHOD_GROUP:
		return new WP6NumberingMethodGroup(input, encryption);
	case WP6_TOP_SET_NUMBER_GROUP:
		return new WP6SetNumberGroup(input, encryption);
	case WP6_TOP_PAGE_GROUP:
		return new WP6PageGroup(input, encryption);
	case WP6_TOP_EOL_GROUP: 
		return new WP6EOLGroup(input, encryption);
	case WP6_TOP_CHARACTER_GROUP:
		return new WP6CharacterGroup(input, encryption);
	case WP6_TOP_COLUMN_GROUP:
		return new WP6ColumnGroup(input, encryption);
	case WP6_TOP_PARAGRAPH_GROUP:
		return new WP6ParagraphGroup(input, encryption);
	case WP6_TOP_FOOTNOTE_ENDNOTE_GROUP:
		return new WP6FootnoteEndnoteGroup(input, encryption);
	case WP6_TOP_HEADER_FOOTER_GROUP:
		return new WP6HeaderFooterGroup(input, encryption);
	case WP6_TOP_DISPLAY_NUMBER_REFERENCE_GROUP:
		return new WP6DisplayNumberReferenceGroup(input, encryption);
	case WP6_TOP_STYLE_GROUP:
		return new WP6StyleGroup(input, encryption);
	case WP6_TOP_TAB_GROUP:
		return new WP6TabGroup(input, encryption);
	case WP6_TOP_BOX_GROUP:
		return new WP6BoxGroup(input, encryption);
	default:
		// this is an unhandled group, just skip it
		return new WP6UnsupportedVariableLengthGroup(input, encryption);
	}
}

bool WP6VariableLengthGroup::isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	uint32_t startPosition = input->tell();

	try
	{
		input->seek(1, WPX_SEEK_CUR);
		uint16_t size = readU16(input, encryption);

		if (input->seek((startPosition + size - 4), WPX_SEEK_SET) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (size != readU16(input, encryption))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (groupID != readU8(input, encryption))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		
		input->seek(startPosition, WPX_SEEK_SET);
		return true;
	}
	catch(...)
	{
		input->seek(startPosition, WPX_SEEK_SET);
		return false;
	}
}

void WP6VariableLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	uint32_t startPosition = input->tell();
	
	m_subGroup = readU8(input, encryption);
	if ((m_size = readU16(input, encryption)) == 0)
		throw FileException();
	m_flags = readU8(input, encryption);

	if (m_flags & WP6_VARIABLE_GROUP_PREFIX_ID_BIT)
	{
		m_numPrefixIDs = readU8(input, encryption);
		
		if (m_numPrefixIDs > 0)
		{
			m_prefixIDs = new uint16_t[m_numPrefixIDs];
			for (uint32_t i = 0; i < m_numPrefixIDs; i++)
			{
				m_prefixIDs[i] = readU16(input, encryption);		
			}
		}	
	}	
	else
	{
		m_numPrefixIDs = 0;
		m_prefixIDs = 0;
	}
		
	m_sizeNonDeletable = readU16(input, encryption);
	if (m_sizeNonDeletable > m_size || (m_sizeNonDeletable & 0x8000))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected, bailing out!\n"));
		throw FileException();
	}
	
	uint32_t tmpPosition = input->tell();
	input->seek(m_sizeNonDeletable, WPX_SEEK_CUR);
	m_sizeDeletable = (uint16_t)(startPosition + m_size - 4 - input->tell());
	input->seek(tmpPosition, WPX_SEEK_SET);

	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, sub_group: %i, size: %i, flags: %i, num_prefix_ids: %i, size_non_deletable: %i, size_deletable: %i)\n", startPosition, m_subGroup, m_size, m_flags, m_numPrefixIDs, m_sizeNonDeletable, m_sizeDeletable));

	_readContents(input, encryption);

	input->seek((startPosition + m_size - 4), WPX_SEEK_SET);

	if (m_size != readU16(input, encryption))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected, bailing out!\n"));
		throw FileException();
	}

	input->seek((startPosition + m_size - 1), WPX_SEEK_SET);
}
