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

#include "WP6EOLGroup.h"

WP6VariableLengthGroup::WP6VariableLengthGroup(WPXParser * parser)
	: WP6Part(parser)
{
}

WP6VariableLengthGroup::~WP6VariableLengthGroup()
{
	if (m_iNumPrefixIDs > 0) 
		g_free(m_pPrefixIDs);
}

WP6VariableLengthGroup * WP6VariableLengthGroup::constructVariableLengthGroup(WPXParser * parser, guint8 groupID)
{
	switch (groupID)
	{
		case WP6_TOP_EOL_GROUP: 
			return new WP6EOLGroup(parser); //new WP6EOLGroup(parser);

		// Add the remaining cases here
		default:
			// this is an unhandled group, just skip it
			return new WP6VariableLengthGroup(parser);
	}
}

gboolean WP6VariableLengthGroup::parse()
{
	FILE * stream = _getParser()->getStream();
	guint32 startPosition = ftell(stream);

	WPD_DEBUG_MSG(("WordPerfect: handling a variable length group\n"));
	
	WPD_CHECK_FILE_READ_ERROR(fread(&m_iSubGroup, sizeof(guint8), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_iSize, sizeof(guint16), 1, stream), 1);
	WPD_CHECK_FILE_READ_ERROR(fread(&m_iFlags, sizeof(guint8), 1, stream), 1);

	if (m_iFlags & WP6_VARIABLE_GROUP_PREFIX_ID_BIT)
	{
		WPD_CHECK_FILE_READ_ERROR(fread(&m_iNumPrefixIDs, sizeof(guint8), 1, stream), 1);
		
		if (m_iNumPrefixIDs > 0)
		{
			m_pPrefixIDs = (guint16 **) g_malloc(sizeof(guint16) * m_iNumPrefixIDs);
			for (guint32 i = 0; i < m_iNumPrefixIDs; i++)
			{
				WPD_CHECK_FILE_READ_ERROR(fread(&m_pPrefixIDs[i], sizeof(guint16), 1, stream), 1);		
			}
		}	
	}	
	else
	{
		m_iNumPrefixIDs = 0;
	}
		
	WPD_CHECK_FILE_READ_ERROR(fread(&m_iSizeNonDeletable, sizeof(guint16), 1, stream), 1);	
	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, sub_group: %i, size: %i, flags: %i, num_prefix_ids: %i, size_non_deletable: %i)\n", startPosition, m_iSubGroup, m_iSize, m_iFlags, m_iNumPrefixIDs, m_iSizeNonDeletable));

	WPD_CHECK_INTERNAL_ERROR( _parseContents() );

	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, (startPosition + m_iSize - 1 - ftell(stream)), SEEK_CUR));
	
	return TRUE;
}

