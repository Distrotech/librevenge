
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

extern const guint16 asciiMap[];
extern const guint16 extendedInternationalCharacterMap[];
extern const guint16 multinationalMap[];
extern const guint16 phoneticMap[];
extern const guint16 typographicMap[];
extern const guint16 iconicMap[];
extern const guint16 mathMap[];
extern const guint16 mathextMap[];
extern const guint16 greekMap[];
extern const guint16 hebrewMap[];
extern const guint16 cyrillicMap[];
extern const guint16 japaneseMap[];
extern const guint16 *tibetanMap1[];

int extendedCharacterToUCS2(guint8 character, guint8 characterSet,
			    const guint16 **chars);

enum WPXNumberingType { ARABIC, LOWERCASE, UPPERCASE, LOWERCASE_ROMAN, UPPERCASE_ROMAN };
enum WPXNoteType { FOOTNOTE, ENDNOTE };

// ATTRIBUTE bits
#define WPX_EXTRALARGE_BIT 1
#define WPX_VERYLARGE_BIT 2
#define WPX_LARGE_BIT 4
#define WPX_SMALLPRINT_BIT 8
#define WPX_FINEPRINT_BIT 16
#define WPX_SUPERSCRIPT_BIT 32
#define WPX_SUBSCRIPT_BIT 64
#define WPX_OUTLINE_BIT 128
#define WPX_ITALICS_BIT 256
#define WPX_SHADOW_BIT 512
#define WPX_REDLINE_BIT 1024
#define WPX_DOUBLEUNDERLINE_BIT 2048
#define WPX_BOLD_BIT 4096
#define WPX_STRIKEOUT_BIT 8192
#define WPX_UNDERLINE_BIT 16384
#define WPX_SMALLCAPS_BIT 32768
#define WPX_BLINK_BIT 65536
#define WPX_REVERSEVIDEO_BIT 131072

// JUSTIFICATION bits.
#define WPX_PARAGRAPH_JUSTIFICATION_LEFT 0x00
#define WPX_PARAGRAPH_JUSTIFICATION_FULL 0x01
#define WPX_PARAGRAPH_JUSTIFICATION_CENTER 0x02
#define WPX_PARAGRAPH_JUSTIFICATION_RIGHT 0x03
#define WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES 0x04
#define WPX_PARAGRAPH_JUSTIFICATION_RESERVED 0x05

// TABLE POSITION bits.
#define WPX_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN 0x00
#define WPX_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN 0x01
#define WPX_TABLE_POSITION_CENTER_BETWEEN_MARGINS 0x02
#define WPX_TABLE_POSITION_FULL 0x03
#define WPX_TABLE_POSITION_ABSOLUTE_FROM_LEFT_MARGIN 0x04

// TABLE CELL BORDER bits
const guint8 WPX_TABLE_CELL_LEFT_BORDER_OFF = 0x01;
const guint8 WPX_TABLE_CELL_RIGHT_BORDER_OFF = 0x02;
const guint8 WPX_TABLE_CELL_TOP_BORDER_OFF = 0x04;
const guint8 WPX_TABLE_CELL_BOTTOM_BORDER_OFF = 0x08;

// BREAK bits
#define WPX_PAGE_BREAK 0x00
#define WPX_COLUMN_BREAK 0x02

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

typedef struct _WPXColumnDefinition WPXColumnDefinition;
struct _WPXColumnDefinition
{
	_WPXColumnDefinition(); // initializes all values to 0
	float m_width;
	float m_leftGutter;
	float m_rightGutter;
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
	UTF8String(const UTF8String &);
	UTF8String(const UCSString &, bool convertToValidXML = false);
	//UTF8String(const gchar *);
	UTF8String(const gchar *format, ...);
	~UTF8String() { g_string_free(m_buf, TRUE); }
	const gchar * getUTF8() const { return m_buf->str; }
	const int getLen() const { return m_buf->len; }
private:
	GString *m_buf;
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
