/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include <math.h>
#include <ctype.h>
#include "WPXListener.h"
#include "WP6Listener.h"
#include "WP6FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP6PrefixData.h"
#include "WP6FontDescriptorPacket.h"
#include "WP6DefaultInitialFontPacket.h"

WP6Listener::WP6Listener() :
	m_prefixData(0)
{
}

const WP6PrefixDataPacket * WP6Listener::getPrefixDataPacket(const int prefixID) const
{
	if (m_prefixData)
		return m_prefixData->getPrefixDataPacket(prefixID);
	else
		return 0;
}

WPXString WP6Listener::getFontNameForPID(const int prefixID) const
{
	
	const WP6FontDescriptorPacket *fontDescriptorPacket = 
	dynamic_cast<const WP6FontDescriptorPacket *>(getPrefixDataPacket(prefixID));
	if (fontDescriptorPacket)
		return fontDescriptorPacket->getFontName();
	return WPXString();
}
