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

#include "WPXPart.h"
#include "WP6Part.h"
#include "WP6Header.h"
#include "WP6VariableLengthGroup.h"
#include "UT_libwpd2.h"

WP6Part::WP6Part(FILE * stream)
	: WPXPart(stream)
{
}

WP6Part * WP6Part::constructPart(FILE * stream, WPXParser * parser)
{
	guint8 readVal;

	while (ftell(stream) < (long)((WP6Header *)parser->getHeader())->m_iDocumentSize)
	{
		WPD_CHECK_FILE_READ_ERROR(fread(&readVal, sizeof(guint8), 1, stream), 1);
		if (readVal > 0 && readVal < 127)
		{	     
			if (readVal < 32)
			{
				// International Characters
				//WPD_CHECK_INTERNAL_ERROR(wp6_handle_character_insert(wp_internationalCharacterMapping[(read_val-1)]));
			}
			else
			{
				// normal ASCII characters
				parser->getLLListener()->insertCharacter( (guint32)readVal );
			}
		}
		else if (readVal >= 0xD0 && readVal <= 0xE2)
		{
			return new WP6VariableLengthGroup(stream);
		}/*			      
		else
		{
			for (i=0; i<WP6_NUM_FIXED_LENGTH_FUNCTIONS; i++)
			{
				if (wordperfect_fixed_length_functions[i]->byte == read_val)
				{
					WPD_CHECK_INTERNAL_ERROR(wordperfect_fixed_length_functions[i]->func(stream, wordperfect_state, 
													     wordperfect_parse_struct));
				}
			}
			
		}*/		
	}
	
	return NULL;

	/*
	
	
	guint8 read_val;
	unsigned int i;
	

	
	while (stream->position < (long)wordperfect_header->document_size) {
		WPD_CHECK_FILE_READ_ERROR(stream->wpd_callback_get_bytes(&read_val, sizeof(guint8), 1, stream), 1);
	  
		if(read_val > 0 && read_val < 127) {	     
			if (read_val < 32) { // International Characters
			      //WPD_CHECK_INTERNAL_ERROR(wp6_handle_character_insert(wp_internationalCharacterMapping[(read_val-1)]));
			}	     
			else { // normal ASCII characters 
				WPD_CHECK_INTERNAL_ERROR(wp6_character_insert(wordperfect_state, wordperfect_parse_struct, (guint32)read_val));
			}
		}
		else if (read_val >= 0xD0 && read_val <= 0xE2) { // variable length byte groups (careful that they don't go more than E2? FIXME)
			WPD_CHECK_INTERNAL_ERROR(wp6_parse_variable_length_group(stream, wordperfect_state, wordperfect_parse_struct, 
							wordperfect_variable_length_functions,
							read_val));
		}				      
		else {
			for (i=0; i<WP6_NUM_FIXED_LENGTH_FUNCTIONS; i++) {
				if (wordperfect_fixed_length_functions[i]->byte == read_val) {
					WPD_CHECK_INTERNAL_ERROR(wordperfect_fixed_length_functions[i]->func(stream, wordperfect_state, 
													     wordperfect_parse_struct));
				}
			}
			
		}
	}

	// flush any remaining text 
	WPD_CHECK_INTERNAL_ERROR(wp6_flush_text(wordperfect_state, wordperfect_parse_struct));
	
	WPD_DEBUG_MSG(("WordPerfect: Finished with document parse (position = %ld)\n",(long)stream->position));
	
	return TRUE;*/
}
