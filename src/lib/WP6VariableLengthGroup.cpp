/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
	if (m_numPrefixIDs > 0) 
		g_free(m_prefixIDs);
}

WP6VariableLengthGroup * WP6VariableLengthGroup::constructVariableLengthGroup(GsfInput *input, guint8 groupID)
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

void WP6VariableLengthGroup::_read(GsfInput *input)
{
	guint32 startPosition = gsf_input_tell(input);
	
	m_subGroup = gsf_le_read_guint8(input);
	m_size = gsf_le_read_guint16(input);
	m_flags = gsf_le_read_guint8(input);

	if (m_flags & WP6_VARIABLE_GROUP_PREFIX_ID_BIT)
	{
		m_numPrefixIDs = gsf_le_read_guint8(input);
		
		if (m_numPrefixIDs > 0)
		{
			m_prefixIDs = (guint16 *) g_malloc(sizeof(guint16) * m_numPrefixIDs);
			for (guint32 i = 0; i < m_numPrefixIDs; i++)
			{
				m_prefixIDs[i] = gsf_le_read_guint16(input);		
			}
		}	
	}	
	else
	{
		m_numPrefixIDs = 0;
		m_prefixIDs = NULL;
	}
		
	m_sizeNonDeletable = gsf_le_read_guint16(input);	
	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, sub_group: %i, size: %i, flags: %i, num_prefix_ids: %i, size_non_deletable: %i)\n", startPosition, m_subGroup, m_size, m_flags, m_numPrefixIDs, m_sizeNonDeletable));

	_readContents(input);

	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, (startPosition + m_size - 1 - gsf_input_tell(input)), G_SEEK_CUR));
}
