/* libwpd
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include <math.h>
#include "WP3DisplayGroup.h"
#include "WP3FileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"
#include "WP3Listener.h"

WP3DisplayGroup::WP3DisplayGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_noteReference(),
	m_pageNumber()	
{
	_read(input, encryption);
}

WP3DisplayGroup::~WP3DisplayGroup()
{
}

void WP3DisplayGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch (getSubGroup())
	{
	case WP3_DISPLAY_GROUP_INSERT_PAGE_NUMBER:
		input->seek(4, WPX_SEEK_CUR);
		m_pageNumber = readPascalString(input, encryption);
		break;
	case WP3_DISPLAY_GROUP_INSERT_FOOTNOTE_NUMBER:
	case WP3_DISPLAY_GROUP_INSERT_ENDNOTE_NUMBER:
		input->seek(4, WPX_SEEK_CUR);
		m_noteReference = readPascalString(input, encryption);
		break;
	default:
		break;
	}
}

void WP3DisplayGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Display group\n"));

	switch (getSubGroup())
	{
	case WP3_DISPLAY_GROUP_INSERT_PAGE_NUMBER:
		listener->insertPageNumber(m_pageNumber);
		break;
	case WP3_DISPLAY_GROUP_INSERT_FOOTNOTE_NUMBER:
	case WP3_DISPLAY_GROUP_INSERT_ENDNOTE_NUMBER:
		listener->insertNoteReference(m_noteReference);
		break;
	default:
		break;
	}
}
