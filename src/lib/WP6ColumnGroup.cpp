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

#include "WP6ColumnGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6ColumnGroup::WP6ColumnGroup(FILE *stream) :
	WP6VariableLengthGroup(),
	m_margin(0)
{
	_read(stream);
}

void WP6ColumnGroup::_readContents(FILE *stream)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly	
	switch (getSubGroup())	
	{
		case 0: // Left Margin Set
		case 1: // Right Margin Set
			{
				WPD_CHECK_FILE_READ_ERROR(fread(&m_margin, sizeof(guint16), 1, stream), 1);
				WPD_DEBUG_MSG(("WordPerfect: Read column group margin size (margin: %i)\n", m_margin));
			}
			break;
		case 2:
			{
				WPD_CHECK_FILE_READ_ERROR(fread(&m_colType, sizeof(guint8), 1, stream), 1);
				if (fread(&m_rowSpacing[0], sizeof(guint8), 4, stream) != 4*sizeof(guint8))
					/*return FALSE*/; // TODO: Throw exception of something
				WPD_CHECK_FILE_READ_ERROR(fread(&m_numColumns, sizeof(guint8), 1, stream), 1);
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

void WP6ColumnGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an Column group\n"));
	
	switch (getSubGroup())
	{
		case 0: // Left Margin Set
		case 1: // Right Margin Set
			{						
				llListener->marginChange(getSubGroup(), m_margin);
			}
			break;
		case 2: // TODO: Define Text Columns, Partially implemented.
			{
				// number of columns = {0,1} means columns off
				if ((m_numColumns == 0) || (m_numColumns == 1))
				{
					llListener->columnChange(1); // the value "1" is bugus, the FALSE boolean gives you all the information you need here
				} 
				else
				{
					// TODO: pass the column type to the listener
					switch (m_colType & 0x03)
					{
						case WP6_COLUMN_TYPE_NEWSPAPER:
							llListener->columnChange(m_numColumns);
							break;
						case WP6_COLUMN_TYPE_NEWSPAPER_VERTICAL_BALANCE:
							llListener->columnChange(m_numColumns);
							break;
						case WP6_COLUMN_TYPE_PARALLEL:
							llListener->columnChange(m_numColumns);
							break;
						case WP6_COLUMN_TYPE_PARALLEL_PROTECT:
							llListener->columnChange(m_numColumns);
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
