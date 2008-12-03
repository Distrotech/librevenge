/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include <math.h>
#include "WP3DefinitionGroup.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"
#include "WP3Listener.h"

WP3DefinitionGroup::WP3DefinitionGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_colType(0),
	m_numColumns(0),
	m_isFixedWidth(),
	m_columnWidth()
{
	_read(input, encryption);
}

WP3DefinitionGroup::~WP3DefinitionGroup()
{
	// fixme delete the font name
}

void WP3DefinitionGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP3_DEFINITION_GROUP_SET_COLUMNS:
		{
			uint8_t tmpColType = readU8(input, encryption);
			if (tmpColType)
			{
				uint8_t tmpNumColumns = readU8(input, encryption);
				if (tmpNumColumns)
					input->seek(((2*tmpNumColumns) - 1), WPX_SEEK_CUR);
			}

			m_colType = readU8(input, encryption);
			if (!m_colType)
			{
				m_numColumns = 1;
				m_isFixedWidth.clear();
				m_columnWidth.clear();
			}
			else
			{
				m_numColumns = readU8(input, encryption);
				if (m_numColumns > 1)
				{
					for (int i=0; i<((2*m_numColumns)-1); i++)
					{
						if (i%2)
						{
							uint32_t tmpSpaceBetweenColumns = readU32(input, encryption, true);
							m_isFixedWidth.push_back(true);
							m_columnWidth.push_back((double)((double)fixedPointToWPUs(tmpSpaceBetweenColumns)/(double)WPX_NUM_WPUS_PER_INCH));
						}
						else
						{
							uint16_t tmpSizeOfColumn = readU16(input, encryption, true);
							m_isFixedWidth.push_back(false);
							m_columnWidth.push_back((double)((double)tmpSizeOfColumn/(double)0x10000));
						}
					}
				}
			}
		}
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3DefinitionGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Definition group\n"));

	switch (getSubGroup())
	{
	case WP3_DEFINITION_GROUP_SET_COLUMNS:
		// number of columns = {0,1} means columns off
		if ((m_numColumns <= 1))
		{
			listener->columnChange(NEWSPAPER, 1, m_columnWidth, m_isFixedWidth); // the value "1" is bugus, the false bool gives you all the information you need here
		} 
		else
		{
			switch (m_colType)
			{
				case WP3_COLUMN_TYPE_NEWSPAPER:
					listener->columnChange(NEWSPAPER, m_numColumns, m_columnWidth, m_isFixedWidth);
					break;
				case WP3_COLUMN_TYPE_PARALLEL:
					listener->columnChange(PARALLEL, m_numColumns, m_columnWidth, m_isFixedWidth);
					break;
				case WP3_COLUMN_TYPE_EXTENDED:
					listener->columnChange(PARALLEL_PROTECT, m_numColumns, m_columnWidth, m_isFixedWidth);
					break;
				default: // something else we don't support, since it isn't in the docs
					break;
			}
		}
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
