/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP6ParagraphGroup.h"
#include "libwpd_internal.h"
#include "WPXFileStructure.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"

WP6ParagraphGroup::WP6ParagraphGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_subGroupData(0)
{
	_read(input, encryption);
}

WP6ParagraphGroup::~WP6ParagraphGroup()
{
	if (m_subGroupData)
		delete(m_subGroupData);
}

void WP6ParagraphGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch (getSubGroup())
	{
	case WP6_PARAGRAPH_GROUP_LINE_SPACING:
		m_subGroupData = new WP6ParagraphGroup_LineSpacingSubGroup(input, encryption);
		break;
	case WP6_PARAGRAPH_GROUP_TAB_SET:
		m_subGroupData = new WP6ParagraphGroup_TabSetSubGroup(input, encryption);
		break;
	case WP6_PARAGRAPH_GROUP_JUSTIFICATION:
		m_subGroupData = new WP6ParagraphGroup_JustificationModeSubGroup(input, encryption);
		break;
	case WP6_PARAGRAPH_GROUP_SPACING_AFTER_PARAGRAPH:
		m_subGroupData = new WP6ParagraphGroup_SpacingAfterParagraphSubGroup(input, encryption, getSizeNonDeletable());
		break;
	case WP6_PARAGRAPH_GROUP_INDENT_FIRST_LINE_OF_PARAGRAPH:
		m_subGroupData = new WP6ParagraphGroup_IndentFirstLineSubGroup(input, encryption);
		break;
	case WP6_PARAGRAPH_GROUP_LEFT_MARGIN_ADJUSTMENT:
		m_subGroupData = new WP6ParagraphGroup_LeftMarginAdjustmentSubGroup(input, encryption);
		break;
	case WP6_PARAGRAPH_GROUP_RIGHT_MARGIN_ADJUSTMENT:
		m_subGroupData = new WP6ParagraphGroup_RightMarginAdjustmentSubGroup(input, encryption);
		break;
	case WP6_PARAGRAPH_GROUP_OUTLINE_DEFINE:
		m_subGroupData = new WP6ParagraphGroup_OutlineDefineSubGroup(input, encryption);
		break;
	}
}

void WP6ParagraphGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Paragraph group\n"));

	if (m_subGroupData)
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
}

WP6ParagraphGroup_LineSpacingSubGroup::WP6ParagraphGroup_LineSpacingSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_lineSpacing(0)
{
	uint32_t lineSpacing = readU32(input, encryption);
	int16_t lineSpacingIntegerPart = (int16_t)((lineSpacing & 0xFFFF0000) >> 16);
	double lineSpacingFractionalPart = (double)(lineSpacing & 0xFFFF)/(double)0xFFFF;
	WPD_DEBUG_MSG(("WordPerfect: line spacing integer part: %i fractional part: %f (original value: %i)\n",
		       lineSpacingIntegerPart, lineSpacingFractionalPart, lineSpacing));
	m_lineSpacing = lineSpacingIntegerPart + lineSpacingFractionalPart;
}

void WP6ParagraphGroup_LineSpacingSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
							const uint16_t * /* prefixIDs */) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing a line spacing change of: %f\n", m_lineSpacing));
	listener->lineSpacingChange(m_lineSpacing);
}

WP6ParagraphGroup_TabSetSubGroup::WP6ParagraphGroup_TabSetSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_isRelative(false),
	m_tabAdjustValue(0.0),
	m_usePreWP9LeaderMethods(),
	m_tabStops()
{
	uint8_t tmp_definition = readU8(input, encryption);
	uint16_t tmp_tabAdjustValue = readU16(input, encryption);
	if (tmp_definition == 0)
	{
		m_isRelative = false;
		m_tabAdjustValue = 0.0;
	}
	else
	{
		m_isRelative = true;
		m_tabAdjustValue = (double)((double)tmp_tabAdjustValue/(double)WPX_NUM_WPUS_PER_INCH);
	}
	uint8_t tmp_repetitionCount = 0;
	WPXTabStop tmp_tabStop;
	uint8_t tmp_numTabStops = readU8(input, encryption);
	bool tmp_usePreWP9LeaderMethod = false;
	uint8_t tmp_tabType = 0;
	for (int i = 0; i < tmp_numTabStops; i++)
	{
		tmp_tabType = readU8(input, encryption);
		if ((tmp_tabType & 0x80) != 0)
		{
			tmp_repetitionCount = (tmp_tabType & 0x7F);
		}
		else
		{
			switch (tmp_tabType & 0x0F) //alignment bits
			{
			case 0x00:
				tmp_tabStop.m_alignment = LEFT;
				break;
			case 0x01:
				tmp_tabStop.m_alignment = CENTER;
				break;
			case 0x02:
				tmp_tabStop.m_alignment = RIGHT;
				break;
			case 0x03:
				tmp_tabStop.m_alignment = DECIMAL;
				break;
			case 0x04:
				tmp_tabStop.m_alignment = BAR;
				break;
			default: // should not happen, maybe corruption
				tmp_tabStop.m_alignment = LEFT;
				break;
			}
			tmp_tabStop.m_leaderNumSpaces = 0;
			if ((tmp_tabType & 0x10) == 0) // no leader character
			{
				tmp_tabStop.m_leaderCharacter = '\0';
				tmp_usePreWP9LeaderMethod = false;
			}
			else
			{
				switch ((tmp_tabType & 0x60) >> 5) // leader character type
				{
				case 0: // pre-WP9 leader method
					tmp_tabStop.m_leaderCharacter = '.';
					tmp_tabStop.m_leaderNumSpaces = 0;
					tmp_usePreWP9LeaderMethod = true;
					break;
				case 1: // dot leader
					tmp_tabStop.m_leaderCharacter = '.';
					tmp_tabStop.m_leaderNumSpaces = 0;
					tmp_usePreWP9LeaderMethod = false;
					break;
				case 2: // hyphen leader
					tmp_tabStop.m_leaderCharacter = '-';
					tmp_tabStop.m_leaderNumSpaces = 0;
					tmp_usePreWP9LeaderMethod = false;
					break;
				case 3: // underscore leader
					tmp_tabStop.m_leaderCharacter = '_';
					tmp_tabStop.m_leaderNumSpaces = 0;
					tmp_usePreWP9LeaderMethod = false;
					break;
				}
			}
		}
		uint16_t tmp_tabPosition = readU16(input, encryption);
		if (tmp_repetitionCount == 0)
		{
			if (tmp_tabPosition != 0xFFFF)
			{
				tmp_tabStop.m_position = (double)((double)tmp_tabPosition/(double)WPX_NUM_WPUS_PER_INCH) -
					m_tabAdjustValue;
				m_tabStops.push_back(tmp_tabStop);
				m_usePreWP9LeaderMethods.push_back(tmp_usePreWP9LeaderMethod);
			}
		}
		else
		{
			for (int k=0; k<tmp_repetitionCount; k++)
			{
				tmp_tabStop.m_position += (double)((double)tmp_tabPosition/(double)WPX_NUM_WPUS_PER_INCH);
				m_tabStops.push_back(tmp_tabStop);
				m_usePreWP9LeaderMethods.push_back(tmp_usePreWP9LeaderMethod);
			}
			tmp_repetitionCount = 0;
		}
	}
}

WP6ParagraphGroup_TabSetSubGroup::~WP6ParagraphGroup_TabSetSubGroup()
{
}

void WP6ParagraphGroup_TabSetSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
						const uint16_t * /* prefixIDs */) const
{
#ifdef DEBUG
	WPD_DEBUG_MSG(("Parsing Tab Set (isRelative: %s, positions: ", (m_isRelative?"true":"false")));
	for(std::vector<WPXTabStop>::const_iterator i = m_tabStops.begin(); i != m_tabStops.end(); i++)
	{
		WPD_DEBUG_MSG((" %.4f", (*i).m_position));
	}
	WPD_DEBUG_MSG((")\n"));
#endif
	listener->defineTabStops(m_isRelative, m_tabStops, m_usePreWP9LeaderMethods);
}

WP6ParagraphGroup_IndentFirstLineSubGroup::WP6ParagraphGroup_IndentFirstLineSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_firstLineOffset(0)
{
	m_firstLineOffset = (int16_t)readU16(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: indent first line: %i\n", m_firstLineOffset));
}

void WP6ParagraphGroup_IndentFirstLineSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
							const uint16_t * /* prefixIDs */) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing first line indent change of: %i\n", m_firstLineOffset));
	listener->indentFirstLineChange(m_firstLineOffset);
}

WP6ParagraphGroup_LeftMarginAdjustmentSubGroup::WP6ParagraphGroup_LeftMarginAdjustmentSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_leftMargin(0)
{
	m_leftMargin = (int16_t)readU16(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: left margin adjustment: %i\n", m_leftMargin));
}

void WP6ParagraphGroup_LeftMarginAdjustmentSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
							const uint16_t * /* prefixIDs */) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing left margin adjustment change of: %i\n", m_leftMargin));
	listener->paragraphMarginChange(WPX_LEFT, m_leftMargin);
}

WP6ParagraphGroup_RightMarginAdjustmentSubGroup::WP6ParagraphGroup_RightMarginAdjustmentSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_rightMargin(0)
{
	m_rightMargin = (int16_t)readU16(input, encryption);
	WPD_DEBUG_MSG(("WordPerfect: right margin adjustment: %i\n", m_rightMargin));
}

void WP6ParagraphGroup_RightMarginAdjustmentSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
								const uint16_t * /* prefixIDs */) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing right margin adjustment change of: %i\n", m_rightMargin));
	listener->paragraphMarginChange(WPX_RIGHT, m_rightMargin);
}

WP6ParagraphGroup_JustificationModeSubGroup::WP6ParagraphGroup_JustificationModeSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_justification(0)
{
	m_justification = readU8(input, encryption);
}

void WP6ParagraphGroup_JustificationModeSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
								const uint16_t * /* prefixIDs */) const
{
	listener->justificationChange(m_justification);
}

WP6ParagraphGroup_SpacingAfterParagraphSubGroup::WP6ParagraphGroup_SpacingAfterParagraphSubGroup(WPXInputStream *input,
	WPXEncryption *encryption, const uint16_t sizeNonDeletable) :
	m_spacingAfterParagraphAbsolute(0.0),
	m_spacingAfterParagraphRelative(1.0),
	m_sizeNonDeletable(sizeNonDeletable)
{
	uint32_t spacingAfterRelative = readU32(input, encryption);
	int16_t spacingAfterIntegerPart = (int16_t)((spacingAfterRelative & 0xFFFF0000) >> 16);
	double spacingAfterFractionalPart = (double)(spacingAfterRelative & 0xFFFF)/(double)0xFFFF;
	WPD_DEBUG_MSG(("WordPerfect: spacing after paragraph relative integer part: %i fractional part: %f (original value: %i)\n",
				spacingAfterIntegerPart, spacingAfterFractionalPart, spacingAfterRelative));
	m_spacingAfterParagraphRelative = spacingAfterIntegerPart + spacingAfterFractionalPart;
	if (m_sizeNonDeletable == (uint16_t)0x06) // Let us use the optional information that is in WPUs
	{
		uint16_t spacingAfterAbsolute = readU16(input, encryption);
		m_spacingAfterParagraphAbsolute = (double)((double)spacingAfterAbsolute / (double)WPX_NUM_WPUS_PER_INCH);
		WPD_DEBUG_MSG(("WordPerfect: spacing after paragraph absolute: %i\n", spacingAfterAbsolute));
	}
}

void WP6ParagraphGroup_SpacingAfterParagraphSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
								const uint16_t * /* prefixIDs */) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing a change of spacing after paragraph: relative %f, absolute %f\n",
			m_spacingAfterParagraphRelative, m_spacingAfterParagraphAbsolute));
	listener->spacingAfterParagraphChange(m_spacingAfterParagraphRelative, m_spacingAfterParagraphAbsolute);
}

WP6ParagraphGroup_OutlineDefineSubGroup::WP6ParagraphGroup_OutlineDefineSubGroup(WPXInputStream *input, WPXEncryption *encryption) :
	m_outlineHash(0),
	m_numberingMethods(),
	m_tabBehaviourFlag(0)
{
	// NB: this is identical to WP6OutlineStylePacket::_readContents!!
	m_outlineHash = readU16(input, encryption);
	for (unsigned int i=0; i<WP6_NUM_LIST_LEVELS; i++)
		m_numberingMethods[i] = readU8(input, encryption);
	m_tabBehaviourFlag = readU8(input, encryption);

	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (, outlineHash: %i, tab behaviour flag: %i)\n", (int) m_outlineHash, (int) m_tabBehaviourFlag));
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_numberingMethods: %i %i %i %i %i %i %i %i)\n",
		       m_numberingMethods[0], m_numberingMethods[1], m_numberingMethods[2], m_numberingMethods[3],
		       m_numberingMethods[4], m_numberingMethods[5], m_numberingMethods[6], m_numberingMethods[7]));
}

void WP6ParagraphGroup_OutlineDefineSubGroup::parse(WP6Listener *listener, const uint8_t /* numPrefixIDs */,
						const uint16_t * /* prefixIDs */) const
{
	listener->updateOutlineDefinition(paragraphGroup, m_outlineHash, m_numberingMethods, m_tabBehaviourFlag);
}
