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

#ifndef WP6DEFAULTINITIALFONTPACKET_H
#define WP6DEFAULTINITIALFONTPACKET_H
#include "WP6PrefixDataPacket.h"

class WP6DefaultInitialFontPacket : public WP6PrefixDataPacket
{
 public:
	WP6DefaultInitialFontPacket(GsfInput *input, int id, guint32 dataOffset, guint32 dataSize);
	virtual void _readContents(GsfInput *input);
	const guint16 getInitialFontDescriptorPID() const { return m_initialFontDescriptorPID; }
	const guint16 getPointSize() const { return m_pointSize; }
	virtual ParseResult parse(WP6LLListener *llListener) const;
	
 private:
	guint16 m_numPrefixIDs;
	guint16 m_initialFontDescriptorPID;
	guint16 m_pointSize;
};
#endif
