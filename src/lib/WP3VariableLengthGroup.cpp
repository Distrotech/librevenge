/* libwpd
 * Copyright (C) 2004 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004-2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3FileStructure.h"
#include "WP3VariableLengthGroup.h"
#include "WP3UnsupportedVariableLengthGroup.h"
#include "WP3EndOfLinePageGroup.h"
#include "WP3MiscellaneousGroup.h"
#include "WP3PageFormatGroup.h"
#include "WP3FontGroup.h"
#include "WP3DefinitionGroup.h"
#include "WP3DisplayGroup.h"
#include "WP3HeaderFooterGroup.h"
#include "WP3FootnoteEndnoteGroup.h"
#include "WP3TablesGroup.h"
#include "WP3WindowGroup.h"
#include "libwpd_internal.h"
#include <limits>

WP3VariableLengthGroup::WP3VariableLengthGroup() :
	m_subGroup(0),
	m_size(0)
{
}

WP3VariableLengthGroup * WP3VariableLengthGroup::constructVariableLengthGroup(WPXInputStream *input, WPXEncryption *encryption, const uint8_t group)
{
	switch (group)
	{
		case WP3_PAGE_FORMAT_GROUP:
			return new WP3PageFormatGroup(input, encryption);
		case WP3_END_OF_LINE_PAGE_GROUP:
			return new WP3EndOfLinePageGroup(input, encryption);
		case WP3_MISCELLANEOUS_GROUP:
			return new WP3MiscellaneousGroup(input, encryption);
		case WP3_TABLES_GROUP:
			return new WP3TablesGroup(input, encryption);
		case WP3_FONT_GROUP:
			return new WP3FontGroup(input, encryption);
		case WP3_DEFINITION_GROUP:
			return new WP3DefinitionGroup(input, encryption);
		case WP3_HEADER_FOOTER_GROUP:
			return new WP3HeaderFooterGroup(input, encryption);
		case WP3_FOOTNOTE_ENDNOTE_GROUP:
			return new WP3FootnoteEndnoteGroup(input, encryption);
		case WP3_DISPLAY_GROUP:
			return new WP3DisplayGroup(input, encryption);
		case WP3_WINDOW_GROUP:
			return new WP3WindowGroup(input, encryption);
		default:
			// this is an unhandled group, just skip it
			return new WP3UnsupportedVariableLengthGroup(input, encryption);
	}
}

bool WP3VariableLengthGroup::isGroupConsistent(WPXInputStream *input, WPXEncryption *encryption, const uint8_t group)
{
	uint32_t startPosition = input->tell();
	if (startPosition > ((std::numeric_limits<unsigned long>::max)() / 2))
		return false;

	try
	{
		uint8_t subGroup = readU8(input, encryption);
		uint16_t size = readU16(input, encryption, true);
		if (startPosition + size < startPosition)
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}

		if (input->seek((startPosition + size - 1), WPX_SEEK_SET) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (size != readU16(input, encryption, true))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (subGroup != readU8(input, encryption))
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

void WP3VariableLengthGroup::_read(WPXInputStream *input, WPXEncryption *encryption)
{
	uint32_t startPosition = input->tell();

	WPD_DEBUG_MSG(("WordPerfect: handling a variable length group\n"));	
	
	m_subGroup = readU8(input, encryption);
	m_size = readU16(input, encryption, true) + 4; // the length is the number of data bytes minus 4 (ie. the function codes)
	
	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, sub_group: 0x%x, size: %i)\n", startPosition, m_subGroup, m_size));
	
	_readContents(input, encryption);
	
	input->seek((startPosition + m_size - 5), WPX_SEEK_SET);

	if (m_size != (readU16(input, encryption, true) + 4))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}
	if (m_subGroup != readU8(input, encryption))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}
	
	input->seek((startPosition + m_size - 1), WPX_SEEK_SET);

}
