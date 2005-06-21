/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef WP5FILESTRUCTURE_H
#define WP5FILESTRUCTURE_H

// size of the fixed length functiongroups 0xC0 to 0xCF
extern int WP5_FIXED_LENGTH_FUNCTION_GROUP_SIZE[16]; 

#define WP5_ATTRIBUTE_EXTRA_LARGE 0x00
#define WP5_ATTRIBUTE_VERY_LARGE 0x01
#define WP5_ATTRIBUTE_LARGE 0x02
#define WP5_ATTRIBUTE_SMALL_PRINT 0x03
#define WP5_ATTRIBUTE_FINE_PRINT 0x04
#define WP5_ATTRIBUTE_SUPERSCRIPT 0x05
#define WP5_ATTRIBUTE_SUBSCRIPT 0x06
#define WP5_ATTRIBUTE_OUTLINE 0x07
#define WP5_ATTRIBUTE_ITALICS 0x08
#define WP5_ATTRIBUTE_SHADOW 0x09
#define WP5_ATTRIBUTE_REDLINE 0x0A
#define WP5_ATTRIBUTE_DOUBLE_UNDERLINE 0xB
#define WP5_ATTRIBUTE_BOLD 0x0C
#define WP5_ATTRIBUTE_STRIKE_OUT 0x0D
#define WP5_ATTRIBUTE_UNDERLINE 0x0E
#define WP5_ATTRIBUTE_SMALL_CAPS 0x0F

/* Main function group list  */

#define WP5_TOP_EXTENDED_CHARACTER 0xC0
#define WP5_TOP_ATTRIBUTE_ON 0xC3
#define WP5_TOP_ATTRIBUTE_OFF 0xC4
#define WP5_TOP_PAGE_FORMAT_GROUP 0xD0
#define WP5_TOP_FONT_GROUP 0xD1
#define WP5_TOP_FORMAT_GROUP 0xD4

/* Page Format Group */
#define WP5_TOP_PAGE_FORMAT_GROUP_LEFT_RIGHT_MARGIN_SET 0x01
#define WP5_TOP_PAGE_FORMAT_GROUP_SPACING_SET 0x02
#define WP5_TOP_PAGE_FORMAT_GROUP_TOP_BOTTOM_MARGIN_SET 0x05
#define WP5_TOP_PAGE_FORMAT_GROUP_JUSTIFICATION 0x06
#define WP5_TOP_PAGE_FORMAT_GROUP_FORM 0x0B

/* Font Group */
#define WP5_TOP_FONT_GROUP_COLOR 0x00
#define WP5_TOP_FONT_GROUP_FONT_CHANGE 0x01 

/* Format Group */
#define WP5_FORMAT_GROUP_END_OF_PAGE 0x00

/* Extended Character */
#define WP5_INTERNATIONAL_1_CHARACTER_SET 1
#define WP5_INTERNATIONAL_2_CHARACTER_SET 2
#define WP5_BOX_DRAWING_CHARACTER_SET 3
#define WP5_TYPOGRAPHIC_SYMBOL_CHARACTER_SET 4
#define WP5_ICONIC_SYMBOL_CHARACTER_SET 5
#define WP5_MATH_SCIENTIFIC_CHARACTER_SET 6
#define WP5_MATH_SCIENTIFIC_EXTENDED_CHARACTER_SET 7
#define WP5_GREEK_CHARACTER_SET 8
#define WP5_HEBREW_CHARACTER_SET 9
#define WP5_CYRILLIC_CHARACTER_SET 10
#define WP5_JAPANESE_CHARACTER_SET 11

#define WP5_NUM_INTERNATIONAL_1_CHARACTERS 234
#define WP5_NUM_INTERNATIONAL_2_CHARACTERS 28
#define WP5_NUM_BOX_DRAWING_CHARACTERS 88
#define WP5_NUM_TYPOGRAPHIC_CHARACTERS 85
#define WP5_NUM_ICONIC_CHARACTERS 35
#define WP5_NUM_MATH_SCIENTIFIC_CHARACTERS 56
#define WP5_NUM_MATH_SCIENTIFIC_EXTENDED_CHARACTERS 229
#define WP5_NUM_GREEK_CHARACTERS 210
#define WP5_NUM_HEBREW_CHARACTERS 44
#define WP5_NUM_CYRILLIC_CHARACTERS 150
#define WP5_NUM_JAPANESE_CHARACTERS 185

#endif /* WP5FILESTRUCTURE_H */
