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

/*************************************************************************
 * WP6CharacterGroup_FontFaceChangeSubGroups
 *************************************************************************/

WP6CharacterGroup_FontFaceChangeSubGroup::WP6CharacterGroup_FontFaceChangeSubGroup(GsfInput *input)
{
	m_oldMatchedPointSize = gsf_le_read_guint16(input);
	m_hash = gsf_le_read_guint16(input);
	m_matchedFontIndex = gsf_le_read_guint16(input);
	m_matchedFontPointSize = gsf_le_read_guint16(input);
	WPD_DEBUG_MSG(("WordPerfect: Character Group Font Face Change subgroup info (old matched point size: %i, hash: %i, matched font index: %i, matched font point size: %i\n", m_oldMatchedPointSize, m_hash, m_matchedFontIndex, m_matchedFontPointSize));
}

ParseResult WP6CharacterGroup_FontFaceChangeSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: FontFaceChangeSubGroup parsing\n"));
	// TODO: check that we have 1 prefix id
	llListener->fontChange(m_matchedFontPointSize, prefixIDs[0]);
	// emit an exception otherwise

	return PARSE_OK;
}

/*************************************************************************
 * WP6CharacterGroup_ParagraphNumberOnSubGroup
 *************************************************************************/

WP6CharacterGroup_ParagraphNumberOnSubGroup::WP6CharacterGroup_ParagraphNumberOnSubGroup(GsfInput *input)
{
	m_outlineHash = gsf_le_read_guint16(input);
	m_level = gsf_le_read_guint8(input);
	m_flag = gsf_le_read_guint8(input);
}

ParseResult WP6CharacterGroup_ParagraphNumberOnSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	llListener->paragraphNumberOn(m_outlineHash, m_level, m_flag);
	return PARSE_OK;
}

/*************************************************************************
 * WP6CharacterGroup_TableDefinitionOnSubGroup
 *************************************************************************/

WP6CharacterGroup_TableDefinitionOnSubGroup::WP6CharacterGroup_TableDefinitionOnSubGroup(GsfInput *input)
{
	m_flags = gsf_le_read_guint8(input);
	m_position = gsf_le_read_guint8(input);
	m_leftOffset = gsf_le_read_guint16(input);
	// TODO: add the remaining table properties here
}

ParseResult WP6CharacterGroup_TableDefinitionOnSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	llListener->defineTable(m_position, m_leftOffset);
	return PARSE_OK;
}

/*************************************************************************
 * WP6CharacterGroup_TableDefinitionOffSubGroup
 *************************************************************************/

WP6CharacterGroup_TableDefinitionOffSubGroup::WP6CharacterGroup_TableDefinitionOffSubGroup(GsfInput *input)
{
	
}

ParseResult WP6CharacterGroup_TableDefinitionOffSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	// the table is degined now; start the table
	llListener->startTable();
}

/*************************************************************************
 * WP6CharacterGroup_TableColumnSubGroup
 *************************************************************************/

WP6CharacterGroup_TableColumnSubGroup::WP6CharacterGroup_TableColumnSubGroup(GsfInput *input)
{
	m_flags = gsf_le_read_guint8(input);
	m_width = gsf_le_read_guint16(input);
	
	m_leftGutter = gsf_le_read_guint16(input);
	m_rigthGutter = gsf_le_read_guint16(input);
	m_attribWord1 = gsf_le_read_guint16(input);
	m_attribWord2 = gsf_le_read_guint16(input);
	m_alignment = gsf_le_read_guint8(input);
	m_absPosFromRight = gsf_le_read_guint16(input);
	m_numberType = gsf_le_read_guint16(input);
	m_currencyIndex = gsf_le_read_guint8(input);
}

ParseResult WP6CharacterGroup_TableColumnSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	llListener->addTableColumnDefinition(m_width, m_leftGutter, m_rigthGutter);
	return PARSE_OK;
}

/*************************************************************************
 * WP6CharacterGroup
 *************************************************************************/

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
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_ON:
		m_subGroupData = new WP6CharacterGroup_TableDefinitionOnSubGroup(input);
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_OFF:
		m_subGroupData = new WP6CharacterGroup_TableDefinitionOffSubGroup(input);
		break;	
	case WP6_CHARACTER_GROUP_TABLE_COLUMN:
		m_subGroupData = new WP6CharacterGroup_TableColumnSubGroup(input);
		break;
	default:
		break;
	}
}

ParseResult WP6CharacterGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Character group\n"));
	
	switch (getSubGroup())
	{
	case WP6_CHARACTER_GROUP_FONT_FACE_CHANGE:
	case WP6_CHARACTER_GROUP_FONT_SIZE_CHANGE:
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_ON:
		return m_subGroupData->parse(llListener, getNumPrefixIDs(), getPrefixIDs());
		break;
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_OFF:
		llListener->paragraphNumberOff();
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_ON:
		WPD_DEBUG_MSG(("WordPerfect: TABLE Definition ON\n"));
		return m_subGroupData->parse(llListener, getNumPrefixIDs(), getPrefixIDs());
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_OFF:
		WPD_DEBUG_MSG(("WordPerfect: TABLE Definition OFF\n"));
		return m_subGroupData->parse(llListener, getNumPrefixIDs(), getPrefixIDs());
	case WP6_CHARACTER_GROUP_TABLE_COLUMN:
		WPD_DEBUG_MSG(("WordPerfect: Table Column\n"));
		return m_subGroupData->parse(llListener, getNumPrefixIDs(), getPrefixIDs());
	default: // something else we don't support yet
		break;
	}

	return PARSE_OK;
}
