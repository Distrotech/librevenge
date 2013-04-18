/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP6NumberingMethodGroup.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"


WP6NumberingMethodGroup::WP6NumberingMethodGroup(WPXInputStream *input,
        WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_levelNumber(0x00),
	m_numberingMethod(0x00)
{
	_read(input, encryption);
}

WP6NumberingMethodGroup::~WP6NumberingMethodGroup()
{
}

void WP6NumberingMethodGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch (getSubGroup())
	{
	case WP6_NUMBERING_METHOD_GROUP_SET_PAGE_NUMBERING_MODE:
		m_levelNumber = readU8(input, encryption);;
		m_numberingMethod = readU8(input, encryption);
		break;
	default:
		break;
	}
}

void WP6NumberingMethodGroup::parse(WP6Listener *listener)
{
	switch (getSubGroup())
	{
	case WP6_NUMBERING_METHOD_GROUP_SET_PAGE_NUMBERING_MODE:
		switch (m_numberingMethod)
		{
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_LOWERCASE:
			listener->setPageNumberingType(LOWERCASE);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_UPPERCASE:
			listener->setPageNumberingType(UPPERCASE);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_LOWERCASE_ROMAN:
			listener->setPageNumberingType(LOWERCASE_ROMAN);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_UPPERCASE_ROMAN:
			listener->setPageNumberingType(UPPERCASE_ROMAN);
			break;
		case WP6_NUMBERING_METHOD_GROUP_PAGE_NUMBERING_ARABIC:
		default:
			listener->setPageNumberingType(ARABIC);
			break;
		}
		break;
	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
