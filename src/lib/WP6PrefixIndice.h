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

#ifndef WP6PREFIXINDICE_H
#define WP6PREFIXINDICE_H
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

class WP6PrefixIndice
{
 public:
	WP6PrefixIndice(FILE * stream, int id);	
	const int getID() const { return m_id; }
	const guint8 getType() const { return m_type; }
	const guint32 getDataSize() const { return m_dataSize; }
	const guint32 getDataOffset() const { return m_dataOffset; }

 protected:
 	void _read(FILE *stream);
 
 private:
	int m_id;
	guint8 m_type;
 	guint8 m_flags;
 	guint16	m_useCount;
 	guint16 m_hideCount;
 	guint32 m_dataSize;
 	guint32 m_dataOffset;
 
 	bool m_hasChildren;
};

#endif /* WP6PREFIXINDICE_H */
