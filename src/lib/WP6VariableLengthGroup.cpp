/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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
#include "WP6CharacterGroup.h"
#include "WP6ColumnGroup.h"
#include "WP6EOLGroup.h"
#include "WP6ParagraphGroup.h"
#include "WP6TabGroup.h"
#include "WP6UnsupportedVariableLengthGroup.h"

#include "libwpd_internal.h"


WP6VariableLengthGroup::WP6VariableLengthGroup() :
	m_numPrefixIDs(0)
{
}

WP6VariableLengthGroup::~WP6VariableLengthGroup()
{
	if (m_numPrefixIDs > 0) 
		g_free(m_prefixIDs);
}

WP6VariableLengthGroup * WP6VariableLengthGroup::constructVariableLengthGroup(FILE *stream, guint8 groupID)
{
	switch (groupID)
	{
		case WP6_TOP_EOL_GROUP: 
			return new WP6EOLGroup(stream);
		case WP6_TOP_CHARACTER_GROUP:
			return new WP6CharacterGroup(stream);
		case WP6_TOP_COLUMN_GROUP:
			return new WP6ColumnGroup(stream);
		case WP6_TOP_PARAGRAPH_GROUP:
			return new WP6ParagraphGroup(stream);
		case WP6_TOP_TAB_GROUP:
			return new WP6TabGroup(stream);
		default:
			// this is an unhandled group, just skip it
			return new WP6UnsupportedVariableLengthGroup(stream);
	}
}

void WP6VariableLengthGroup::_read(FILE *stream)
{
	guint32 startPosition = ftell(stream);

	WPD_DEBUG_MSG(("WordPerfect: handling a variable length group\n"));
	
	WPD_CHECK_FILE_READ_ERROR(fread(&m_subGroup, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_size, sizeof(guint16), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_flags, sizeof(guint8), 1, stream), 1);

	if (m_flags & WP6_VARIABLE_GROUP_PREFIX_ID_BIT)
	{
		WPD_CHECK_FILE_READ_ERROR(fread(&m_numPrefixIDs, sizeof(guint8), 1, stream), 1);
		
		if (m_numPrefixIDs > 0)
		{
			m_prefixIDs = (guint16 *) g_malloc(sizeof(guint16) * m_numPrefixIDs);
			for (guint32 i = 0; i < m_numPrefixIDs; i++)
			{
				WPD_CHECK_FILE_READ_ERROR(fread(&m_prefixIDs[i], sizeof(guint16), 1, stream), 1);		
			}
		}	
	}	
	else
	{
		m_numPrefixIDs = 0;
	}
		
	WPD_CHECK_FILE_READ_ERROR(fread(&m_sizeNonDeletable, sizeof(guint16), 1, stream), 1);	
	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, sub_group: %i, size: %i, flags: %i, num_prefix_ids: %i, size_non_deletable: %i)\n", startPosition, m_subGroup, m_size, m_flags, m_numPrefixIDs, m_sizeNonDeletable));

	_readContents(stream);

	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, (startPosition + m_size - 1 - ftell(stream)), SEEK_CUR));
}

