/* libwpd
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
#include "libwpd_internal.h"
#include "WPXStream.h"
#include <gsf/gsf-utils.h>

guint8 readU8(WPXInputStream *input)
{
	return GSF_LE_GET_GUINT8(input->read(sizeof(guint8)));
}

gint8 read8(WPXInputStream *input)
{
	return (gint8)*(input->read(sizeof(gint8)));
}

guint16 readU16(WPXInputStream *input)
{
	guint16 val = *(guint16 const *)input->read(sizeof(guint16));
	return GSF_LE_GET_GUINT16(&val);
}

guint32 readU32(WPXInputStream *input)
{
	guint32 val = *(guint32 const *)input->read(sizeof(guint32));
	return GSF_LE_GET_GUINT32(&val);
}
