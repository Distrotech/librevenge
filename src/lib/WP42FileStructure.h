/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003-2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef WP42FILESTRUCTURE_H
#define WP42FILESTRUCTURE_H

// size of the functiongroups 0xC0 to 0xFF
extern int WP42_FUNCTION_GROUP_SIZE[64]; 

#define WP42_ATTRIBUTE_BOLD 0
#define WP42_ATTRIBUTE_ITALICS 1
#define WP42_ATTRIBUTE_UNDERLINE 2
#define WP42_ATTRIBUTE_STRIKE_OUT 3
#define WP42_ATTRIBUTE_SHADOW 4
#define WP42_ATTRIBUTE_REDLINE 5

#endif /* WP42FILESTRUCTURE_H */
