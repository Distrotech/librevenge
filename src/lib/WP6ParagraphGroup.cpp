/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6ParagraphGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"
#include "WPXFileStructure.h"

WP6ParagraphGroup::WP6ParagraphGroup(WPXInputStream *input) :
	WP6VariableLengthGroup(),
	m_subGroupData(NULL)
{
	_read(input);
}

WP6ParagraphGroup::~WP6ParagraphGroup()
{
	if (m_subGroupData)
		delete(m_subGroupData);
}

void WP6ParagraphGroup::_readContents(WPXInputStream *input)
{
	switch (getSubGroup())
	{
	case WP6_PARAGRAPH_GROUP_LINE_SPACING:
		m_subGroupData = new WP6ParagraphGroup_LineSpacingSubGroup(input);
		break;
	case WP6_PARAGRAPH_GROUP_TAB_SET:
		m_subGroupData = new WP6ParagraphGroup_TabSetSubGroup(input);
		break;
	case WP6_PARAGRAPH_GROUP_JUSTIFICATION:
		m_subGroupData = new WP6ParagraphGroup_JustificationModeSubGroup(input);
		break;
	case WP6_PARAGRAPH_GROUP_SPACING_AFTER_PARAGRAPH:
		m_subGroupData = new WP6ParagraphGroup_SpacingAfterParagraphSubGroup(input, getSizeNonDeletable());
		break;
	case WP6_PARAGRAPH_GROUP_INDENT_FIRST_LINE_OF_PARAGRAPH:
		m_subGroupData = new WP6ParagraphGroup_IndentFirstLineSubGroup(input);
		break;
	case WP6_PARAGRAPH_GROUP_LEFT_MARGIN_ADJUSTMENT:
		m_subGroupData = new WP6ParagraphGroup_LeftMarginAdjustmentSubGroup(input);
		break;
	case WP6_PARAGRAPH_GROUP_RIGHT_MARGIN_ADJUSTMENT:
		m_subGroupData = new WP6ParagraphGroup_RightMarginAdjustmentSubGroup(input);
		break;
	case WP6_PARAGRAPH_GROUP_OUTLINE_DEFINE:
		m_subGroupData = new WP6ParagraphGroup_OutlineDefineSubGroup(input);
		break;
	}
}

void WP6ParagraphGroup::parse(WP6HLListener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an Paragraph group\n"));

	if (m_subGroupData)
		m_subGroupData->parse(listener, getNumPrefixIDs(), getPrefixIDs());
}

WP6ParagraphGroup_LineSpacingSubGroup::WP6ParagraphGroup_LineSpacingSubGroup(WPXInputStream *input)
{
	guint32 lineSpacing = readU32(input);
	gint16 lineSpacingIntegerPart = (gint16)((lineSpacing & 0xFFFF0000) >> 16);
	float lineSpacingFractionalPart = (float)(lineSpacing & 0xFFFF)/(float)0xFFFF;
	WPD_DEBUG_MSG(("WordPerfect: line spacing integer part: %i fractional part: %f (original value: %i)\n",
		       lineSpacingIntegerPart, lineSpacingFractionalPart, lineSpacing));
	m_lineSpacing = lineSpacingIntegerPart + lineSpacingFractionalPart;
}

void WP6ParagraphGroup_LineSpacingSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing a line spacing change of: %f\n", m_lineSpacing));
	listener->lineSpacingChange(m_lineSpacing);
}

WP6ParagraphGroup_TabSetSubGroup::WP6ParagraphGroup_TabSetSubGroup(WPXInputStream *input) :
	m_isRelative(false),
	m_tabAdjustValue(0.0f),
	m_numberOfTabStops(0),
	m_usePreWP9LeaderMethod(NULL),
	m_tabStops(NULL)
{
	guint8 tmp_definition = readU8(input);
	guint16 tmp_tabAdjustValue = readU16(input);
	if (tmp_definition == 0)
	{
		m_isRelative = false;
		m_tabAdjustValue = 0.0f;
	}
	else
	{
		m_isRelative = true;
		m_tabAdjustValue = (float)((double)tmp_tabAdjustValue/(double)WPX_NUM_WPUS_PER_INCH);
	}
	guint8 tmp_repetitionCount = 0;
	WPXTabStop tmp_tabStop;
	guint8 tmp_numberOfTabStops = readU8(input);
	bool tmp_usePreWP9LeaderMethod;
	guint8 tmp_tabType;
	m_numberOfTabStops = 0;
	int i;
	for (i = 0; i < tmp_numberOfTabStops; i++)
	{
		tmp_tabType = readU8(input);
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
			tmp_tabStop.m_leaderNumberOfSpaces = 0;
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
					tmp_tabStop.m_leaderNumberOfSpaces = 0;
					tmp_usePreWP9LeaderMethod = true;
					break;
				case 1: // dot leader
					tmp_tabStop.m_leaderCharacter = '.';
					tmp_tabStop.m_leaderNumberOfSpaces = 0;
					tmp_usePreWP9LeaderMethod = false;
					break;
				case 2: // hyphen leader
					tmp_tabStop.m_leaderCharacter = '-';
					tmp_tabStop.m_leaderNumberOfSpaces = 0;
					tmp_usePreWP9LeaderMethod = false;
					break;
				case 3: // underscore leader
					tmp_tabStop.m_leaderCharacter = '_';
					tmp_tabStop.m_leaderNumberOfSpaces = 0;
					tmp_usePreWP9LeaderMethod = false;
					break;
				}
			}
		}
		guint16 tmp_tabPosition = readU16(input);
		if (tmp_repetitionCount == 0)
		{
			tmp_tabStop.m_position = (float)((double)tmp_tabPosition/(double)WPX_NUM_WPUS_PER_INCH) -
							m_tabAdjustValue;
			_addTabStop(tmp_tabStop);
			_addLeaderMethod(tmp_usePreWP9LeaderMethod);
			m_numberOfTabStops++;
		}
		else
		{
			int k;
			for (k=0; k<tmp_repetitionCount; k++)
			{
				tmp_tabStop.m_position += (float)((double)tmp_tabPosition/(double)WPX_NUM_WPUS_PER_INCH);
				_addTabStop(tmp_tabStop);
				_addLeaderMethod(tmp_usePreWP9LeaderMethod);
				m_numberOfTabStops++;
			}
			tmp_repetitionCount = 0;
		}
		WPD_DEBUG_MSG(("\n"));
	}
}

WP6ParagraphGroup_TabSetSubGroup::~WP6ParagraphGroup_TabSetSubGroup()
{
	free(m_tabStops);
	free(m_usePreWP9LeaderMethod);
}

void WP6ParagraphGroup_TabSetSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("Parsing Tab Set (isRelative: %s, numberOfTabStops: %i, positions: ", (m_isRelative?"true":"false"), m_numberOfTabStops));
	int i;
	for(i=0; i<m_numberOfTabStops; i++)
	{
		WPD_DEBUG_MSG((" %.4f", m_tabStops[i].m_position));
	}
	WPD_DEBUG_MSG((")\n"));
	listener->defineTabStops(m_isRelative, m_numberOfTabStops, m_tabStops, m_usePreWP9LeaderMethod);
}

void WP6ParagraphGroup_TabSetSubGroup::_addTabStop(WPXTabStop tabStop)
{
	if (m_numberOfTabStops == 0)
		m_tabStops = (WPXTabStop*)malloc(sizeof(WPXTabStop));
	else
		m_tabStops = (WPXTabStop*)realloc(m_tabStops, sizeof(WPXTabStop)*(m_numberOfTabStops + 1));
	m_tabStops[m_numberOfTabStops] = tabStop;	
}

void WP6ParagraphGroup_TabSetSubGroup::_addLeaderMethod(bool usePreWP9LeaderMethod)
{
	if (m_numberOfTabStops == 0)
		m_usePreWP9LeaderMethod = (bool*)malloc(sizeof(bool));
	else
		m_usePreWP9LeaderMethod = (bool*)realloc(m_usePreWP9LeaderMethod, sizeof(bool)*(m_numberOfTabStops + 1));
	m_usePreWP9LeaderMethod[m_numberOfTabStops] = usePreWP9LeaderMethod;	
}

WP6ParagraphGroup_IndentFirstLineSubGroup::WP6ParagraphGroup_IndentFirstLineSubGroup(WPXInputStream *input)
{
	m_firstLineOffset = (gint16)readU16(input);
	WPD_DEBUG_MSG(("WordPerfect: indent first line: %i\n", m_firstLineOffset));
}

void WP6ParagraphGroup_IndentFirstLineSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing first line indent change of: %i\n", m_firstLineOffset));
	listener->indentFirstLineChange(m_firstLineOffset);
}

WP6ParagraphGroup_LeftMarginAdjustmentSubGroup::WP6ParagraphGroup_LeftMarginAdjustmentSubGroup(WPXInputStream *input)
{
	m_leftMargin = (gint16)readU16(input);
	WPD_DEBUG_MSG(("WordPerfect: left margin adjustment: %i\n", m_leftMargin));
}

void WP6ParagraphGroup_LeftMarginAdjustmentSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing left margin adjustment change of: %i\n", m_leftMargin));
	listener->paragraphMarginChange(WPX_LEFT, m_leftMargin);
}

WP6ParagraphGroup_RightMarginAdjustmentSubGroup::WP6ParagraphGroup_RightMarginAdjustmentSubGroup(WPXInputStream *input)
{
	m_rightMargin = (gint16)readU16(input);
	WPD_DEBUG_MSG(("WordPerfect: right margin adjustment: %i\n", m_rightMargin));
}

void WP6ParagraphGroup_RightMarginAdjustmentSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing right margin adjustment change of: %i\n", m_rightMargin));
	listener->paragraphMarginChange(WPX_RIGHT, m_rightMargin);
}

WP6ParagraphGroup_JustificationModeSubGroup::WP6ParagraphGroup_JustificationModeSubGroup(WPXInputStream *input)
{
	m_justification = readU8(input);
}

void WP6ParagraphGroup_JustificationModeSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	listener->justificationChange(m_justification);
}

WP6ParagraphGroup_SpacingAfterParagraphSubGroup::WP6ParagraphGroup_SpacingAfterParagraphSubGroup(WPXInputStream *input, const guint16 sizeNonDeletable)
{
	m_sizeNonDeletable = sizeNonDeletable;
	m_spacingAfterParagraphAbsolute = 0.0f;
	m_spacingAfterParagraphRelative = 1.0f;
	guint32 spacingAfterRelative = readU32(input);
	gint16 spacingAfterIntegerPart = (gint16)((spacingAfterRelative & 0xFFFF0000) >> 16);
	float spacingAfterFractionalPart = (float)(spacingAfterRelative & 0xFFFF)/(float)0xFFFF;
	WPD_DEBUG_MSG(("WordPerfect: spacing after paragraph relative integer part: %i fractional part: %f (original value: %i)\n",
				spacingAfterIntegerPart, spacingAfterFractionalPart, spacingAfterRelative));
	m_spacingAfterParagraphRelative = spacingAfterIntegerPart + spacingAfterFractionalPart;
	if (m_sizeNonDeletable == (guint16)0x06) // Let us use the optional information that is in WPUs
	{
		guint16 spacingAfterAbsolute = readU16(input);
		m_spacingAfterParagraphAbsolute = (float)((double)spacingAfterAbsolute / (double)WPX_NUM_WPUS_PER_INCH);
		WPD_DEBUG_MSG(("WordPerfect: spacing after paragraph absolute: %i\n", spacingAfterAbsolute));
	}
}

void WP6ParagraphGroup_SpacingAfterParagraphSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	WPD_DEBUG_MSG(("WordPerfect: parsing a change of spacing after paragraph: relative %f, absolute %f\n",
			m_spacingAfterParagraphRelative, m_spacingAfterParagraphAbsolute));
	listener->spacingAfterParagraphChange(m_spacingAfterParagraphRelative, m_spacingAfterParagraphAbsolute);
}

WP6ParagraphGroup_OutlineDefineSubGroup::WP6ParagraphGroup_OutlineDefineSubGroup(WPXInputStream *input)
{
	// NB: this is identical to WP6OutlineStylePacket::_readContents!!
	m_outlineHash = readU16(input);
	for (unsigned int i=0; i<WP6_NUM_LIST_LEVELS; i++)
		m_numberingMethods[i] = readU8(input);
	m_tabBehaviourFlag = readU8(input);

	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (, outlineHash: %i, tab behaviour flag: %i)\n", (int) m_outlineHash, (int) m_tabBehaviourFlag));
// 	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_paragraphStylePIDs: %i %i %i %i %i %i %i %i)\n",
// 		       m_paragraphStylePIDs[0], m_paragraphStylePIDs[1], m_paragraphStylePIDs[2], m_paragraphStylePIDs[3],
// 		       m_paragraphStylePIDs[4], m_paragraphStylePIDs[5], m_paragraphStylePIDs[6], m_paragraphStylePIDs[7]));
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_numberingMethods: %i %i %i %i %i %i %i %i)\n",
		       m_numberingMethods[0], m_numberingMethods[1], m_numberingMethods[2], m_numberingMethods[3],
		       m_numberingMethods[4], m_numberingMethods[5], m_numberingMethods[6], m_numberingMethods[7]));
}

void WP6ParagraphGroup_OutlineDefineSubGroup::parse(WP6HLListener *listener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	listener->updateOutlineDefinition(paragraphGroup, m_outlineHash, m_numberingMethods, m_tabBehaviourFlag);
}
