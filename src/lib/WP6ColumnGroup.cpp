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

WP6ColumnGroup::WP6ColumnGroup(FILE *stream) :
	WP6VariableLengthGroup(),
	m_margin(0)
{
	_read(stream);
}

void WP6ColumnGroup::_readContents(FILE *stream)
{
	WPD_CHECK_FILE_READ_ERROR(fread(&m_margin, sizeof(guint16), 1, stream), 1);
	WPD_DEBUG_MSG(("WordPerfect: Read column group margin size (margin: %i)\n", m_margin));

}

void WP6ColumnGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an Column group\n"));
	
	switch (getSubGroup()) {
	case 0: // Left Margin Set
	case 1: // Right Margin Set
		{						
			llListener->marginChange(getSubGroup(), m_margin);
		}
		break;
#if 0
	case 2: /* TODO: Define Text Columns, Partially implemented. */
			  {
				  guint8 col_type;
				  guint8 number_of_columns;
				  guint8 row_spacing[4]; // a WP SPacing type variable, which is 4 bytes
				  
				  WPD_CHECK_FILE_READ_ERROR(stream->wpd_callback_get_bytes(&col_type, sizeof(guint8), 1, stream), 1);
				  if (stream->wpd_callback_get_bytes(&row_spacing[0], sizeof(guint8), 4, stream) != 4*sizeof(guint8))
				    return FALSE;
				  WPD_CHECK_FILE_READ_ERROR(stream->wpd_callback_get_bytes(&number_of_columns, sizeof(guint8), 1, stream), 1);
				  
				  WPD_DEBUG_MSG(("WordPerfect: Column type: %d\n", col_type & 0x03));
				  WPD_DEBUG_MSG(("WordPerfect: Column number: %d\n", number_of_columns));
				  
				  // number of columns = {0,1} means columns off
				  WPD_CHECK_INTERNAL_ERROR(wp6_flush_text(wordperfect_state, wordperfect_parse_struct));
				  if ((number_of_columns == 0) || (number_of_columns == 1)) {
					  wordperfect_parse_struct->wpd_callback_columns_off(wordperfect_parse_struct);
				  } 
				  else {
					  switch (col_type & 0x03) {
					   case WP6_COLUMN_TYPE_NEWSPAPER:
						  wordperfect_parse_struct->wpd_callback_columns_on(wordperfect_parse_struct, newspaper, number_of_columns);
						  break;
					   case WP6_COLUMN_TYPE_NEWSPAPER_VERTICAL_BALANCE:
						  wordperfect_parse_struct->wpd_callback_columns_on(wordperfect_parse_struct, newspaper_vertical_balance, number_of_columns);
						  break;
					   case WP6_COLUMN_TYPE_PARALLEL:
						  wordperfect_parse_struct->wpd_callback_columns_on(wordperfect_parse_struct, parallel, number_of_columns);
						  break;
					   case WP6_COLUMN_TYPE_PARALLEL_PROTECT:
						  wordperfect_parse_struct->wpd_callback_columns_on(wordperfect_parse_struct, parallel_protect, number_of_columns);
						  break;
					    }
				  }
			  }
			break;
		 case 3: /* TODO: Column Border */
			break;
#endif
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}
