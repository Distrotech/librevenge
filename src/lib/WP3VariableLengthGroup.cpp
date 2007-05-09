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
#include "libwpd_internal.h"
#include <limits>

WP3VariableLengthGroup::WP3VariableLengthGroup() :
	m_subGroup(0),
	m_size(0)
{
}

WP3VariableLengthGroup * WP3VariableLengthGroup::constructVariableLengthGroup(WPXInputStream *input, const uint8_t group)
{
	switch (group)
	{
		case WP3_PAGE_FORMAT_GROUP:
			return new WP3PageFormatGroup(input);
		case WP3_END_OF_LINE_PAGE_GROUP:
			return new WP3EndOfLinePageGroup(input);
		case WP3_MISCELLANEOUS_GROUP:
			return new WP3MiscellaneousGroup(input);
		case WP3_TABLES_GROUP:
			return new WP3TablesGroup(input);
		case WP3_FONT_GROUP:
			return new WP3FontGroup(input);
		case WP3_DEFINITION_GROUP:
			return new WP3DefinitionGroup(input);
		case WP3_HEADER_FOOTER_GROUP:
			return new WP3HeaderFooterGroup(input);
		case WP3_FOOTNOTE_ENDNOTE_GROUP:
			return new WP3FootnoteEndnoteGroup(input);
		case WP3_DISPLAY_GROUP:
			return new WP3DisplayGroup(input);
		default:
			// this is an unhandled group, just skip it
			return new WP3UnsupportedVariableLengthGroup(input);
	}
}

bool WP3VariableLengthGroup::isGroupConsistent(WPXInputStream *input, const uint8_t group)
{
	uint32_t startPosition = input->tell();
	if (startPosition > ((std::numeric_limits<unsigned long>::max)() / 2))
		return false;

	try
	{
		uint8_t subGroup = readU8(input);
		uint16_t size = readU16(input, true);
		if (startPosition + size < startPosition)
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}

		if (input->seek((startPosition + size - 1 - input->tell()), WPX_SEEK_CUR) || input->atEOS())
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (size != readU16(input, true))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (subGroup != readU8(input))
		{
			input->seek(startPosition, WPX_SEEK_SET);
			return false;
		}
		if (group != readU8(input))
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

void WP3VariableLengthGroup::_read(WPXInputStream *input)
{
	uint32_t startPosition = input->tell();

	WPD_DEBUG_MSG(("WordPerfect: handling a variable length group\n"));	
	
	m_subGroup = readU8(input);
	m_size = readU16(input, true) + 4; // the length is the number of data bytes minus 4 (ie. the function codes)
	
	WPD_DEBUG_MSG(("WordPerfect: Read variable group header (start_position: %i, sub_group: 0x%x, size: %i)\n", startPosition, m_subGroup, m_size));
	
	_readContents(input);
	
	input->seek((startPosition + m_size - 5 - input->tell()), WPX_SEEK_CUR);

	if (m_size != (readU16(input, true) + 4))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}
	if (m_subGroup != readU8(input))
	{
		WPD_DEBUG_MSG(("WordPerfect: Possible corruption detected. Bailing out!\n"));
		throw FileException();
	}
	
	input->seek((startPosition + m_size - 1 - input->tell()), WPX_SEEK_CUR);

}
