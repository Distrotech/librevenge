/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP3MiscellaneousGroup.h"
#include "WP3FileStructure.h"
#include "WP3LLListener.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"

WP3MiscellaneousGroup::WP3MiscellaneousGroup(WPXInputStream *input) :
	WP3VariableLengthGroup(),
	m_pageWidth(0),
	m_pageHeight(0),
	m_pageOrientation(PORTRAIT),
	m_isPersistent(true)
{
	_read(input);
}

WP3MiscellaneousGroup::~WP3MiscellaneousGroup()
{
}

void WP3MiscellaneousGroup::_readContents(WPXInputStream *input)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP3_MISCELLANEOUS_GROUP_PAGE_SIZE_OVERRIDE:
		uint32_t tmpPageWidth;
		uint32_t tmpPageHeight;
		uint16_t tmpPageOrientation;
		
		// skip 20 bytes of old values
		input->seek(20, WPX_SEEK_CUR);
		
		// read the new values
		tmpPageOrientation = readU16(input, true);
		tmpPageWidth = readU32(input, true);
		tmpPageHeight = readU32(input, true);
		
		// determine whether the orientation lasts only one page or is persistent
		if ((tmpPageOrientation & 0x8000) == 0x0000)
			m_isPersistent = false;
		else 
			m_isPersistent = true;
		
		// determine whether it is portrait or landscape
		if ((tmpPageOrientation & 0x0001) == 0x0000)
			m_pageOrientation = PORTRAIT;
		else
			m_pageOrientation = LANDSCAPE;
		
		// compute page dimensions in WPUs
		{
			int16_t pageWidthIntegerPart = (int16_t)((tmpPageWidth & 0xFFFF0000) >> 16);
			float pageWidthFractionalPart = (float)(tmpPageWidth & 0xFFFF)/(float)0xFFFF;
			m_pageWidth = (uint16_t)rint((((float)pageWidthIntegerPart + pageWidthFractionalPart)*50)/3);
			int16_t pageHeightIntegerPart = (int16_t)((tmpPageHeight & 0xFFFF0000) >> 16);
			float pageHeightFractionalPart = (float)(tmpPageHeight & 0xFFFF)/(float)0xFFFF;
			m_pageHeight = (uint16_t)rint((((float)pageHeightIntegerPart + pageHeightFractionalPart)*50)/3);
		}
		break;
		
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3MiscellaneousGroup::parse(WP3HLListener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Miscellaneous group\n"));

	switch (getSubGroup())
	{
	case WP3_MISCELLANEOUS_GROUP_PAGE_SIZE_OVERRIDE:
		listener->pageFormChange(m_pageHeight, m_pageWidth, m_pageOrientation, m_isPersistent);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
