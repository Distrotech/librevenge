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

#include "WP6CharacterGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6CharacterGroup_FontFaceChangeSubGroup::WP6CharacterGroup_FontFaceChangeSubGroup(GsfInput *input)
{
	m_oldMatchedPointSize = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	m_hash = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	m_matchedFontIndex = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	m_matchedFontPointSize = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	WPD_DEBUG_MSG(("WordPerfect: Character Group Font Face Change subgroup info (old matched point size: %i, hash: %i, matched font index: %i, matched font point size: %i\n", m_oldMatchedPointSize, m_hash, m_matchedFontIndex, m_matchedFontPointSize));
}

void WP6CharacterGroup_FontFaceChangeSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: FontFaceChangeSubGroup parsing\n"));
	// TODO: check that we have 1 prefix id
	llListener->fontChange(m_matchedFontPointSize, prefixIDs[0]);

	// emit an exception otherwise
}

WP6CharacterGroup_ParagraphNumberOnSubGroup::WP6CharacterGroup_ParagraphNumberOnSubGroup(GsfInput *input)
{
	m_outlineHash = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	m_level = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
	m_flag = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
}

void WP6CharacterGroup_ParagraphNumberOnSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	llListener->paragraphNumberOn(m_outlineHash, m_level, m_flag);
}


WP6CharacterGroup::WP6CharacterGroup(GsfInput *input) :
	WP6VariableLengthGroup(),
	m_subGroupData(NULL)
{
	_read(input);
}

WP6CharacterGroup::~WP6CharacterGroup()
{
	if (m_subGroupData)
		delete(m_subGroupData);
}

void WP6CharacterGroup::_readContents(GsfInput *input)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())	
	{
	case WP6_CHARACTER_GROUP_FONT_FACE_CHANGE:
	case WP6_CHARACTER_GROUP_FONT_SIZE_CHANGE:
		m_subGroupData = new WP6CharacterGroup_FontFaceChangeSubGroup(input);
		break;
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_ON:
		m_subGroupData = new WP6CharacterGroup_ParagraphNumberOnSubGroup(input);
	default:
		break;
	}
}

void WP6CharacterGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Character group\n"));
	
	switch (getSubGroup())
	{
	case WP6_CHARACTER_GROUP_FONT_FACE_CHANGE:
	case WP6_CHARACTER_GROUP_FONT_SIZE_CHANGE:
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_ON:
		m_subGroupData->parse(llListener, getNumPrefixIDs(), getPrefixIDs());
		break;
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_OFF:
		llListener->paragraphNumberOff();
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_ON:
		WPD_DEBUG_MSG(("WordPerfect: TABLE Definition ON\n"));
		/*guint8 numPfxID;
			
		// FIXME: The following prefix IDs only exist if the PFX bit is set
		// in the flags. For now, we just assume it _is_ set.
		(fread(&numPfxID, sizeof(numPfxID);
		gDEBUGMSG(("WordPerfect: Number of Table PFX IDs: %d\n", numPfxID));
		for (int i=0; i<numPfxID; i++)
		{
		guint16 pfxID;
		(fread(&pfxID, sizeof(pfxID);
		// FIXME: handle the differt prefixes
		// For now, we just skip over them
		}
	
		guint16 sizeOfNonDelData;
		(fread(&sizeOfNonDelData, sizeof(sizeOfNonDelData);
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

// WP6CharacterGroup * WP6CharacterGroup::constructWP6CharacterGroup
