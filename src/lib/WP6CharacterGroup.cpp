/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6CharacterGroup.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"
#include "WP6CommentAnnotationPacket.h"
#include "WP6FontDescriptorPacket.h"

/*************************************************************************
 * WP6CharacterGroup_SetAlignmentCharacterSubGroup
 *************************************************************************/

WP6CharacterGroup_SetAlignmentCharacterSubGroup::WP6CharacterGroup_SetAlignmentCharacterSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_character(0),
	m_characterSet(0)
{
	uint16_t charWord = readU16(input, encryption);
	m_characterSet = (uint8_t)((charWord >> 8) & 0x00FF);
	m_character = (uint8_t)(charWord & 0x00FF);
}

void WP6CharacterGroup_SetAlignmentCharacterSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
        uint16_t const * /* prefixIDs */) const
{
	const uint32_t *chars;
	extendedCharacterWP6ToUCS4(m_character, m_characterSet, &chars);
	WPD_DEBUG_MSG(("WordPerfect: Parsing Set Alignment Character (alignment character: 0x%.4x)\n", chars[0]));
	listener->setAlignmentCharacter(chars[0]);
}

/*************************************************************************
 * WP6CharacterGroup_ColorSubGroup
 *************************************************************************/

WP6CharacterGroup_ColorSubGroup::WP6CharacterGroup_ColorSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_red(0), m_green(0), m_blue(0)
{
	m_red = readU8(input, encryption);
	m_green = readU8(input, encryption);
	m_blue = readU8(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: Character Group Color subgroup info (red: %i, green: %i, blue: %i)\n", m_red, m_green, m_blue));
}

void WP6CharacterGroup_ColorSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */, uint16_t const * /* prefixIDs */) const
{
	listener->characterColorChange(m_red, m_green, m_blue);
}

/*************************************************************************
 * WP6CharacterGroup_CharacterShadingChangeSubGroup
 *************************************************************************/

WP6CharacterGroup_CharacterShadingChangeSubGroup::WP6CharacterGroup_CharacterShadingChangeSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_shading(0)
{
	m_shading = readU8(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: Character Group Character Shading Change subgroup info (shading: %i)\n", m_shading));
}

void WP6CharacterGroup_CharacterShadingChangeSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
        uint16_t const * /* prefixIDs */) const
{
	listener->characterShadingChange(m_shading);
}

/*************************************************************************
 * WP6CharacterGroup_FontFaceChangeSubGroups
 *************************************************************************/

WP6CharacterGroup_FontFaceChangeSubGroup::WP6CharacterGroup_FontFaceChangeSubGroup(WPXInputStream *input, WPXEncryption *encryption, uint16_t sizeDeletable) :
	m_oldMatchedPointSize(0), m_hash(0), m_matchedFontIndex(0), m_matchedFontPointSize(0), m_packet(0)
{
	m_oldMatchedPointSize = readU16(input, encryption);
	m_hash = readU16(input, encryption);
	m_matchedFontIndex = readU16(input, encryption);
	m_matchedFontPointSize = readU16(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: Character Group Font Face Change subgroup info (old matched point size: %i, hash: %i, matched font index: %i, matched font point size: %i)\n", m_oldMatchedPointSize, m_hash, m_matchedFontIndex, m_matchedFontPointSize));

	if (sizeDeletable > 24)
	{
		m_packet = new WP6FontDescriptorPacket(input, encryption, 0, (uint32_t)input->tell(), sizeDeletable);

		WPD_DEBUG_MSG(("WordPerfect: Character Group Font Face Change subgroup info (font name: %s)\n", m_packet->getFontName().cstr()));
	}
}

WP6CharacterGroup_FontFaceChangeSubGroup::~WP6CharacterGroup_FontFaceChangeSubGroup()
{
	if (m_packet)
		delete m_packet;
	m_packet = NULL;
}

void WP6CharacterGroup_FontFaceChangeSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */, uint16_t const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: FontFaceChangeSubGroup parsing\n"));
	if (!prefixIDs)
		return;
	listener->fontChange(m_matchedFontPointSize, prefixIDs[0], m_packet ? m_packet->getFontName() : WPXString());
}

/*************************************************************************
 * WP6CharacterGroup_FontSizeChangeSubGroups
 *************************************************************************/

WP6CharacterGroup_FontSizeChangeSubGroup::WP6CharacterGroup_FontSizeChangeSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_desiredFontPointSize(0)
{
	m_desiredFontPointSize = readU16(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: Character Group Font Size Change subgroup info (desired font point size: %i\n", m_desiredFontPointSize));
}

void WP6CharacterGroup_FontSizeChangeSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */, uint16_t const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: FontSizeChangeSubGroup parsing\n"));
	if (!prefixIDs)
		return;
	listener->fontChange(m_desiredFontPointSize, prefixIDs[0], WPXString());
}

/*************************************************************************
 * WP6CharacterGroup_SetDotLeaderCharactersSubGroup
 *************************************************************************/

WP6CharacterGroup_SetDotLeaderCharactersSubGroup::WP6CharacterGroup_SetDotLeaderCharactersSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_character(0),
	m_characterSet(0),
	m_numberOfSpaces(0)
{
	uint16_t charWord = readU16(input, encryption);
	m_characterSet = (uint8_t)((charWord >> 8) & 0x00FF);
	m_character = (uint8_t)(charWord & 0x00FF);
	m_numberOfSpaces = readU8(input, encryption);
}

void WP6CharacterGroup_SetDotLeaderCharactersSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
        uint16_t const * /* prefixIDs */) const
{
	const uint32_t *chars;
	extendedCharacterWP6ToUCS4(m_character, m_characterSet, &chars);
	WPD_DEBUG_MSG(("WordPerfect: Parsing Set Dot Leader Characters (leader character: 0x%.4x), (number of spaces: %i)\n",
	               chars[0], m_numberOfSpaces));
	listener->setLeaderCharacter(chars[0], m_numberOfSpaces);
}

/*************************************************************************
 * WP6CharacterGroup_ParagraphNumberOnSubGroup
 *************************************************************************/

WP6CharacterGroup_ParagraphNumberOnSubGroup::WP6CharacterGroup_ParagraphNumberOnSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_outlineHash(0), m_level(0), m_flag(0)
{
	m_outlineHash = readU16(input, encryption);
	m_level = readU8(input, encryption);
	m_flag = readU8(input, encryption);
}

void WP6CharacterGroup_ParagraphNumberOnSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
        uint16_t const * /* prefixIDs */) const
{
	listener->paragraphNumberOn(m_outlineHash, m_level, m_flag);
}

/*************************************************************************
 * WP6CharacterGroup_TableDefinitionOnSubGroup
 *************************************************************************/

WP6CharacterGroup_TableDefinitionOnSubGroup::WP6CharacterGroup_TableDefinitionOnSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_flags(0), m_position(0), m_leftOffset(0)
{
	m_flags = readU8(input, encryption);
	m_position = readU8(input, encryption);
	m_leftOffset = readU16(input, encryption);
	// TODO: add the remaining table properties here
}

void WP6CharacterGroup_TableDefinitionOnSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
        uint16_t const * /* prefixIDs */) const
{
	listener->defineTable(m_position, m_leftOffset);
}

/*************************************************************************
 * WP6CharacterGroup_TableDefinitionOffSubGroup
 *************************************************************************/

WP6CharacterGroup_TableDefinitionOffSubGroup::WP6CharacterGroup_TableDefinitionOffSubGroup(WPXInputStream * /* input */, WPXEncryption * /* encryption */)
{
}

void WP6CharacterGroup_TableDefinitionOffSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
        uint16_t const * /* prefixIDs */) const
{
	// the table is degined now; start the table
	listener->startTable();
}

/*************************************************************************
 * WP6CharacterGroup_TableColumnSubGroup
 *************************************************************************/

WP6CharacterGroup_TableColumnSubGroup::WP6CharacterGroup_TableColumnSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_flags(0), m_width(0), m_leftGutter(0), m_rightGutter(0),
	m_attributes(0), m_alignment(0), m_absPosFromRight(0), m_numberType(0),
	m_currencyIndex(0)
{
	m_flags = readU8(input, encryption);
	m_width = readU16(input, encryption);

	m_leftGutter = readU16(input, encryption);
	m_rightGutter = readU16(input, encryption);
	m_attributes = (readU32(input, encryption) & 0x0003FFFF);
	m_alignment = (readU8(input, encryption) & 0x07);
	m_absPosFromRight = readU16(input, encryption);
	m_numberType = readU16(input, encryption);
	m_currencyIndex = readU8(input, encryption);
}

void WP6CharacterGroup_TableColumnSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */, uint16_t const * /* prefixIDs */) const
{
	listener->addTableColumnDefinition(m_width, m_leftGutter, m_rightGutter, m_attributes, m_alignment);
}

/*************************************************************************
 * WP6CharacterGroup_CommentSubGroup
 *************************************************************************/

WP6CharacterGroup_CommentSubGroup::WP6CharacterGroup_CommentSubGroup(WPXInputStream * /* input */, WPXEncryption * /* encryption */)
{
}

void WP6CharacterGroup_CommentSubGroup::parse(WP6Listener *listener, const uint8_t numPrefixIDs,
        uint16_t const *prefixIDs) const
{
	uint16_t textPID = 0;
	for (uint8_t i=0; i<numPrefixIDs; i++)
	{
		if (const WP6CommentAnnotationPacket *caPacket = dynamic_cast<const WP6CommentAnnotationPacket *>(listener->getPrefixDataPacket(prefixIDs[i])))
		{
			textPID = caPacket->getTextPID();
			break;
		}
	}
	if (textPID)
		listener->commentAnnotation(textPID);
}

/*************************************************************************
 * WP6CharacterGroup
 *************************************************************************/

WP6CharacterGroup::WP6CharacterGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_subGroupData(0)
{
	_read(input, encryption);
}

WP6CharacterGroup::~WP6CharacterGroup()
{
	delete m_subGroupData;
}

void WP6CharacterGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP6_CHARACTER_GROUP_FONT_FACE_CHANGE:
		m_subGroupData = new WP6CharacterGroup_FontFaceChangeSubGroup(input, encryption, getSizeDeletable());
		break;
	case WP6_CHARACTER_GROUP_FONT_SIZE_CHANGE:
		m_subGroupData = new WP6CharacterGroup_FontSizeChangeSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_SET_ALIGNMENT_CHARACTER:
		m_subGroupData = new WP6CharacterGroup_SetAlignmentCharacterSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_COLOR:
		m_subGroupData = new WP6CharacterGroup_ColorSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_CHARACTER_SHADING_CHANGE:
		m_subGroupData = new WP6CharacterGroup_CharacterShadingChangeSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_SET_DOT_LEADER_CHARACTERS:
		m_subGroupData = new WP6CharacterGroup_SetDotLeaderCharactersSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_ON:
		m_subGroupData = new WP6CharacterGroup_ParagraphNumberOnSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_ON:
		m_subGroupData = new WP6CharacterGroup_TableDefinitionOnSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_OFF:
		m_subGroupData = new WP6CharacterGroup_TableDefinitionOffSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_TABLE_COLUMN:
		m_subGroupData = new WP6CharacterGroup_TableColumnSubGroup(input, encryption);
		break;
	case WP6_CHARACTER_GROUP_COMMENT:
		m_subGroupData = new WP6CharacterGroup_CommentSubGroup(input, encryption);
		break;
	default:
		break;
	}
}

void WP6CharacterGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Character group\n"));

	switch (getSubGroup())
	{
	case WP6_CHARACTER_GROUP_FONT_FACE_CHANGE:
	case WP6_CHARACTER_GROUP_FONT_SIZE_CHANGE:
	case WP6_CHARACTER_GROUP_SET_ALIGNMENT_CHARACTER:
	case WP6_CHARACTER_GROUP_COLOR:
	case WP6_CHARACTER_GROUP_CHARACTER_SHADING_CHANGE:
	case WP6_CHARACTER_GROUP_SET_DOT_LEADER_CHARACTERS:
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_ON:
	case WP6_CHARACTER_GROUP_COMMENT:
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
		break;
	case WP6_CHARACTER_GROUP_PARAGRAPH_NUMBER_OFF:
		listener->paragraphNumberOff();
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_ON:
		WPD_DEBUG_MSG(("WordPerfect: TABLE Definition ON\n"));
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
		break;
	case WP6_CHARACTER_GROUP_TABLE_DEFINITION_OFF:
		WPD_DEBUG_MSG(("WordPerfect: TABLE Definition OFF\n"));
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
		break;
	case WP6_CHARACTER_GROUP_TABLE_COLUMN:
		WPD_DEBUG_MSG(("WordPerfect: Table Column\n"));
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
		break;
	default: // something else we don't support yet
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
