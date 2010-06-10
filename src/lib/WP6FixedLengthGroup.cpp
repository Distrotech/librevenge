/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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

#include "WP6FixedLengthGroup.h"
#include "WP6ExtendedCharacterGroup.h"
#include "WP6UndoGroup.h"
#include "WP6AttributeGroup.h"
#include "WP6HighlightGroup.h"
#include "WP6UnsupportedFixedLengthGroup.h"
#include "WP6FileStructure.h"
#include "libwpd_internal.h"

WP6FixedLengthGroup::WP6FixedLengthGroup(uint8_t groupID)
	: m_group(groupID)
{
}

WP6FixedLengthGroup * WP6FixedLengthGroup::constructFixedLengthGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t groupID)
{
	switch (groupID)
	{
		case WP6_TOP_EXTENDED_CHARACTER:
			return new WP6ExtendedCharacterGroup(input, encryption, groupID);

		case WP6_TOP_UNDO_GROUP:
			return new WP6UndoGroup(input, encryption, groupID);

		case WP6_TOP_ATTRIBUTE_ON:
			return new WP6AttributeOnGroup(input, encryption, groupID);

		case WP6_TOP_ATTRIBUTE_OFF:
			return new WP6AttributeOffGroup(input, encryption, groupID);

		case WP6_TOP_HIGHLIGHT_ON:
			return new WP6HighlightOnGroup(input, encryption, groupID);

		case WP6_TOP_HIGHLIGHT_OFF:
			return new WP6HighlightOffGroup(input, encryption, groupID);

		// Add the remaining cases here
		default:
			return new WP6UnsupportedFixedLengthGroup(input, encryption, groupID);
	}
}

bool WP6FixedLengthGroup::isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t groupID)
{
	if (groupID == (uint8_t)0xFF)
		return false;

	uint32_t startPosition = input->tell();

	try
	{
		uint32_t size = WP6_FIXED_LENGTH_FUNCTION_GROUP_SIZE[groupID-0xF0];
		if (input->seek((startPosition + size - 2), WPX_SEEK_SET) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (input->atEOS() || groupID != readU8(input, encryption))
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

void WP6FixedLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	uint32_t startPosition = input->tell();
	_readContents(input, encryption);

	if (m_group >= 0xF0 && m_group < 0xFF) // just an extra safety check
	{
		int size = WP6_FIXED_LENGTH_FUNCTION_GROUP_SIZE[m_group-0xF0];
		input->seek((startPosition + size - 2), WPX_SEEK_SET);
		if (m_group != readU8(input, encryption))
		{
			WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected: bailing out!\n"));
			throw FileException();
		}
	}
	else
		throw FileException();
}
