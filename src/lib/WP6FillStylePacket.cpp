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
#include <gsf/gsf-input-memory.h>

#include "WP6FillStylePacket.h"
#include "WP6Parser.h"
#include "libwpd_internal.h"

WP6FillStylePacket::WP6FillStylePacket(GsfInput *input, int id, guint32 dataOffset, guint32 dataSize) 
	: WP6PrefixDataPacket(input)
{	
	_read(input, dataOffset, dataSize);
}


WP6FillStylePacket::~WP6FillStylePacket()
{

}

const int WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_PREFIX_PACKETS = 6;
const int WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_FILL_NAME = 3;
void WP6FillStylePacket::_readContents(GsfInput *input)
{
	/* skip a whole bunch of useless crap */
	guint16 numChildPrefixIDs = gsf_le_read_guint16(input);
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, sizeof(guint16)*numChildPrefixIDs, G_SEEK_CUR));
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_PREFIX_PACKETS, G_SEEK_CUR));
	gint16 fillNameLength = gsf_le_read_guint16(input);
	if (fillNameLength > 0)
		WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, fillNameLength*sizeof(guint8), G_SEEK_CUR));
	WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, WP6_FILL_STYLE_PACKET_SKIPABLE_DATA_AFTER_FILL_NAME, G_SEEK_CUR));

	/* now we can finally grab the meat */
	m_fgColor.m_r = gsf_le_read_guint8(input);
	m_fgColor.m_g = gsf_le_read_guint8(input);
	m_fgColor.m_b = gsf_le_read_guint8(input);
	m_fgColor.m_s = gsf_le_read_guint8(input);
	m_bgColor.m_r = gsf_le_read_guint8(input);
	m_bgColor.m_g = gsf_le_read_guint8(input);
	m_bgColor.m_b = gsf_le_read_guint8(input);
	m_bgColor.m_s = gsf_le_read_guint8(input);
	WPD_DEBUG_MSG(("WordPerfect: Fill Prefix Packet FG Color (%i, %i, %i, %i) BG Color (%i, %i, %i, %i)\n",
		       m_fgColor.m_r, m_fgColor.m_g, m_fgColor.m_b, m_fgColor.m_s,
		       m_bgColor.m_r, m_bgColor.m_g, m_bgColor.m_b, m_bgColor.m_s));

}
