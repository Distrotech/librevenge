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
#include "WP6UnsupportedVariableLengthGroup.h"

#include "libwpd_internal.h"


WP6VariableLengthGroup::WP6VariableLengthGroup() :
	m_numPrefixIDs(0),
	m_prefixIDs(NULL)
{
}

WP6VariableLengthGroup::~WP6VariableLengthGroup()
{
	if (m_numPrefixIDs > 0 && m_prefixIDs)
		delete [] m_prefixIDs;
}

WP6VariableLengthGroup * WP6VariableLengthGroup::constructVariableLengthGroup(WPXInputStream *input, const uint8_t groupID)
{
	switch (groupID)
	{
	case WP6_TOP_PAGE_GROUP:
		return new WP6PageGroup(input);
	case WP6_TOP_EOL_GROUP: 
		return new WP6EOLGroup(input);
	case WP6_TOP_CHARACTER_GROUP:
		return new WP6CharacterGroup(input);
	case WP6_TOP_COLUMN_GROUP:
		return new WP6ColumnGroup(input);
	case WP6_TOP_PARAGRAPH_GROUP:
		return new WP6ParagraphGroup(input);
	case WP6_TOP_FOOTNOTE_ENDNOTE_GROUP:
		return new WP6FootnoteEndnoteGroup(input);
	case WP6_TOP_HEADER_FOOTER_GROUP:
		return new WP6HeaderFooterGroup(input);
	case WP6_TOP_DISPLAY_NUMBER_REFERENCE_GROUP:
		return new WP6DisplayNumberReferenceGroup(input);
	case WP6_TOP_STYLE_GROUP:
		return new WP6StyleGroup(input);
	case WP6_TOP_TAB_GROUP:
		return new WP6TabGroup(input);
	default:
		// this is an unhandled group, just skip it
		return new WP6UnsupportedVariableLengthGroup(input);
	}
}

bool WP6VariableLengthGroup::isGroupConsistent(WPXInputStream *input, const uint8_t groupID)
{
	uint32_t startPosition = input->tell();

	try
	{
		input->seek(1, WPX_SEEK_CUR);
		uint16_t size = readU16(input);

		if (input->seek((startPosition + size - 4 - input->tell()), WPX_SEEK_CUR) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (size != readU16(input))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (groupID != readU8(input))
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

void WP6VariableLengthGroup::_read(WPXInputStream *input)
{
	uint32_t startPosition = input->tell();
	
	m_subGroup = readU8(input);
	m_size = readU16(input);
	m_flags = readU8(input);

	if (m_flags & WP6_VARIABLE_GROUP_PREFIX_ID_BIT)
	{
		m_numPrefixIDs = readU8(input);
		
		if (m_numPrefixIDs > 0)
		{
			m_prefixIDs = new uint16_t[m_numPrefixIDs];
			for (uint32_t i = 0; i < m_numPrefixIDs; i++)
			{
				m_prefixIDs[i] = readU16(input);		
			}
		}	
	}	
	else
	{
		m_numPrefixIDs = 0;
		m_prefixIDs = NULL;
	}
		
	m_sizeNonDeletable = readU16(input);
	if (m_sizeNonDeletable > m_size)
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected, bailing out!\n"));
		throw FileException();
	}	

	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, sub_group: %i, size: %i, flags: %i, num_prefix_ids: %i, size_non_deletable: %i)\n", startPosition, m_subGroup, m_size, m_flags, m_numPrefixIDs, m_sizeNonDeletable));

	_readContents(input);

	input->seek((startPosition + m_size - 4 - input->tell()), WPX_SEEK_CUR);

	if (m_size != readU16(input))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected, bailing out!\n"));
		throw FileException();
	}

	input->seek((startPosition + m_size - 1 - input->tell()), WPX_SEEK_CUR);
}
