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

#ifndef WP6GENERALTEXTPACKET_H
#define WP6GENERALTEXTPACKET_H
#include "WP6PrefixDataPacket.h"
#include "WP6FileStructure.h"
#include "WP6LLListener.h"

class WP6GeneralTextPacket : public WP6PrefixDataPacket
{
public:
	WP6GeneralTextPacket(GsfInput *input, int id, guint32 dataOffset, guint32 dataSize);
	virtual ~WP6GeneralTextPacket();
	virtual void _readContents(GsfInput *input);
	virtual void parse(WP6LLListener *llListener) const;

private:              
	guint16 m_numTextBlocks;
	guint32 m_firstTextBlockOffset;
	guint32 *m_blockSizes;

	GsfInput *m_stream;
	
};
#endif /* WP6GENERALTEXTPACKET_H */
