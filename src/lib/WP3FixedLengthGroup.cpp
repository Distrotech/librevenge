/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3FixedLengthGroup.h"
#include "WP3FileStructure.h"
#include "WP3AttributeGroup.h"
#include "WP3UnsupportedFixedLengthGroup.h"
#include "WP3ExtendedCharacterGroup.h"
#include "WP3TabGroup.h"
#include "WP3IndentGroup.h"
#include "WP3UndoGroup.h"
#include "libwpd_internal.h"

WP3FixedLengthGroup::WP3FixedLengthGroup(const uint8_t groupID)
	: m_group(groupID)
{
}

WP3FixedLengthGroup * WP3FixedLengthGroup::constructFixedLengthGroup(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	switch (groupID) 
	{
		case WP3_EXTENDED_CHARACTER_GROUP:
			return new WP3ExtendedCharacterGroup(input, encryption, groupID);
		
		case WP3_TAB_GROUP:
			return new WP3TabGroup(input, encryption, groupID);
		
		case WP3_INDENT_GROUP:
			return new WP3IndentGroup(input, encryption, groupID);
			
		case WP3_UNDO_GROUP:
			return new WP3UndoGroup(input, encryption, groupID);
			
		case WP3_ATTRIBUTE_GROUP:
			return new WP3AttributeGroup(input, encryption, groupID);
			
		// Add the remaining cases here
		default:
			return new WP3UnsupportedFixedLengthGroup(input, encryption, groupID);
	}
}

bool WP3FixedLengthGroup::isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	uint32_t startPosition = input->tell();

	try
	{
		int size = WP3_FIXED_LENGTH_FUNCTION_GROUP_SIZE[groupID-0xC0];
		if (input->seek((startPosition + size - 2), WPX_SEEK_SET) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (groupID != readU8(input, encryption))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		
		input->seek(startPosition, WPX_SEEK_SET);
		return true;
	}
	catch(...)
	{
		input->seek(startPosition, WPX_SEEK_SET);
		return false;
	}
}

void WP3FixedLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	uint32_t startPosition = input->tell();
	_readContents(input, encryption);
	
	if (m_group >= 0xC0 && m_group <= 0xCF) // just an extra safety check
	{
		int size = WP3_FIXED_LENGTH_FUNCTION_GROUP_SIZE[m_group-0xC0];
		input->seek((startPosition + size - 2), WPX_SEEK_SET);
		if (m_group != readU8(input, encryption))
		{
			WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
			throw FileException();
		}
	}
	else
		throw FileException();
}
