/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP5PrefixData.h"
#include "WP5GeneralPacketIndex.h"
#include "WP5SpecialHeaderIndex.h"
#include "libwpd_internal.h"

WP5PrefixData::WP5PrefixData(WPXInputStream *input)
{
	std::vector<WP5GeneralPacketIndex> prefixIndexVector;
	int id = 0;
	while (true)
	{
		WP5SpecialHeaderIndex shi = WP5SpecialHeaderIndex(input);
		for (int i=0; i<(shi.getNumOfIndexes()-1); i++)
		{
			WP5GeneralPacketIndex gpi = WP5GeneralPacketIndex(input, id);
			if ((gpi.getType() != 0) && (gpi.getType() != 0xffff))
			{
				WPD_DEBUG_MSG(("WordPerfect: constructing general packet index 0x%x\n", id));
				prefixIndexVector.push_back(gpi);
				id++;
			}
		}
		if (shi.getNextBlockOffset() == 0)
			return;
		else
			input->seek(shi.getNextBlockOffset(), WPX_SEEK_SET);
	}
	
	std::vector<WP5GeneralPacketIndex>::iterator gpiIter;
	for (gpiIter = prefixIndexVector.begin(); gpiIter != prefixIndexVector.end(); gpiIter++)
	{
		WPD_DEBUG_MSG(("WordPerfect: constructing general packet data 0x%x\n", (*gpiIter).getID()));
		WP5GeneralPacketData *generalPacketData = WP5GeneralPacketData::constructGeneralPacketData(input, &(*gpiIter));
		if (generalPacketData)
		{
			m_generalPacketDataMapByType[gpiIter->getType()] = generalPacketData;
			m_generalPacketDataMapByID[gpiIter->getID()] = generalPacketData;
		}	
	}

	
}

WP5PrefixData::~WP5PrefixData()
{
	std::map<int, WP5GeneralPacketData *>::const_iterator Iter;
	for (Iter = m_generalPacketDataMapByType.begin(); Iter != m_generalPacketDataMapByType.end(); Iter++)
		delete (Iter->second);
}

const WP5GeneralPacketData * WP5PrefixData::getGeneralPacketDataByID(const int packetID) const
{
	std::map<int, WP5GeneralPacketData *>::const_iterator Iter = m_generalPacketDataMapByID.find(packetID);
	if (Iter != m_generalPacketDataMapByID.end())
		return static_cast<const WP5GeneralPacketData *>(Iter->second);
	else
		return NULL;
}

const WP5GeneralPacketData * WP5PrefixData::getGeneralPacketDataByType(const int type) const
{
	std::map<int, WP5GeneralPacketData *>::const_iterator Iter = m_generalPacketDataMapByType.find(type);
	if (Iter != m_generalPacketDataMapByType.end())
		return static_cast<const WP5GeneralPacketData *>(Iter->second);
	else
		return NULL;
}
