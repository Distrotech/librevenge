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

#include "WP6ParagraphGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6ParagraphGroup::WP6ParagraphGroup(GsfInput *input) :
	WP6VariableLengthGroup(),
	m_subGroupData(NULL),
	m_justification(0)
{
	_read(input);
}

WP6ParagraphGroup::~WP6ParagraphGroup()
{
	if (m_subGroupData)
		delete(m_subGroupData);
}

void WP6ParagraphGroup::_readContents(GsfInput *input)
{
	switch (getSubGroup())
	{
		case WP6_PARAGRAPH_GROUP_JUSTIFICATION:
		{   
			m_justification = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
			break;
		}
		case WP6_PARAGRAPH_GROUP_OUTLINE_DEFINE:
		{
			m_subGroupData = new WP6ParagraphGroup_OutlineDefineSubGroup(input);
			break;
		}
	}
}

void WP6ParagraphGroup::parse(WP6LLListener *llListener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an Paragraph group\n"));
	
	switch (getSubGroup())
	{
		case WP6_PARAGRAPH_GROUP_JUSTIFICATION:
		{   
			llListener->justificationChange(getJustification());
			break;
		}
		case WP6_PARAGRAPH_GROUP_OUTLINE_DEFINE:
		{
			m_subGroupData->parse(llListener, getNumPrefixIDs(), getPrefixIDs());
			break;
		}
	}
}

WP6ParagraphGroup_OutlineDefineSubGroup::WP6ParagraphGroup_OutlineDefineSubGroup(GsfInput *input)
{
	// NB: this is identical to WP6OutlineStylePacket::_readContents!!
	m_outlineHash = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
	for (unsigned int i=0; i<WP6_NUM_LIST_LEVELS; i++)  
		m_numberingMethods[i] = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
	m_tabBehaviourFlag = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
	
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (, outlineHash: %i, tab behaviour flag: %i)\n", (int) m_outlineHash, (int) m_tabBehaviourFlag));
// 	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_paragraphStylePIDs: %i %i %i %i %i %i %i %i)\n", 
// 		       m_paragraphStylePIDs[0], m_paragraphStylePIDs[1], m_paragraphStylePIDs[2], m_paragraphStylePIDs[3],
// 		       m_paragraphStylePIDs[4], m_paragraphStylePIDs[5], m_paragraphStylePIDs[6], m_paragraphStylePIDs[7]));
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_numberingMethods: %i %i %i %i %i %i %i %i)\n", 
		       m_numberingMethods[0], m_numberingMethods[1], m_numberingMethods[2], m_numberingMethods[3],
		       m_numberingMethods[4], m_numberingMethods[5], m_numberingMethods[6], m_numberingMethods[7]));
}

void WP6ParagraphGroup_OutlineDefineSubGroup::parse(WP6LLListener *llListener, const guint8 numPrefixIDs, guint16 const *prefixIDs) const
{
	llListener->updateOutlineDefinition(paragraphGroup, m_outlineHash, m_numberingMethods, m_tabBehaviourFlag);
}
