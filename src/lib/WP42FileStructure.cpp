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

#include "WP42FileStructure.h"

// size of the function groups 0xC0 to 0xFE
// -1 means the size is variable
int WP42_FUNCTION_GROUP_SIZE[63] = 
{
	6,	// 0xC0
	4,
	3,	
	5,	
	5,	
	6,	
	4,	
	6,	
	8,	
	42,	
	3,	
	6,	
	4,	
	3,	
	4,
	3,	
	6,	// 0xD0
	-1,	
	-1,	
	4,	
	4,	
	4,	
	6,	
	-1,	
	4,	
	4,	
	4,	
	4,	
	-1,	
	24,	
	4,	
	-1,	
	4,	// 0XE0
	3,	
	-1,	
	150,	
	6,	
	23,	
	11,		
	3,	
	3,	
	-1,	
	-1,	
	-1,	// 0XEB Documentation lies that the size is 32, but it is not true.	
	4,	
	-1,	
	44,	
	18,	
	6,	// 0XF0
	106,	
	-1,	
	100,	
	4,	
	-1,	
	5,	
	-1,
	-1,
	-1,	// 0xF9
	-1,	// 0xFA
	-1,	// 0xFB
	-1,	// 0xFC
	-1,	// 0xFD
	-1	// 0xFE
};
