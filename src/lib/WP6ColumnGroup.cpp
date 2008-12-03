/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP6ColumnGroup.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"

WP6ColumnGroup::WP6ColumnGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_margin(0),
	m_colType(0),
	m_numColumns(0),
	m_rowSpacing(0.0),
	m_isFixedWidth(),
	m_columnWidth()
{
	_read(input, encryption);
}

void WP6ColumnGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly	
	switch (getSubGroup())	
	{
		case 0: // Left Margin Set
		case 1: // Right Margin Set
			{
				m_margin = readU16(input, encryption);
				WPD_DEBUG_MSG(("WordPerfect: Read column group margin size (margin: %i)\n", m_margin));
			}
			break;
		case 2:
			{
				m_colType = readU8(input, encryption);
				uint32_t tmpRowSpacing = readU32(input, encryption);
				int16_t tmpRowSpacingIntegerPart = (int16_t)((tmpRowSpacing & 0xffff0000) >> 16);
				double tmpRowSpacingFractionalPart = (double)((double)(tmpRowSpacing & 0xffff)/(double)0x10000);
				m_rowSpacing = (double)tmpRowSpacingIntegerPart + tmpRowSpacingFractionalPart;
				m_numColumns = readU8(input, encryption);
				uint8_t tmpDefinition;
				uint16_t tmpWidth;
				if (m_numColumns > 1)
				{
					for (int i=0; i<((2*m_numColumns)-1); i++)
					{
						tmpDefinition = readU8(input, encryption);
						tmpWidth = readU16(input, encryption);
						if ((tmpDefinition & 0x01) == 0x01)
						{
							m_isFixedWidth.push_back(true);
							m_columnWidth.push_back((double)((double)tmpWidth/(double)WPX_NUM_WPUS_PER_INCH));
						}
						else
						{
							m_isFixedWidth.push_back(false);
							m_columnWidth.push_back((double)((double)tmpWidth/(double)0x10000));
						}	
						
					}
				}
				WPD_DEBUG_MSG(("WordPerfect: Column type: %d\n", m_colType & 0x03));
				WPD_DEBUG_MSG(("WordPerfect: Numer of columns: %d\n", m_numColumns));				
			}
			break;
		case 3: /* TODO: Column Border */
			{
			}
			break;
		default: /* something else we don't support, since it isn't in the docs */
			break;
	}
}

void WP6ColumnGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Column group\n"));

	if (getFlags() & 0x40)  // Ignore function flag
		return;
		
	switch (getSubGroup())
	{
		case 0: // Left Margin Set
			{						
				listener->marginChange(WPX_LEFT, m_margin);
			}
			break;
		case 1: // Right Margin Set
			{						
				listener->marginChange(WPX_RIGHT, m_margin);
			}
			break;
		case 2: // Define Text Columns
			{
				// number of columns = {0,1} means columns off
				if ((m_numColumns == 0) || (m_numColumns == 1))
				{
					listener->columnChange(NEWSPAPER, 1, m_columnWidth, m_isFixedWidth); // the value "1" is bugus, the false bool gives you all the information you need here
				} 
				else
				{
					switch (m_colType & 0x03)
					{
						case WP6_COLUMN_TYPE_NEWSPAPER:
							listener->columnChange(NEWSPAPER, m_numColumns, m_columnWidth, m_isFixedWidth);
							break;
						case WP6_COLUMN_TYPE_NEWSPAPER_VERTICAL_BALANCE:
							listener->columnChange(NEWSPAPER_VERTICAL_BALANCE, m_numColumns, m_columnWidth, m_isFixedWidth);
							break;
						case WP6_COLUMN_TYPE_PARALLEL:
							listener->columnChange(PARALLEL, m_numColumns, m_columnWidth, m_isFixedWidth);
							break;
						case WP6_COLUMN_TYPE_PARALLEL_PROTECT:
							listener->columnChange(PARALLEL_PROTECT, m_numColumns, m_columnWidth, m_isFixedWidth);
							break;
						default: // something else we don't support, since it isn't in the docs
							break;
					}
				}
			}
			break;
		case 3: // TODO: Column Border
			{
			}
			break;
		default: // something else we don't support, since it isn't in the docs
			break;
	}
}
