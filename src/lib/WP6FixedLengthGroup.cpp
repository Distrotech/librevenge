/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#include "WP6FixedLengthGroup.h"

#include "WP6ExtendedCharacterGroup.h"
#include "WP6UndoGroup.h"
#include "WP6AttributeOnGroup.h"
#include "WP6AttributeOffGroup.h"

WP6FixedLengthGroup::WP6FixedLengthGroup(WPXParser * parser)
	: WP6Part(parser)
{
}

WP6FixedLengthGroup * WP6FixedLengthGroup::constructFixedLengthGroup(WPXParser * parser, guint8 groupID)
{
	switch (groupID)
	{
		case WP6_TOP_EXTENDED_CHARACTER: 
			return new WP6ExtendedCharacterGroup(parser);
		
		case WP6_TOP_UNDO_GROUP:
			return new WP6UndoGroup(parser);
		
		case WP6_TOP_ATTRIBUTE_ON:
			return new WP6AttributeOnGroup(parser);
		
		case WP6_TOP_ATTRIBUTE_OFF:
			return new WP6AttributeOffGroup(parser);
		
		// Add the remaining cases here
		default:
			// should not happen
			return NULL;
	}
}

gboolean WP6FixedLengthGroup::parse()
{
	FILE * stream = _getParser()->getStream();
	guint32 startPosition = ftell(stream);
	WPD_CHECK_INTERNAL_ERROR( _parseContents() );	
	WPD_CHECK_FILE_SEEK_ERROR(fseek(stream, (startPosition + m_iSize - 1 - ftell(stream)), SEEK_CUR));
}
