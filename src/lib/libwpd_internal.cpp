/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002,2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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
#include "libwpd_internal.h"
#include "WPXStream.h"

uint8_t readU8(WPXInputStream *input)
{
	size_t numBytesRead;
	uint8_t const * p = input->read(sizeof(uint8_t), numBytesRead);
	
  	if (numBytesRead != sizeof(uint8_t))
  		throw FileException();

	return WPD_LE_GET_GUINT8(p);
}

int8_t read8(WPXInputStream *input)
{
	size_t numBytesRead;
	int8_t const * p = (int8_t const *) input->read(sizeof(int8_t), numBytesRead);

  	if (numBytesRead != sizeof(int8_t))
  		throw FileException();

	return (int8_t)*(p);
}

uint16_t readU16(WPXInputStream *input, bool bigendian)
{
	size_t numBytesRead;
	uint16_t const *val = (uint16_t const *)input->read(sizeof(uint16_t), numBytesRead);

	if (numBytesRead != sizeof(uint16_t))
  		throw FileException();

	if (bigendian)
		return WPD_BE_GET_GUINT16(val);
	return WPD_LE_GET_GUINT16(val);
}

uint32_t readU32(WPXInputStream *input, bool bigendian)
{
	size_t numBytesRead;
	uint32_t const *val = (uint32_t const *)input->read(sizeof(uint32_t), numBytesRead);

	if (numBytesRead != sizeof(uint32_t))
  		throw FileException();

	if (bigendian)
		return WPD_BE_GET_GUINT32(val);
	return WPD_LE_GET_GUINT32(val);
}
