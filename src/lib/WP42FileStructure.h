/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#ifndef WP42FILESTRUCTURE_H
#define WP42FILESTRUCTURE_H

// size of the functiongroups 0xC0 to 0xF8
extern int WP42_FUNCTION_GROUP_SIZE[63]; 

#define WP42_ATTRIBUTE_BOLD 0
#define WP42_ATTRIBUTE_ITALICS 1
#define WP42_ATTRIBUTE_UNDERLINE 2
#define WP42_ATTRIBUTE_STRIKE_OUT 3
#define WP42_ATTRIBUTE_SHADOW 4
#define WP42_ATTRIBUTE_REDLINE 5

#define WP42_MARGIN_RESET_GROUP 0xC0

#define WP42_SUPPRESS_PAGE_CHARACTERISTICS_GROUP 0xCF

#define WP42_HEADER_FOOTER_GROUP 0xD1
#define WP42_HEADER_FOOTER_GROUP_ALL_BIT 1
#define WP42_HEADER_FOOTER_GROUP_ODD_BIT 2
#define WP42_HEADER_FOOTER_GROUP_EVEN_BIT 4

#endif /* WP42FILESTRUCTURE_H */
