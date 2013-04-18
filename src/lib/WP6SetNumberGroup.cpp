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

#include "WP6SetNumberGroup.h"
#include "WP6FileStructure.h"
#include "WP6Listener.h"
#include "libwpd_internal.h"


WP6SetNumberGroup::WP6SetNumberGroup(WPXInputStream *input,
                                     WPXEncryption *encryption) :
	WP6VariableLengthGroup(),
	m_countOfLevelNumbersSetting(0x00),
	m_startingLevelNumber(0x00),
	m_countNumbers(0)
{
	_read(input, encryption);
}

WP6SetNumberGroup::~WP6SetNumberGroup()
{
}

void WP6SetNumberGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch (getSubGroup())
	{
	case WP6_SET_NUMBER_GROUP_SET_PAGE_NUMBER:
		m_countOfLevelNumbersSetting = readU8(input, encryption);
		m_startingLevelNumber = readU8(input, encryption);
		m_countNumbers = readU16(input, encryption);
		WPD_DEBUG_MSG(("WordPerfect: Set number page number (%d, %d, %d)\n", m_countOfLevelNumbersSetting, m_startingLevelNumber, m_countNumbers));
		break;
	default:
		break;
	}
}

void WP6SetNumberGroup::parse(WP6Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a set number group\n"));

	switch (getSubGroup())
	{
	case WP6_SET_NUMBER_GROUP_SET_PAGE_NUMBER:
		listener->setPageNumber(m_countNumbers);
		break;
	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
