/* libwpd2
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

// size of the fixed length functiongroups 0xF0 to 0xFF
int WP6_FIXED_LENGTH_FUCNTION_GROUP_SIZE[16] = 
{
	4,	// 0xF0
	5,
	3,	
	3,	
	6,	
	6,	
	3,	
	3,	
	4,	
	4,	
	4,	
	5,	
	5,	
	8,	
	8,	
	-1	// 0xFF - Cannot be used. -1 is reserved so no size is assigned to this value.
};
