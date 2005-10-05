/* libwpd
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004-2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP3FILESTRUCTURE_H
#define WP3FILESTRUCTURE_H

// size of the fixed length functiongroups 0xC0 to 0xCF
extern int WP3_FIXED_LENGTH_FUNCTION_GROUP_SIZE[16]; 

/* Main function group list  */

#define WP3_EXTENDED_CHARACTER_GROUP 0xC0

#define WP3_INDENT_GROUP 0xC2
#define WP3_ATTRIBUTE_GROUP 0xC3

#define WP3_UNDO_GROUP 0xCD

#define WP3_PAGE_FORMAT_GROUP 0xD0

#define WP3_MISCELLANEOUS_GROUP 0xD9
#define WP3_WINDOW_GROUP 0xDA
#define WP3_STYLE_GROUP 0xDB
#define WP3_END_OF_LINE_PAGE_GROUP 0xDC

#define WP3_TABLES_GROUP 0xE2

/* Attribute Group */
#define WP3_ATTRIBUTE_BOLD 0
#define WP3_ATTRIBUTE_ITALICS 1
#define WP3_ATTRIBUTE_UNDERLINE 2
#define WP3_ATTRIBUTE_OUTLINE 3
#define WP3_ATTRIBUTE_SHADOW 4
// 5 to 7 is not documented
#define WP3_ATTRIBUTE_REDLINE 8
#define WP3_ATTRIBUTE_STRIKE_OUT 9
#define WP3_ATTRIBUTE_SUBSCRIPT 10
#define WP3_ATTRIBUTE_SUPERSCRIPT 11
#define WP3_ATTRIBUTE_DOUBLE_UNDERLINE 12
#define WP3_ATTRIBUTE_EXTRA_LARGE 13
#define WP3_ATTRIBUTE_VERY_LARGE 14
#define WP3_ATTRIBUTE_LARGE 15
#define WP3_ATTRIBUTE_SMALL_PRINT 16
#define WP3_ATTRIBUTE_FINE_PRINT 17
#define WP3_ATTRIBUTE_SMALL_CAPS 18

/* Page Format Group */
#define WP3_PAGE_FORMAT_GROUP_HORIZONTAL_MARGINS 0x01
#define WP3_PAGE_FORMAT_GROUP_LINE_SPACING 0x02
#define WP3_PAGE_FORMAT_GROUP_VERTICAL_MARGINS 0x05
#define WP3_PAGE_FORMAT_GROUP_JUSTIFICATION_MODE 0x06
#define WP3_PAGE_FORMAT_GROUP_INDENT_AT_BEGINNING_OF_PARAGRAPH 0x0C

/* Miscellaneous Group */

#define WP3_MISCELLANEOUS_GROUP_PAGE_SIZE_OVERRIDE 0x04

/* Tables Group */

#define WP3_TABLES_GROUP_TABLE_FUNCTION 0x01
#define WP3_TABLES_GROUP_SET_TABLE_CELL_SPAN 0x02
#define WP3_TABLES_GROUP_SET_TABLE_CELL_TOP_LINE 0x03
#define WP3_TABLES_GROUP_SET_TABLE_CELL_LEFT_LINE 0x04
#define WP3_TABLES_GROUP_SET_TABLE_CELL_BOTTOM_LINE 0x05
#define WP3_TABLES_GROUP_SET_TABLE_CELL_RIGHT_LINE 0x06
#define WP3_TABLES_GROUP_SET_TABLE_CELL_TOP_LINE_COLOR 0x07
#define WP3_TABLES_GROUP_SET_TABLE_CELL_LEFT_LINE_COLOR 0x08
#define WP3_TABLES_GROUP_SET_TABLE_CELL_BOTTOM_LINE_COLOR 0x09
#define WP3_TABLES_GROUP_SET_TABLE_CELL_RIGHT_LINE_COLOR 0x0A
#define WP3_TABLES_GROUP_SET_TABLE_CELL_FILL_COLOR_PATTERN 0x0B
#define WP3_TABLES_GROUP_SET_TABLE_CELL_VERTICAL_ALIGNMENT 0x0C
#define WP3_TABLES_GROUP_SET_TABLE_CELL_MODE 0x0D
#define WP3_TABLES_GROUP_SET_TABLE_ROW_MODE 0x0E

#endif /* WP3FILESTRUCTURE_H */
