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

#ifndef WP6PREFIXPACKET_H
#define WP6PREFIXPACKET_H

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

class WP6PrefixPacket
{
 public:
	WP6PrefixPacket(FILE * stream, guint8 flags);
	static WP6PrefixPacket * constructPrefixPacket(FILE * stream);
 
 protected:
 	virtual void _read(FILE *stream);
	virtual void _readContents(FILE *stream) {} // we don't always need more information than that provided generically
 
 private:
 	guint8 m_flags;
 	guint16	m_useCount;
 	guint16 m_hideCount;
 	guint32 m_dataSize;
 	guint32 m_dataOffset;
 
 	bool m_hasChildren;
};

#endif /* WP6PREFIXPACKET_H */
