/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6FileStructure.h"

// size of the fixed length functiongroups 0xF0 to 0xFF
int WP6_FIXED_LENGTH_FUNCTION_GROUP_SIZE[16] =
{
	4,	// 0xF0
	5,	// 0xF1
	3,	// 0xF2
	3,	// 0xF3
	3,	// 0xF4
	3,	// 0xF5
	4,	// 0xF6
	4,	// 0xF7
	4,	// 0xF8
	5,	// 0xF9
	5,	// 0xFA
	6,	// 0xFB
	6,	// 0xFC
	8,	// 0xFD
	8,	// 0xFE
	-1	// 0xFF - Cannot be used. -1 is reserved so no size is assigned to this value.
};
