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

#include "WP5FixedLengthGroup.h"
#include "WP5FileStructure.h"
#include "libwpd_internal.h"

WP5FixedLengthGroup::WP5FixedLengthGroup(guint groupID)
	: m_group(groupID)
{
}

WP5FixedLengthGroup * WP5FixedLengthGroup::constructFixedLengthGroup(GsfInput *input, guint8 groupID)
{
	switch (groupID) 
	{
			
		// Add the remaining cases here
		default:
			// should not happen
			return NULL;
	}
}

void WP5FixedLengthGroup::_read(GsfInput *input)
{
	guint32 startPosition = gsf_input_tell(input);
	_readContents(input);
	
	if (m_group >= 0xC0 && m_group <= 0xCF) // just an extra safety check
	{
		int size = WP5_FIXED_LENGTH_FUCNTION_GROUP_SIZE[m_group-0xC0];
		WPD_CHECK_FILE_SEEK_ERROR(gsf_input_seek(input, (startPosition + size - 1 - gsf_input_tell(input)), G_SEEK_CUR));
	}
	else
		throw FileException();
}
