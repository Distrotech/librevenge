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

#include "WP6EOLGroup.h"
#include "WP6LLListener.h"
#include "WPXHLListener.h"

WP6EOLGroup::WP6EOLGroup(FILE *stream) :
	WP6VariableLengthGroup()
{
	_read(stream);
}

void WP6EOLGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an EOL group\n"));
	   
	/* WL FIXME: am I handling all the special cases properly here? quite possibly not */
	//if(!wordperfect_state->undo_on)/* && m_paragraphStyleState != beginPart2BeforeNumbering && 
		//			m_paragraphStyleState != beginPart2Numbering && 
				//	m_paragraphStyleState != end) */
	{
		switch(getSubGroup())
		{
			case 0: // 0x00 (beginning of file)
				break; // ignore
			case 1: // 0x01 (soft EOL)
			case 2: // 0x02 (soft EOC) 
			case 3: // 0x03 (soft EOC at EOP) 
			case 20: // 0x014 (deletable soft EOL)
			case 21: // 0x15 (deletable soft EOC) 
			case 22: // 0x16 (deleteable soft EOC at EOP)
				llListener->insertCharacter((guint16) ' ');
				break;
			case 4: // 0x04 (hard end-of-line)
			case 5: // 0x05 (hard EOL at EOC) 
			case 6: // 0x06 (hard EOL at EOP)
			case 23: // 0x17 (deletable hard EOL)
			case 24: // 0x18 (deletable hard EOL at EOC)
			case 25: // 0x19 (deletable hard EOL at EOP)
				llListener->insertEOL();
				break;
			case WP6_EOL_CHARACTER_HARD_END_OF_COLUMN: // 0x07 (hard end of column)
				llListener->insertBreak(WPX_COLUMN_BREAK);
				break;
			case 9: // hard EOP
			case 28: // deletable hard EOP
				{ 
					/*
					if (wordperfect_state->text_buf->len > 0)
					{			
					WPD_CHECK_INTERNAL_ERROR(wp6_flush_text(wordperfect_state, wordperfect_parse_struct));
					}
					UT_UCS4Char ucs = UCS_FF;
					X_CheckDocumentError(getDoc()->appendSpan(&ucs,1));
					*/
				}
				break;
			case 0x0A: // Table Cell
				{
					/*
					UT_DEBUGMSG(("WordPerfect: Table Cell\n"));
					
					// check if a previous cell exists which should be closed first
					if (m_iCurrentTableColumn != -1 && m_cellExists)
					{
						X_CheckWordPerfectError(_handleHardEndOfLine());
						X_CheckWordPerfectError(_flushText()); // make sure the hard end of line goes through
						X_CheckDocumentError(getDoc()->appendStrux(PTX_EndCell, NULL));
					}
				      
					m_iCurrentTableColumn++;
				      
					int rowSpan = 1;
					int colSpan = 1;
					bool boundAbove = false;
					bool boundBeside = false;
					X_CheckWordPerfectError(_getCellSpans(startPosition, size, rowSpan, colSpan, boundAbove, boundBeside));
					
					if (!boundAbove && !boundBeside)
					{
						m_cellExists = true;
						UT_String propBuffer;
						UT_String_sprintf(propBuffer, "left-attach:%d; right-attach:%d; top-attach:%d; bot-attach:%d", m_iCurrentTableColumn, m_iCurrentTableColumn+colSpan, m_iCurrentTableRow, m_iCurrentTableRow+rowSpan);
						UT_DEBUGMSG(("WordPerfect: Inserting a Cell definition: %s\n", propBuffer.c_str()));
						* 
						m_iCurrentTableColumn+=(colSpan-1);
						* 
						XML_Char* pProps = "props";
						const XML_Char* propsArray[3];
						propsArray[0] = pProps;
						propsArray[1] = propBuffer.c_str();
						propsArray[2] = NULL;
						* 
						X_CheckDocumentError(getDoc()->appendStrux(PTX_SectionCell, propsArray));
					}
					else
					{			     
						m_cellExists = false;			     
					}
				       */			     
				}
				break;
			case 0x0B: // Table Row and Cell
				{
					/*
					UT_DEBUGMSG(("WordPerfect: Table Row and Cell\n"));
					
					// check if a previous cell exists which should be closed first
					if (m_iCurrentTableColumn != -1 && m_cellExists)
					{
						X_CheckWordPerfectError(_handleHardEndOfLine());
						X_CheckWordPerfectError(_flushText()); // make sure the hard end of line goes through
						X_CheckDocumentError(getDoc()->appendStrux(PTX_EndCell, NULL));
					}
					m_iCurrentTableRow++;
					
					m_iCurrentTableColumn = 0;
					
					int rowSpan = 1;
					int colSpan = 1;
					bool boundAbove = false;
					bool boundBeside = false;
					X_CheckWordPerfectError(_getCellSpans(startPosition, size, rowSpan, colSpan, boundAbove, boundBeside));
					
					if (!boundAbove && !boundBeside)
					{		
						m_cellExists = true;
						UT_String propBuffer;
						UT_String_sprintf(propBuffer, "left-attach:%d; right-attach:%d; top-attach:%d; bot-attach:%d", m_iCurrentTableColumn, m_iCurrentTableColumn+colSpan, m_iCurrentTableRow, m_iCurrentTableRow+rowSpan);
						UT_DEBUGMSG(("WordPerfect: Inserting a Cell definition: %s\n", propBuffer.c_str()));
						
						m_iCurrentTableColumn+=(colSpan-1);
						
						XML_Char* pProps = "props";
						const XML_Char* propsArray[3];
						propsArray[0] = pProps;
						propsArray[1] = propBuffer.c_str();
						propsArray[2] = NULL;
						
						X_CheckDocumentError(getDoc()->appendStrux(PTX_SectionCell, propsArray));
					}
					else
					{			     
						m_cellExists = false;			     
					}
					*/
				}
				break;
			case 0x11: // Table Off
				/*
				UT_DEBUGMSG(("WordPerfect: TABLE OFF\n"));
				if (m_iCurrentTableColumn != -1 && m_cellExists)
				{
					X_CheckWordPerfectError(_handleHardEndOfLine());
					X_CheckWordPerfectError(_flushText()); // make sure the hard end of line goes through
					X_CheckDocumentError(getDoc()->appendStrux(PTX_EndCell, NULL));
				}
				X_CheckDocumentError(getDoc()->appendStrux(PTX_EndTable, NULL));
				*/
				break;
			    
			default: // something else we don't support yet
				break;
		}
	}
}
