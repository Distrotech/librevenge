/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002,2004 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef LIBWPD_INTERNAL_H
#define LIBWPD_INTERNAL_H
#include "WPXStream.h"
#include <stdio.h>
#include <string>
#include <algorithm>
#include "WPXString.h"
#include "WPXEncryption.h"
#include "libwpd_types.h"

/* Various functions/defines that need not/should not be exported externally */

#ifdef _MSC_VER
#include <minmax.h>
#define LIBWPD_MIN min
#define LIBWPD_MAX max
#else
#define LIBWPD_MIN std::min
#define LIBWPD_MAX std::max
#endif

#define WPD_CHECK_FILE_ERROR(v) if (v==EOF) { WPD_DEBUG_MSG(("X_CheckFileError: %d\n", __LINE__)); throw FileException(); }
#define WPD_CHECK_FILE_SEEK_ERROR(v) if (v) { WPD_DEBUG_MSG(("X_CheckFileSeekError: %d\n", __LINE__)); throw FileException(); }
#define WPD_CHECK_FILE_READ_ERROR(v,num_elements) if (v != num_elements) {\
 WPD_DEBUG_MSG(("X_CheckFileReadElementError: %d\n", __LINE__)); throw FileException(); }

#define DELETEP(m) if (m) { delete m; m = 0; }
 
#ifdef DEBUG
#define WPD_DEBUG_MSG(M) printf M
#else
#define WPD_DEBUG_MSG(M)
#endif

#define WPD_LE_GET_GUINT8(p) (*(uint8_t const *)(p))
#define WPD_LE_GET_GUINT16(p)				  \
        (uint16_t)((((uint8_t const *)(p))[0] << 0)  |    \
                  (((uint8_t const *)(p))[1] << 8))
#define WPD_LE_GET_GUINT32(p) \
        (uint32_t)((((uint8_t const *)(p))[0] << 0)  |    \
                  (((uint8_t const *)(p))[1] << 8)  |    \
                  (((uint8_t const *)(p))[2] << 16) |    \
                  (((uint8_t const *)(p))[3] << 24))

#define WPD_BE_GET_GUINT8(p) (*(uint8_t const *)(p))
#define WPD_BE_GET_GUINT16(p)                           \
        (uint16_t)((((uint8_t const *)(p))[1] << 0)  |    \
                  (((uint8_t const *)(p))[0] << 8))
#define WPD_BE_GET_GUINT32(p)                           \
        (uint32_t)((((uint8_t const *)(p))[3] << 0)  |    \
                  (((uint8_t const *)(p))[2] << 8)  |    \
                  (((uint8_t const *)(p))[1] << 16) |    \
                  (((uint8_t const *)(p))[0] << 24))

// add more of these as needed for byteswapping
// (the 8-bit functions are just there to make things consistent)
uint8_t readU8(WPXInputStream *input, WPXEncryption *encryption); 
uint16_t readU16(WPXInputStream *input, WPXEncryption *encryption, bool bigendian=false);
uint32_t readU32(WPXInputStream *input, WPXEncryption *encryption, bool bigendian=false);

WPXString readPascalString(WPXInputStream *input, WPXEncryption *encryption);
WPXString readCString(WPXInputStream *input, WPXEncryption *encryption);

void appendUCS4(WPXString &str, uint32_t ucs4);

// Various helper structures for the libwpd parser..

int extendedCharacterWP6ToUCS2(uint8_t character, uint8_t characterSet,
			    const uint16_t **chars);

int extendedCharacterWP5ToUCS2(uint8_t character, uint8_t characterSet,
			    const uint16_t **chars);

uint16_t fixedPointToWPUs(const uint32_t fixedPointNumber);
double fixedPointToDouble(const uint32_t fixedPointNumber);
double wpuToFontPointSize(const uint16_t wpuNumber);

enum WPXFileType { WP6_DOCUMENT, WP5_DOCUMENT, WP42_DOCUMENT, OTHER };
enum WPXNumberingType { ARABIC, LOWERCASE, UPPERCASE, LOWERCASE_ROMAN, UPPERCASE_ROMAN };
enum WPXNoteType { FOOTNOTE, ENDNOTE };
enum WPXHeaderFooterType { HEADER, FOOTER };
enum WPXHeaderFooterInternalType { HEADER_A, HEADER_B, FOOTER_A, FOOTER_B, DUMMY };
enum WPXHeaderFooterOccurence { ODD, EVEN, ALL, NEVER };
enum WPXPageNumberPosition { PAGENUMBER_POSITION_NONE = 0, PAGENUMBER_POSITION_TOP_LEFT, PAGENUMBER_POSITION_TOP_CENTER, 
			     PAGENUMBER_POSITION_TOP_RIGHT, PAGENUMBER_POSITION_TOP_LEFT_AND_RIGHT, 
			     PAGENUMBER_POSITION_BOTTOM_LEFT, PAGENUMBER_POSITION_BOTTOM_CENTER, 
			     PAGENUMBER_POSITION_BOTTOM_RIGHT, PAGENUMBER_POSITION_BOTTOM_LEFT_AND_RIGHT, 
			     PAGENUMBER_POSITION_TOP_INSIDE_LEFT_AND_RIGHT, 
			     PAGENUMBER_POSITION_BOTTOM_INSIDE_LEFT_AND_RIGHT };

enum WPXFormOrientation { PORTRAIT, LANDSCAPE };
enum WPXTabAlignment { LEFT, RIGHT, CENTER, DECIMAL, BAR };
enum WPXVerticalAlignment { TOP, MIDDLE, BOTTOM, FULL };

enum WPXTextColumnType { NEWSPAPER, NEWSPAPER_VERTICAL_BALANCE, PARALLEL, PARALLEL_PROTECT };

enum WPXSubDocumentType { WPX_SUBDOCUMENT_NONE, WPX_SUBDOCUMENT_HEADER_FOOTER, WPX_SUBDOCUMENT_NOTE, WPX_SUBDOCUMENT_TEXT_BOX, WPX_SUBDOCUMENT_COMMENT_ANNOTATION };

// ATTRIBUTE bits
#define WPX_EXTRA_LARGE_BIT 1
#define WPX_VERY_LARGE_BIT 2
#define WPX_LARGE_BIT 4
#define WPX_SMALL_PRINT_BIT 8
#define WPX_FINE_PRINT_BIT 16
#define WPX_SUPERSCRIPT_BIT 32
#define WPX_SUBSCRIPT_BIT 64
#define WPX_OUTLINE_BIT 128
#define WPX_ITALICS_BIT 256
#define WPX_SHADOW_BIT 512
#define WPX_REDLINE_BIT 1024
#define WPX_DOUBLE_UNDERLINE_BIT 2048
#define WPX_BOLD_BIT 4096
#define WPX_STRIKEOUT_BIT 8192
#define WPX_UNDERLINE_BIT 16384
#define WPX_SMALL_CAPS_BIT 32768
#define WPX_BLINK_BIT 65536
#define WPX_REVERSEVIDEO_BIT 131072

// JUSTIFICATION bits.
#define WPX_PARAGRAPH_JUSTIFICATION_LEFT 0x00
#define WPX_PARAGRAPH_JUSTIFICATION_FULL 0x01
#define WPX_PARAGRAPH_JUSTIFICATION_CENTER 0x02
#define WPX_PARAGRAPH_JUSTIFICATION_RIGHT 0x03
#define WPX_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES 0x04
#define WPX_PARAGRAPH_JUSTIFICATION_DECIMAL_ALIGNED 0x05

// TABLE POSITION bits.
#define WPX_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN 0x00
#define WPX_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN 0x01
#define WPX_TABLE_POSITION_CENTER_BETWEEN_MARGINS 0x02
#define WPX_TABLE_POSITION_FULL 0x03
#define WPX_TABLE_POSITION_ABSOLUTE_FROM_LEFT_MARGIN 0x04

// TABLE CELL BORDER bits
const uint8_t WPX_TABLE_CELL_LEFT_BORDER_OFF = 0x01;
const uint8_t WPX_TABLE_CELL_RIGHT_BORDER_OFF = 0x02;
const uint8_t WPX_TABLE_CELL_TOP_BORDER_OFF = 0x04;
const uint8_t WPX_TABLE_CELL_BOTTOM_BORDER_OFF = 0x08;

// BREAK bits
#define WPX_PAGE_BREAK 0x00
#define WPX_SOFT_PAGE_BREAK 0x01
#define WPX_COLUMN_BREAK 0x02

// Generic bits
#define WPX_LEFT 0x00
#define WPX_RIGHT 0x01
#define WPX_CENTER 0x02
#define WPX_TOP 0x03
#define WPX_BOTTOM 0x04

typedef struct _RGBSColor RGBSColor;
struct _RGBSColor
{
	_RGBSColor(uint8_t r, uint8_t g, uint8_t b, uint8_t s);
	_RGBSColor(uint16_t red, uint16_t green, uint16_t blue); // Construct
	// RBBSColor from double precision RGB color used by WP3.x for Mac
	_RGBSColor(); // initializes all values to 0
	uint8_t m_r;
	uint8_t m_g;
 	uint8_t m_b;
	uint8_t m_s;
};

typedef struct _WPXColumnDefinition WPXColumnDefinition;
struct _WPXColumnDefinition
{
	_WPXColumnDefinition(); // initializes all values to 0
	double m_width;
	double m_leftGutter;
	double m_rightGutter;
};

typedef struct _WPXColumnProperties WPXColumnProperties;
struct _WPXColumnProperties
{
	_WPXColumnProperties();
	uint32_t m_attributes;
	uint8_t m_alignment;
};

typedef struct _WPXTabStop WPXTabStop;
struct _WPXTabStop
{
	_WPXTabStop(double position, WPXTabAlignment alignment, uint16_t leaderCharacter, uint8_t leaderNumSpaces);
	_WPXTabStop();
	double m_position;
	WPXTabAlignment m_alignment;
	uint16_t m_leaderCharacter;
	uint8_t m_leaderNumSpaces;
};

// Various exceptions: libwpd does not propagate exceptions externally..

class VersionException
{
};

class FileException
{
};

class ParseException
{
};

class GenericException
{
};

class UnsupportedEncryptionException
{
};

class SupportedEncryptionException
{
};

class WrongPasswordException
{
};

// Various usefull, but cheesey functions

int _extractNumericValueFromRoman(const char romanChar);
int _extractDisplayReferenceNumberFromBuf(const WPXString &buf, const WPXNumberingType listType);
WPXNumberingType _extractWPXNumberingTypeFromBuf(const WPXString &buf, const WPXNumberingType putativeWPXNumberingType);
WPXString _numberingTypeToString(WPXNumberingType t);
extern const uint16_t macintoshCharacterMap[];
WPXString doubleToString(const double value);

#endif /* LIBWPD_INTERNAL_H */
