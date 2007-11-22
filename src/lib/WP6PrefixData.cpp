/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002-2003 Marc Maurer (uwog@uwog.net)
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

#include "WP6PrefixData.h"
#include "WP6PrefixIndice.h"
#include "WP6PrefixDataPacket.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"
#include "libwpd_internal.h"

WP6PrefixData::WP6PrefixData(WPXInputStream *input, WPXEncryption *encryption, const int numPrefixIndices) :
	m_prefixDataPacketHash(),
	m_prefixDataPacketTypeHash(),
	m_defaultInitialFontPID((-1))
{
	uint16_t i;

	WP6PrefixIndice ** prefixIndiceArray = new WP6PrefixIndice *[(numPrefixIndices-1)];
	for (i=1; i<numPrefixIndices; i++)
	{
		WPD_DEBUG_MSG(("WordPerfect: constructing prefix indice 0x%x\n", i));
		prefixIndiceArray[(i-1)] = new WP6PrefixIndice(input, encryption, i);
	}
	
	for (i=1; i<numPrefixIndices; i++) 
	{
		WPD_DEBUG_MSG(("WordPerfect: constructing prefix packet 0x%x\n", i));
		WP6PrefixDataPacket *prefixDataPacket = WP6PrefixDataPacket::constructPrefixDataPacket(input, encryption, prefixIndiceArray[(i-1)]);
		if (prefixDataPacket) {
			m_prefixDataPacketHash[i] = prefixDataPacket;
			m_prefixDataPacketTypeHash.insert(::std::map<int, WP6PrefixDataPacket *>::value_type(prefixIndiceArray[i-1]->getType(), prefixDataPacket));
			if (dynamic_cast<WP6DefaultInitialFontPacket *>(prefixDataPacket))
				m_defaultInitialFontPID = i;
		}
	}

	for (i=1; i<numPrefixIndices; i++)
		delete(prefixIndiceArray[(i-1)]);

	delete[] prefixIndiceArray;

}

WP6PrefixData::~WP6PrefixData()
{
	DPH::iterator pos;
	for (pos = m_prefixDataPacketHash.begin(); pos!=m_prefixDataPacketHash.end(); pos++) {
		delete(pos->second);
	}
}

const WP6PrefixDataPacket * WP6PrefixData::getPrefixDataPacket(const int prefixID) const
{
	DPH::const_iterator pos = m_prefixDataPacketHash.find(prefixID);
	if (pos != m_prefixDataPacketHash.end())
		return static_cast<const WP6PrefixDataPacket *>(pos->second);
	else
		return 0;
}

std::pair<MPDP_CIter, MPDP_CIter> WP6PrefixData::getPrefixDataPacketsOfType(const int type) const
{
	std::pair<MPDP_CIter, MPDP_CIter> tempPair = m_prefixDataPacketTypeHash.equal_range(type);

 	return tempPair;
}
