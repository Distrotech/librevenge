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

#ifndef WP6FONTDESCRIPTORPACKET_H
#define WP6FONTDESCRIPTORPACKET_H
#include "WP6PrefixDataPacket.h"

class WP6FontDescriptorPacket : public WP6PrefixDataPacket
{
 public:
	WP6FontDescriptorPacket(GsfInput *input, int id, guint32 dataOffset, guint32 dataSize);
	virtual ~WP6FontDescriptorPacket();
	virtual void _readContents(GsfInput *input);
	const gchar *getFontName() const { return m_fontName; }

 private:
	guint16 m_characterWidth;
	guint16 m_ascenderHeight;
	guint16 m_xHeight;
	guint16 m_descenderHeight;
	guint16 m_italicsAdjust;
	guint8 m_primaryFamilyId; // family id's are supposed to be one unified element, but I split them up to ease parsing
	guint8 m_primaryFamilyMemberId;
	
	guint8 m_scriptingSystem;
	guint8 m_primaryCharacterSet;
	guint8 m_width;
	guint8 m_weight; 
	guint8 m_attributes;
	guint8 m_generalCharacteristics;
	guint8 m_classification;
	guint8 m_fill; // fill byte
	guint8 m_fontType;
	guint8 m_fontSourceFileType;

	guint16 m_fontNameLength;

	gchar *m_fontName; 
};
#endif
