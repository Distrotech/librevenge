/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
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

#include "WP1VariableLengthGroup.h"
#include "WP1UnsupportedVariableLengthGroup.h"
#include "WP1SetTabsGroup.h"
#include "WP1HeaderFooterGroup.h"
#include "WP1FootnoteEndnoteGroup.h"
#include "WP1PictureGroup.h"
#include "WP1FileStructure.h"
#include "libwpd_internal.h"
#include <limits>

WP1VariableLengthGroup::WP1VariableLengthGroup(uint8_t group) :
	m_group(group),
	m_size(0)
{
}

WP1VariableLengthGroup * WP1VariableLengthGroup::constructVariableLengthGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group)
{
	switch (group)
	{
		case WP1_SET_TABS_GROUP:
			return new WP1SetTabsGroup(input, encryption, group);
		case WP1_HEADER_FOOTER_GROUP:
			return new WP1HeaderFooterGroup(input, encryption, group);
		case WP1_FOOTNOTE_ENDNOTE_GROUP:
			return new WP1FootnoteEndnoteGroup(input, encryption, group);
		case WP1_PICTURE_GROUP:
			return new WP1PictureGroup(input, encryption, group);
		default:
			// this is an unhandled group, just skip it
			return new WP1UnsupportedVariableLengthGroup(input, encryption, group);
	}
}

bool WP1VariableLengthGroup::isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t group)
{
	uint32_t startPosition = input->tell();

	try
	{
		uint32_t size = readU32(input, encryption, true);
		if (size > ((std::numeric_limits<uint32_t>::max)() / 2))
			return false;

		if (input->seek(size, WPX_SEEK_CUR) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (size != readU32(input, encryption, true))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (group != readU8(input, encryption))
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

void WP1VariableLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	uint32_t startPosition = input->tell();

	WPD_DEBUG_MSG(("WordPerfect: handling a variable length group\n"));	
	
	m_size = readU32(input, encryption, true); // the length is the number of data bytes minus 4 (ie. the function codes)

	if (m_size + startPosition < startPosition)
		throw FileException(); 
	
	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, size: %i)\n", startPosition, m_size));
	
	_readContents(input, encryption);
	
	if ((m_size + startPosition + 4 < m_size + startPosition) ||
		(m_size + startPosition + 4) > ((std::numeric_limits<uint32_t>::max)() / 2))
		throw FileException(); 
	
	input->seek(startPosition + m_size + 4, WPX_SEEK_SET);

	if (m_size != readU32(input, encryption, true))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}
	if (m_group != readU8(input, encryption))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}
	
	if ((m_size + startPosition + 9 < m_size + startPosition) ||
		(m_size + startPosition + 9) > ((std::numeric_limits<uint32_t>::max)() / 2))
		throw FileException(); 
	input->seek(startPosition + m_size + 9, WPX_SEEK_SET);

}
