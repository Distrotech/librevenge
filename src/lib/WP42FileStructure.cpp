/* libwpd2
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

#include "WP42FileStructure.h"

// size of the functiongroups 0xC0 and 0xF8
// -1 means the size is variable
int WP42_FUCNTION_GROUP_SIZE[57] = 
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
	32,	
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
	-1	// 0xF8
};
