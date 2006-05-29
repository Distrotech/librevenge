/* libwpd
 * Copyright (C) 2003 David Mandelin (mandelin@cs.wisc.edu)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

#include "libwpd_math.h"

#if defined(_WIN32) && !defined(__MINGW32__)

double rint(double x) 
{
	double y, z;
	int n;

	if(x >= 0) 
	{
		y = x + 0.5;
		z = floor(y);
		n = static_cast<int>(z);
		if (y == z && n % 2) --z;
	} 
	else 
	{
		y = x - 0.5;
		z = ceil(y);
		n = static_cast<int>(z);
		if(y == z && n % 2) ++z;
	}
	return z;
}

#endif
