/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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
#include "WP6LLListener.h"
#include "WP6PrefixData.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"

const WP6PrefixDataPacket * WP6LLListener::_getPrefixDataPacket(const int prefixID) const
{ 
	return m_prefixData->getPrefixDataPacket(prefixID); 
}

const WP6DefaultInitialFontPacket * WP6LLListener::_getDefaultInitialFontPacket() const
{
	if (m_prefixData->getDefaultInitialFontPID() > 0)
		{
			return dynamic_cast<const WP6DefaultInitialFontPacket *>(_getPrefixDataPacket(m_prefixData->getDefaultInitialFontPID()));
		}
	return NULL;
}

