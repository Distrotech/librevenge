/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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
#include <string.h>

#include "WP6OutlineStylePacket.h"
#include "libwpd_internal.h"

WP6OutlineStylePacket::WP6OutlineStylePacket(GsfInput *input, int id, guint32 dataOffset, guint32 dataSize) 
	: WP6PrefixDataPacket(input)
{
	_read(input, dataOffset, dataSize);
}

WP6OutlineStylePacket::~WP6OutlineStylePacket()
{
}

void WP6OutlineStylePacket::_readContents(GsfInput *input)
{
	m_numPIDs = gsf_le_read_guint16(input);
	for (unsigned int i=0; i<WP6_NUM_LIST_LEVELS; i++) 
		m_paragraphStylePIDs[i] = gsf_le_read_guint16(input); // seemingly useless
	m_outlineFlags = gsf_le_read_guint8(input);
	m_outlineHash = gsf_le_read_guint16(input);
	for (unsigned int i=0; i<WP6_NUM_LIST_LEVELS; i++)  
		m_numberingMethods[i] = gsf_le_read_guint8(input);
	m_tabBehaviourFlag = gsf_le_read_guint8(input);
	
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (numPrefixIDs: %i, outlineStylePID: %i, outlineHash: %i, outlineFlags: %i, tab behaviour flag: %i)\n", (int) m_numPIDs, (int) m_outlineHash, (int) m_outlineFlags, (int) m_tabBehaviourFlag));
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_paragraphStylePIDs: %i %i %i %i %i %i %i %i)\n", 
		       m_paragraphStylePIDs[0], m_paragraphStylePIDs[1], m_paragraphStylePIDs[2], m_paragraphStylePIDs[3],
		       m_paragraphStylePIDs[4], m_paragraphStylePIDs[5], m_paragraphStylePIDs[6], m_paragraphStylePIDs[7]));
	WPD_DEBUG_MSG(("WordPerfect: Read Outline Style Packet (m_numberingMethods: %i %i %i %i %i %i %i %i)\n", 
		       m_numberingMethods[0], m_numberingMethods[1], m_numberingMethods[2], m_numberingMethods[3],
		       m_numberingMethods[4], m_numberingMethods[5], m_numberingMethods[6], m_numberingMethods[7]));
                     
}

void WP6OutlineStylePacket::parse(WP6LLListener *llListener) const
{
	llListener->updateOutlineDefinition(indexHeader, m_outlineHash, m_numberingMethods, m_tabBehaviourFlag);
}
