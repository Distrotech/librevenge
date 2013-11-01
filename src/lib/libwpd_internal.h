/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002,2004 Marc Maurer (uwog@uwog.net)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://librevenge.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef LIBREVENGE_INTERNAL_H
#define LIBREVENGE_INTERNAL_H

#include <stdio.h>
#include <string>
#include <algorithm>
#include <librevenge/librevenge.h>
#include <librevenge-stream/librevenge-stream.h>
#include "RVNGEncryption.h"
#include "librevenge_types.h"

/* Various functions/defines that need not/should not be exported externally */

#ifdef _MSC_VER
#include <minmax.h>
#define LIBREVENGE_MIN min
#define LIBREVENGE_MAX max
#else
#define LIBREVENGE_MIN std::min
#define LIBREVENGE_MAX std::max
#endif

#define RVNG_CHECK_FILE_ERROR(v) if (v==EOF) { RVNG_DEBUG_MSG(("X_CheckFileError: %d\n", __LINE__)); throw FileException(); }
#define RVNG_CHECK_FILE_SEEK_ERROR(v) if (v) { RVNG_DEBUG_MSG(("X_CheckFileSeekError: %d\n", __LINE__)); throw FileException(); }
#define RVNG_CHECK_FILE_READ_ERROR(v,num_elements) if (v != num_elements) {\
 RVNG_DEBUG_MSG(("X_CheckFileReadElementError: %d\n", __LINE__)); throw FileException(); }

#define DELETEP(m) if (m) { delete m; m = 0; }

#ifdef DEBUG
#define RVNG_DEBUG_MSG(M) printf M
#else
#define RVNG_DEBUG_MSG(M)
#endif

#define RVNG_NUM_ELEMENTS(array) sizeof(array)/sizeof(array[0])

// add more of these as needed for byteswapping
// (the 8-bit functions are just there to make things consistent)
uint8_t readU8(RVNGInputStream *input, RVNGEncryption *encryption);
uint16_t readU16(RVNGInputStream *input, RVNGEncryption *encryption, bool bigendian=false);
int16_t readS16(RVNGInputStream *input, RVNGEncryption *encryption, bool bigendian=false);
uint32_t readU32(RVNGInputStream *input, RVNGEncryption *encryption, bool bigendian=false);

RVNGString readPascalString(RVNGInputStream *input, RVNGEncryption *encryption);
RVNGString readCString(RVNGInputStream *input, RVNGEncryption *encryption);

void appendUCS4(RVNGString &str, uint32_t ucs4);

// Various helper structures for the librevenge parser..

int extendedCharacterWP6ToUCS4(uint8_t character, uint8_t characterSet,
                               const uint32_t **chars);

int extendedCharacterWP5ToUCS4(uint8_t character, uint8_t characterSet,
                               const uint32_t **chars);

int appleWorldScriptToUCS4(uint16_t character, const uint32_t **chars);

int extendedCharacterWP42ToUCS4(uint8_t character, const uint32_t **chars);

uint16_t fixedPointToWPUs(const uint32_t fixedPointNumber);
double fixedPointToDouble(const uint32_t fixedPointNumber);
double wpuToFontPointSize(const uint16_t wpuNumber);

enum RVNGFileType { WP6_DOCUMENT, WP5_DOCUMENT, WP42_DOCUMENT, OTHER };
enum RVNGNumberingType { ARABIC, LOWERCASE, UPPERCASE, LOWERCASE_ROMAN, UPPERCASE_ROMAN };
enum RVNGNoteType { FOOTNOTE, ENDNOTE };
enum RVNGHeaderFooterType { HEADER, FOOTER };
enum RVNGHeaderFooterInternalType { HEADER_A, HEADER_B, FOOTER_A, FOOTER_B, DUMMY };
enum RVNGHeaderFooterOccurence { ODD, EVEN, ALL, NEVER };
enum RVNGPageNumberPosition { PAGENUMBER_POSITION_NONE = 0, PAGENUMBER_POSITION_TOP_LEFT, PAGENUMBER_POSITION_TOP_CENTER,
                             PAGENUMBER_POSITION_TOP_RIGHT, PAGENUMBER_POSITION_TOP_LEFT_AND_RIGHT,
                             PAGENUMBER_POSITION_BOTTOM_LEFT, PAGENUMBER_POSITION_BOTTOM_CENTER,
                             PAGENUMBER_POSITION_BOTTOM_RIGHT, PAGENUMBER_POSITION_BOTTOM_LEFT_AND_RIGHT,
                             PAGENUMBER_POSITION_TOP_INSIDE_LEFT_AND_RIGHT,
                             PAGENUMBER_POSITION_BOTTOM_INSIDE_LEFT_AND_RIGHT
                           };

enum RVNGFormOrientation { PORTRAIT, LANDSCAPE };
enum RVNGTabAlignment { LEFT, RIGHT, CENTER, DECIMAL, BAR };
enum RVNGVerticalAlignment { TOP, MIDDLE, BOTTOM, FULL };

enum RVNGTextColumnType { NEWSPAPER, NEWSPAPER_VERTICAL_BALANCE, PARALLEL, PARALLEL_PROTECT };

enum RVNGSubDocumentType { RVNG_SUBDOCUMENT_NONE, RVNG_SUBDOCUMENT_HEADER_FOOTER, RVNG_SUBDOCUMENT_NOTE, RVNG_SUBDOCUMENT_TEXT_BOX, RVNG_SUBDOCUMENT_COMMENT_ANNOTATION };

// ATTRIBUTE bits
#define RVNG_EXTRA_LARGE_BIT 1
#define RVNG_VERY_LARGE_BIT 2
#define RVNG_LARGE_BIT 4
#define RVNG_SMALL_PRINT_BIT 8
#define RVNG_FINE_PRINT_BIT 16
#define RVNG_SUPERSCRIPT_BIT 32
#define RVNG_SUBSCRIPT_BIT 64
#define RVNG_OUTLINE_BIT 128
#define RVNG_ITALICS_BIT 256
#define RVNG_SHADOW_BIT 512
#define RVNG_REDLINE_BIT 1024
#define RVNG_DOUBLE_UNDERLINE_BIT 2048
#define RVNG_BOLD_BIT 4096
#define RVNG_STRIKEOUT_BIT 8192
#define RVNG_UNDERLINE_BIT 16384
#define RVNG_SMALL_CAPS_BIT 32768
#define RVNG_BLINK_BIT 65536
#define RVNG_REVERSEVIDEO_BIT 131072

// JUSTIFICATION bits.
#define RVNG_PARAGRAPH_JUSTIFICATION_LEFT 0x00
#define RVNG_PARAGRAPH_JUSTIFICATION_FULL 0x01
#define RVNG_PARAGRAPH_JUSTIFICATION_CENTER 0x02
#define RVNG_PARAGRAPH_JUSTIFICATION_RIGHT 0x03
#define RVNG_PARAGRAPH_JUSTIFICATION_FULL_ALL_LINES 0x04
#define RVNG_PARAGRAPH_JUSTIFICATION_DECIMAL_ALIGNED 0x05

// TABLE POSITION bits.
#define RVNG_TABLE_POSITION_ALIGN_WITH_LEFT_MARGIN 0x00
#define RVNG_TABLE_POSITION_ALIGN_WITH_RIGHT_MARGIN 0x01
#define RVNG_TABLE_POSITION_CENTER_BETWEEN_MARGINS 0x02
#define RVNG_TABLE_POSITION_FULL 0x03
#define RVNG_TABLE_POSITION_ABSOLUTE_FROM_LEFT_MARGIN 0x04

// TABLE CELL BORDER bits
const uint8_t RVNG_TABLE_CELL_LEFT_BORDER_OFF = 0x01;
const uint8_t RVNG_TABLE_CELL_RIGHT_BORDER_OFF = 0x02;
const uint8_t RVNG_TABLE_CELL_TOP_BORDER_OFF = 0x04;
const uint8_t RVNG_TABLE_CELL_BOTTOM_BORDER_OFF = 0x08;

// BREAK bits
#define RVNG_PAGE_BREAK 0x00
#define RVNG_SOFT_PAGE_BREAK 0x01
#define RVNG_COLUMN_BREAK 0x02

// Generic bits
#define RVNG_LEFT 0x00
#define RVNG_RIGHT 0x01
#define RVNG_CENTER 0x02
#define RVNG_TOP 0x03
#define RVNG_BOTTOM 0x04

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

typedef struct _RVNGColumnDefinition RVNGColumnDefinition;
struct _RVNGColumnDefinition
{
	_RVNGColumnDefinition(); // initializes all values to 0
	double m_width;
	double m_leftGutter;
	double m_rightGutter;
};

typedef struct _RVNGColumnProperties RVNGColumnProperties;
struct _RVNGColumnProperties
{
	_RVNGColumnProperties();
	uint32_t m_attributes;
	uint8_t m_alignment;
};

typedef struct _RVNGTabStop RVNGTabStop;
struct _RVNGTabStop
{
	_RVNGTabStop();
	double m_position;
	RVNGTabAlignment m_alignment;
	uint32_t m_leaderCharacter;
	uint8_t m_leaderNumSpaces;
};

// Various exceptions: librevenge does not propagate exceptions externally..

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
int _extractDisplayReferenceNumberFromBuf(const RVNGString &buf, const RVNGNumberingType listType);
RVNGNumberingType _extractRVNGNumberingTypeFromBuf(const RVNGString &buf, const RVNGNumberingType putativeRVNGNumberingType);
RVNGString _numberingTypeToString(RVNGNumberingType t);
extern const uint32_t macRomanCharacterMap[];
RVNGString doubleToString(const double value);

#endif /* LIBREVENGE_INTERNAL_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
