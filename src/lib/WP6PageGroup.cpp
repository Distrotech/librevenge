/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
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

#include "WP6PageGroup.h"
#include "WP6LLListener.h"
#include "libwpd_internal.h"

WP6PageGroup::WP6PageGroup(WPXInputStream *input) :
	WP6VariableLengthGroup(),
	m_margin(0),
	m_formName(NULL)
{
	_read(input);
}

WP6PageGroup::~WP6PageGroup()
{
	delete [] m_formName;
}

void WP6PageGroup::_readContents(WPXInputStream *input)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP6_PAGE_GROUP_TOP_MARGIN_SET:
	case WP6_PAGE_GROUP_BOTTOM_MARGIN_SET:
		m_margin = gsf_le_read_guint16(input);
		WPD_DEBUG_MSG(("WordPerfect: Read page group margin size (margin: %i)\n", m_margin));
		break;
	case WP6_PAGE_GROUP_SUPPRESS_PAGE_CHARACTERISTICS:
		m_suppressedCode = gsf_le_read_guint8(input);
		WPD_DEBUG_MSG(("WordPerfect: Read suppressed code (%i)\n", m_suppressedCode));
		break;
	case WP6_PAGE_GROUP_FORM:
		guint8 tmpOrientation;
		m_formHashTableIndex = gsf_le_read_guint8(input);
		m_formHashValue = gsf_le_read_guint16(input);
		m_formLength = gsf_le_read_guint16(input);
		m_formWidth = gsf_le_read_guint16(input);
		m_formType = gsf_le_read_guint8(input);
		tmpOrientation = gsf_le_read_guint8(input);
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
		m_formNameLength = gsf_le_read_guint8(input);

		if (m_formNameLength == 0)
		{
			m_formName = new gchar[1];
			m_formName[0]='\0';
		}
		else
		{
			m_formName = new gchar[m_formNameLength];
			for (guint8 i=0; i<m_formNameLength; i++)
			{
				guint8 characterSet;
				guint8 character;
				int len;
				const guint16 *chars;
				guint16 charWord = gsf_le_read_guint16(input);
				characterSet = (charWord & 0xFF00) >> 8;
				character = (charWord & 0xFF);
				len = extendedCharacterToUCS2(character, characterSet, &chars);
				m_formName[i] = chars[0];
			}
		}
		WPD_DEBUG_MSG(("WordPerfect: Read form information (length: %i), (width: %i), (orientation: %i), (form name: %s),\n",m_formLength, m_formWidth, m_formName ));
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP6PageGroup::parse(WP6HLListener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling an Page group\n"));
	
	switch (getSubGroup())
	{
	case WP6_PAGE_GROUP_TOP_MARGIN_SET:
	case WP6_PAGE_GROUP_BOTTOM_MARGIN_SET:
		listener->pageMarginChange(getSubGroup(), m_margin);
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
