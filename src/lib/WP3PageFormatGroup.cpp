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

#include <math.h>
#include "WP3PageFormatGroup.h"
#include "WP3FileStructure.h"
#include "WP3LLListener.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"

WP3PageFormatGroup::WP3PageFormatGroup(WPXInputStream *input) :
	WP3VariableLengthGroup(),
	m_leftMargin(0),
	m_rightMargin(0),
	m_topMargin(0),
	m_bottomMargin(0),
	m_lineSpacing(1.0f),
	m_justification(0),
	m_indent(0)
{
	_read(input);
}

WP3PageFormatGroup::~WP3PageFormatGroup()
{
}

void WP3PageFormatGroup::_readContents(WPXInputStream *input)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP3_PAGE_FORMAT_GROUP_HORIZONTAL_MARGINS:
		// skip 8 bytes (old values of no interest for us)
		input->seek(8, WPX_SEEK_CUR);
		m_leftMargin = readU32(input, true);
		m_rightMargin = readU32(input, true);
		WPD_DEBUG_MSG(("WordPerfect: Page format group horizontal margins\n"));
		break;
		
	case WP3_PAGE_FORMAT_GROUP_LINE_SPACING:
		// skip 4 bytes (old spacing of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		{
			uint32_t lineSpacing = readU32(input, true);
			int16_t lineSpacingIntegerPart = (int16_t)((lineSpacing & 0xFFFF0000) >> 16);
			float lineSpacingFractionalPart = (float)((double)(lineSpacing & 0xFFFF)/(double)0xFFFF);
			WPD_DEBUG_MSG(("WordPerfect: Page format group line spacing - integer part: %i fractional part: %f (original value: %i)\n",
				       lineSpacingIntegerPart, lineSpacingFractionalPart, lineSpacing));
			m_lineSpacing = lineSpacingIntegerPart + lineSpacingFractionalPart;
		}
		break;
		
	case WP3_PAGE_FORMAT_GROUP_VERTICAL_MARGINS:
		// skip 8 bytes (old values of no interest for us)
		input->seek(8, WPX_SEEK_CUR);
		m_topMargin = readU32(input, true);
		m_bottomMargin = readU32(input, true);
		WPD_DEBUG_MSG(("WordPerfect: Page format group vertical margins\n"));
		break;
		
	case WP3_PAGE_FORMAT_GROUP_JUSTIFICATION_MODE:
		// skip 1 byte (old justifcation value of no interest for us)
		input->seek(1, WPX_SEEK_CUR);
		m_justification = readU8(input);
		break;
		
	case WP3_PAGE_FORMAT_GROUP_INDENT_AT_BEGINNING_OF_PARAGRAPH:
		// skip 4 bytes (old indent indent value of no interest for us)
		input->seek(4, WPX_SEEK_CUR);
		m_indent = readU32(input, true);
		break;
		
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3PageFormatGroup::parse(WP3HLListener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Page group\n"));

	switch (getSubGroup())
	{
	case WP3_PAGE_FORMAT_GROUP_HORIZONTAL_MARGINS:
		if (m_leftMargin != 0x80000000)
		{
			int16_t leftMarginIntegerPart = (int16_t)((m_leftMargin & 0xFFFF0000) >> 16);
			float leftMarginFractionalPart = (float)((double)(m_leftMargin & 0xFFFF)/(double)0xFFFF);
			uint16_t leftMarginWPU = (uint16_t)rint((((float)leftMarginIntegerPart + leftMarginFractionalPart)*50)/3);
			listener->marginChange(WPX_LEFT, leftMarginWPU);
			WPD_DEBUG_MSG(("WordPerfect: Page format group left margin - integer part: %i fractional part: %f WPUs: %i (original value: %i)\n",
				       leftMarginIntegerPart, leftMarginFractionalPart, leftMarginWPU, m_leftMargin));
		}
		if (m_rightMargin != 0x8000000)
		{
			int16_t rightMarginIntegerPart = (int16_t)((m_rightMargin & 0xFFFF0000) >> 16);
			float rightMarginFractionalPart = (float)((double)(m_rightMargin & 0xFFFF)/(double)0xFFFF);
			uint16_t rightMarginWPU = (uint16_t)rint((((float)rightMarginIntegerPart + rightMarginFractionalPart)*50)/3);
			WPD_DEBUG_MSG(("WordPerfect: Page format group right margin - integer part: %i fractional part: %f WPUs: %i (original value: %i)\n",
				       rightMarginIntegerPart, rightMarginFractionalPart, rightMarginWPU, m_rightMargin));
			listener->marginChange(WPX_RIGHT, rightMarginWPU);
		}
		break;
		
	case WP3_PAGE_FORMAT_GROUP_LINE_SPACING:
	        WPD_DEBUG_MSG(("WordPerfect: parsing a line spacing change of: %f\n", m_lineSpacing));
	        listener->lineSpacingChange(m_lineSpacing);
		break;
				
	case WP3_PAGE_FORMAT_GROUP_VERTICAL_MARGINS:
		if (m_topMargin != 0x80000000)
		{
			int16_t topMarginIntegerPart = (int16_t)((m_topMargin & 0xFFFF0000) >> 16);
			float topMarginFractionalPart = (float)((double)(m_topMargin & 0xFFFF)/(double)0xFFFF);
			uint16_t topMarginWPU = (uint16_t)rint((((float)topMarginIntegerPart + topMarginFractionalPart)*50)/3);
			listener->pageMarginChange(WPX_TOP, topMarginWPU);
		}
		if (m_bottomMargin != 0x80000000)
		{
			int16_t bottomMarginIntegerPart = (int16_t)((m_bottomMargin & 0xFFFF0000) >> 16);
			float bottomMarginFractionalPart = (float)((double)(m_bottomMargin & 0xFFFF)/(double)0xFFFF);
			uint16_t bottomMarginWPU = (uint16_t)rint((((float)bottomMarginIntegerPart + bottomMarginFractionalPart)*50)/3);
			listener->pageMarginChange(WPX_BOTTOM, bottomMarginWPU);
		}
		break;

	case WP3_PAGE_FORMAT_GROUP_JUSTIFICATION_MODE:
		listener->justificationChange(m_justification);
		break;

	case WP3_PAGE_FORMAT_GROUP_INDENT_AT_BEGINNING_OF_PARAGRAPH:
		{
			int16_t indentIntegerPart = (int16_t)((m_indent & 0xFFFF0000) >> 16);
			float indentFractionalPart = (float)((double)(m_indent & 0xFFFF)/(double)0xFFFF);
			uint16_t indentWPU =  (int16_t)rint((((float)indentIntegerPart + indentFractionalPart)*50)/3);
			listener->indentFirstLineChange(indentWPU);
		}
		break;
		

	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
