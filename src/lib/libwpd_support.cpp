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

#include "libwpd.h"
#include <string.h>

_RGBSColor::_RGBSColor(guint8 r, guint8 g, guint8 b, guint8 s)
	:	m_r(r),
		m_g(g),
		m_b(b),
		m_s(s)
{
}

UCSString::UCSString() : m_stringBuf(g_array_new(TRUE, FALSE, sizeof(guint32)))
{
}

UCSString::UCSString(const UCSString &stringBuf) : m_stringBuf(g_array_new(TRUE, FALSE, sizeof(guint32)))
{
	g_array_insert_vals(m_stringBuf, 0, stringBuf.getUCS4(), stringBuf.getLen()); 
}

UCSString::~UCSString()
{
	g_array_free(m_stringBuf, TRUE);
}

void UCSString::append(guint32 c)
{
	g_array_append_val(m_stringBuf, c);
}

void UCSString::append(const UCSString &stringBuf)
{
	m_stringBuf = g_array_append_vals(m_stringBuf, stringBuf.getUCS4(), stringBuf.getLen());
}

// append: appends an ascii-standard (not UTF8!!) string onto the buffer
void UCSString::append(const gchar *buf)
{
	for (int i=0; i<strlen(buf); i++)
	{
		append((guint32)buf[i]);
	}
}

void UCSString::clear()
{
	m_stringBuf = g_array_set_size(m_stringBuf, 0);			
}

UTF8String::UTF8String(const UCSString &stringBuf, gboolean doConvertToValidXML)
{
	if (doConvertToValidXML) 
	{
		UCSString tempUCS4;
		for (guint i=0; i<stringBuf.getLen(); i++) {
			if (stringBuf.getUCS4()[i] == '&') {
				tempUCS4.append("&amp;");
			}
			else if (stringBuf.getUCS4()[i] == (guint16)'<') {
				tempUCS4.append("&lt;");
			}
			else if (stringBuf.getUCS4()[i] == (guint16)'>') {
				tempUCS4.append("&gt;");
			}
			else {
				tempUCS4.append(stringBuf.getUCS4()[i]);
			}
		}

		m_buf = g_ucs4_to_utf8((const gunichar *)tempUCS4.getUCS4(), tempUCS4.getLen(), NULL, &m_len, NULL); // TODO: handle errors
	}
	else 
		m_buf = g_ucs4_to_utf8((const gunichar *)stringBuf.getUCS4(), stringBuf.getLen(), NULL, &m_len, NULL); // TODO: handle errors
}
