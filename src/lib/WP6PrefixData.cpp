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

#include "WP6PrefixData.h"
#include "WP6PrefixIndice.h"
#include "WP6PrefixDataPacket.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

// void parsePrefixDataPacketNotify(gpointer key, gpointer value, gpointer user_data);
// void destroyPrefixDataPacketKeyNotify(gpointer data);
// void destroyPrefixDataPacketNotify(gpointer data);

WP6PrefixData::WP6PrefixData(GsfInput *input, const int numPrefixIndices) :
	m_defaultInitialFontPID((-1))

{
	WP6PrefixIndice ** prefixIndiceArray = new (WP6PrefixIndice *)[(numPrefixIndices-1)];
	for (guint16 i=1; i<numPrefixIndices; i++)
	{
		WPD_DEBUG_MSG(("WordPerfect: constructing prefix indice 0x%x\n", i));
		prefixIndiceArray[(i-1)] = new WP6PrefixIndice(input, i);
	}
	
	for (guint16 i=1; i<numPrefixIndices; i++) 
		{
			WPD_DEBUG_MSG(("WordPerfect: constructing prefix packet 0x%x\n", i));
			WP6PrefixDataPacket *prefixDataPacket = WP6PrefixDataPacket::constructPrefixDataPacket(input, prefixIndiceArray[(i-1)]);
			if (prefixDataPacket) {
// 				gint *key = new gint;
// 				*key = i;
				// g_hash_table_insert(m_prefixDataPacketHash, (gpointer)key, (gpointer)prefixDataPacket);
				m_prefixDataPacketHash[i] = prefixDataPacket;
				if (dynamic_cast<WP6DefaultInitialFontPacket *>(prefixDataPacket))
					m_defaultInitialFontPID = i;
			}
			
	}

	for (guint16 i=1; i<numPrefixIndices; i++)
		delete(prefixIndiceArray[(i-1)]);

	delete[] prefixIndiceArray;

}

WP6PrefixData::~WP6PrefixData()
{
	typedef map<int, WP6PrefixDataPacket *>::iterator Iter;
	for (Iter packet = m_prefixDataPacketHash.begin(); packet!=m_prefixDataPacketHash.end(); packet++) {
		delete(packet->second);
	}
 	//g_hash_table_destroy(m_prefixDataPacketHash);
}

const WP6PrefixDataPacket * WP6PrefixData::getPrefixDataPacket(const int prefixID) const
{
	return static_cast<const WP6PrefixDataPacket *>(m_prefixDataPacketHash.find(prefixID)->second); //(const WP6PrefixDataPacket *)g_hash_table_lookup(m_prefixDataPacketHash, &prefixID);
}

void WP6PrefixData::parse(WP6LLListener *llListener)
{
	typedef map<int, WP6PrefixDataPacket *>::iterator Iter;
	for (Iter packet = m_prefixDataPacketHash.begin(); packet!=m_prefixDataPacketHash.end(); ++packet) {
		packet->second->parse(llListener);
	}
	//g_hash_table_foreach(m_prefixDataPacketHash, parsePrefixDataPacketNotify, llListener);
}

