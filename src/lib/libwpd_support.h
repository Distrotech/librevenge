
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

#ifndef LIBWPD_SUPPORT_H
#define LIBWPD_SUPPORT_H
#include <glib.h>

extern guint16 extendedInternationalCharacterMap[];
extern guint16 multinationalMap[];
extern guint16 phoneticMap[];
extern guint16 typographicMap[];
extern guint16 iconicMap[];
extern guint16 mathMap[];
extern guint16 mathextMap[];
extern guint16 greekMap[];
extern guint16 hebrewMap[];
extern guint16 cyrillicMap[];
extern guint16 japaneseMap[];

guint16 extendedCharacterToUCS2(guint8 character, guint8 characterSet);

enum OrderedListType { ARABIC, LOWERCASE, UPPERCASE, LOWERCASE_ROMAN, UPPERCASE_ROMAN };

typedef struct _RGBSColor RGBSColor;
struct _RGBSColor
{
	_RGBSColor(guint8 r, guint8 g, guint8 b, guint8 s);	
	_RGBSColor(); // initializes all values to 0
	guint8 m_r;
	guint8 m_g;
 	guint8 m_b;
	guint8 m_s; 
};

// UCSString: minimal string class, basically an object-oriented wrapper around glib's UCS4 string
// (so we don't have to pull in yet more dependencies)
class UCSString
{
public:
	UCSString();
	UCSString(const UCSString &);
	~UCSString();
	// UCS2 conversion not needed (yet)
	// const guint16 * getUCS2();
	void append(guint32);
	void append(const UCSString &);
	void append(const gchar *);
	void clear();

	const guint32 * getUCS4() const { return (guint32 *)m_stringBuf->data; }
	const int getLen() const { return m_stringBuf->len; }

private:
	GArray *m_stringBuf;
};

class UTF8String
{
public:
	UTF8String(const UCSString &, gboolean doConvertToValidXML = FALSE);
	~UTF8String() { g_free(m_buf); }
	const gchar * getUTF8() const { return m_buf; }
	const int getLen() const { return m_len; }
private:
	gchar *m_buf;
	glong m_len;
};

class FileException
{
	// needless to say, we could flesh this class out a bit
};

class ParseException
{
	// needless to say, we could flesh this class out a bit
};
#endif
