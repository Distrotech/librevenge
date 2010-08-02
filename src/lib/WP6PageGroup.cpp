/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6PageGroup.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"

WP6PageGroup::WP6PageGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_margin(0),
	m_marginType(0),
	m_suppressedCode(0),
	m_formLength(0),
	m_formWidth(0),
	m_formType(0),
	m_formOrientation(PORTRAIT)
{
	_read(input, encryption);
}

WP6PageGroup::~WP6PageGroup()
{
}

void WP6PageGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP6_PAGE_GROUP_TOP_MARGIN_SET:
	case WP6_PAGE_GROUP_BOTTOM_MARGIN_SET:
		m_margin = readU16(input, encryption);
		WPD_DEBUG_MSG(("WordPerfect: Read page group margin size (margin: %i)\n", m_margin));
		break;
	case WP6_PAGE_GROUP_SUPPRESS_PAGE_CHARACTERISTICS:
		m_suppressedCode = readU8(input, encryption);
		WPD_DEBUG_MSG(("WordPerfect: Read suppressed code (%i)\n", m_suppressedCode));
		break;
        case WP6_PAGE_GROUP_PAGE_NUMBER_POSITION:
		m_pageNumberTypefaceDesc = readU16(input, encryption);
		m_pageNumberUseFlag = readU8(input, encryption);
		m_pageNumberingFontPIDCopy = readU16(input, encryption);
		m_pageNumberPointSize = readU16(input, encryption);
		m_pageNumberPosition = readU8(input, encryption);
		m_pageNumberMatchedFontIndex = readU16(input, encryption);
		m_pageNumberMatchedFontPointSize = readU16(input, encryption);
		m_pageNumberAttributes1 = readU16(input, encryption);
		m_pageNumberAttributes2 = readU16(input, encryption);
		m_pageNumberColor.m_r = readU8(input, encryption);
		m_pageNumberColor.m_g = readU8(input, encryption);
		m_pageNumberColor.m_b = readU8(input, encryption);
		m_pageNumberColor.m_s = readU8(input, encryption);
		m_pageNumberHeight = readU16(input, encryption);
		m_pageNumberNewPagePosition = readU8(input, encryption);
		break;
	case WP6_PAGE_GROUP_FORM:
		uint8_t tmpOrientation;
		// skip Hash values that we do not use (2+1 bytes)
		input->seek(3, WPX_SEEK_CUR);
		m_formLength = readU16(input, encryption);
		m_formWidth = readU16(input, encryption);
		m_formType = readU8(input, encryption);
		tmpOrientation = readU8(input, encryption);
		switch (tmpOrientation)
		{
		case 0x01:
			m_formOrientation = LANDSCAPE;
			break;
		case 0x00:
			m_formOrientation = PORTRAIT;
			break;
		default:
			m_formOrientation = PORTRAIT;
			break;
		}
		WPD_DEBUG_MSG(("WordPerfect: Read form information (length: %i), (width: %i), (form orientation: %s),\n",
						m_formLength, m_formWidth, ((m_formOrientation==PORTRAIT)?"portrait":"landscape")));
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP6PageGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Page group\n"));

	switch (getSubGroup())
	{
        case WP6_PAGE_GROUP_PAGE_NUMBER_POSITION:
		if (m_pageNumberUseFlag == 0 || !getNumPrefixIDs())
			listener->pageNumberingChange((WPXPageNumberPosition)m_pageNumberPosition, 0, 0);
		else
			listener->pageNumberingChange((WPXPageNumberPosition)m_pageNumberPosition, m_pageNumberMatchedFontPointSize, getPrefixIDs()[0]);
		break;
	case WP6_PAGE_GROUP_TOP_MARGIN_SET:
		listener->pageMarginChange(WPX_TOP, m_margin);
		break;
	case WP6_PAGE_GROUP_BOTTOM_MARGIN_SET:
		listener->pageMarginChange(WPX_BOTTOM, m_margin);
		break;
	case WP6_PAGE_GROUP_SUPPRESS_PAGE_CHARACTERISTICS:
		listener->suppressPageCharacteristics(m_suppressedCode);
		break;
	case WP6_PAGE_GROUP_FORM:
		listener->pageFormChange(m_formLength, m_formWidth, m_formOrientation);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
