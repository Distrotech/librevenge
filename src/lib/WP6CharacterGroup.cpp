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

#include "WP6CharacterGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6CharacterGroup::WP6CharacterGroup(FILE *stream) :
	WP6VariableLengthGroup()
{
	_read(stream);
}

void WP6CharacterGroup::_readContents(FILE *stream)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())	
	{
	}
}

void WP6CharacterGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Character group\n"));
	
	switch (getSubGroup())
	{
		/*case WP_CHARACTER_GROUP_FONT_FACE_CHANGE:
			X_CheckWordPerfectError(_handleFontFaceChange());
			break;     
		case WP_CHARACTER_GROUP_FONT_SIZE_CHANGE:
			X_CheckWordPerfectError(_handleFontSizeChange());
			break;
		case WP_CHARACTER_GROUP_PARAGRAPH_NUMBER_ON:
			if (m_paragraphStyleState == beginBeforeNumbering || m_paragraphStyleState == styleBody)
			{
				m_bPutativeListHasParagraphNumber = true; // indicates that a paragraph style really does encapsulate a list
				UT_uint16 nonDeletableInfoSize;
				UT_uint16 outlineStyleHash;
				UT_uint8 flag;
				X_CheckFileReadElementError(fread(&nonDeletableInfoSize, sizeof(UT_uint16), 1, m_importFile));
				X_CheckFileReadElementError(fread(&m_currentOutlineHash, sizeof(UT_uint16), 1, m_importFile));
				X_CheckFileReadElementError(fread(&m_currentListLevel, sizeof(UT_uint8), 1, m_importFile));
				X_CheckFileReadElementError(fread(&flag, sizeof(UT_uint8), 1, m_importFile));
				UT_DEBUGMSG(("WordPerfect: LISTS Paragraph Number ON (outlineStyleHash: %i, level: %i, flag: %i)\n", (int)m_currentOutlineHash, (int)m_currentListLevel, (int) flag));
				// first, find the correct list definition
				WordPerfectListDefinition *listDefinition = _getListDefinition(m_currentOutlineHash);
				if( listDefinition == NULL )
				return UT_ERROR;
				xxx_UT_DEBUGMSG(("WordPerfect: %i %i\n", listDefinition->getListID(0), listDefinition->getListID(1)));
				X_CheckWordPerfectError(_updateDocumentListDefinition(listDefinition, m_currentListLevel));
				if (listDefinition->isLevelNumbered(m_currentListLevel))
				listDefinition->incrementLevelNumber(m_currentListLevel);
				m_currentListTag = UT_rand();
				m_paragraphStyleState = beginNumbering;
			}        
			break;
		case WP_CHARACTER_GROUP_PARAGRAPH_NUMBER_OFF:
			UT_DEBUGMSG(("WordPerfect: LISTS Paragraph Number OFF\n"));
			if (m_paragraphStyleState == beginNumbering || m_paragraphStyleState == beginDisplayReferencing)
			{ 
				m_paragraphStyleState = styleBody;         
				X_CheckWordPerfectError(_appendCurrentListProperties());
				m_numDeferredParagraphBreaks = 0; // just to make sure that the no. of deferred paragraph breaks doesn't continually go down - we only need to compensate once
			}
		break;*/
		case WP6_CHARACTER_GROUP_TABLE_DEFINITION_ON:
			WPD_DEBUG_MSG(("WordPerfect: TABLE Definition ON\n"));
			/*UT_uint8 numPfxID;
			
			// FIXME: The following prefix IDs only exist if the PFX bit is set
			// in the flags. For now, we just assume it _is_ set.
			X_CheckFileReadElementError(fread(&numPfxID, sizeof(numPfxID), 1, m_importFile));
			UT_DEBUGMSG(("WordPerfect: Number of Table PFX IDs: %d\n", numPfxID));
			for (int i=0; i<numPfxID; i++)
			{
			UT_uint16 pfxID;
			X_CheckFileReadElementError(fread(&pfxID, sizeof(pfxID), 1, m_importFile));
			// FIXME: handle the differt prefixes
			// For now, we just skip over them
			}
	
			UT_uint16 sizeOfNonDelData;
			X_CheckFileReadElementError(fread(&sizeOfNonDelData, sizeof(sizeOfNonDelData), 1, m_importFile));
			// FIXME: handle all the non-deletable (style) data.
			// For now, we just skip over it.
			
			m_iCurrentTableRow = -1;
			m_iCurrentTableColumn = -1;
			X_CheckDocumentError(getDoc()->appendStrux(PTX_SectionTable, NULL));*/
			
			llListener->startTable();
			break;
		case WP6_CHARACTER_GROUP_TABLE_DEFINITION_OFF:
			WPD_DEBUG_MSG(("WordPerfect: TABLE Definition OFF\n"));
			break;
		case WP6_CHARACTER_GROUP_TABLE_COLUMN:
			WPD_DEBUG_MSG(("WordPerfect: Table Column\n"));
			// FIXME: handle all the table column data.
			// For now, we just skip over it.
			break;
		default: // something else we don't support yet
			break;
	}
}
