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

#include <math.h>
#include "WP3MiscellaneousGroup.h"
#include "WP3FileStructure.h"
#include "WPXFileStructure.h"
#include "WP3Listener.h"

WP3MiscellaneousGroup::WP3MiscellaneousGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_pageWidth(0),
	m_pageHeight(0),
	m_pageOrientation(PORTRAIT)
{
	_read(input, encryption);
}

WP3MiscellaneousGroup::~WP3MiscellaneousGroup()
{
}

void WP3MiscellaneousGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP3_MISCELLANEOUS_GROUP_PAGE_SIZE_OVERRIDE:
		uint16_t tmpPageOrientation;

		// skip 20 bytes of old values
		input->seek(20, WPX_SEEK_CUR);

		// read the new values
		tmpPageOrientation = readU16(input, encryption, true);
		m_pageWidth = fixedPointToWPUs(readU32(input, encryption, true));
		m_pageHeight = fixedPointToWPUs(readU32(input, encryption, true));

		// determine whether it is portrait or landscape
		if ((tmpPageOrientation & 0x0001) == 0x0000)
			m_pageOrientation = PORTRAIT;
		else
			m_pageOrientation = LANDSCAPE;

		break;

	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3MiscellaneousGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Miscellaneous group\n"));

	switch (getSubGroup())
	{
	case WP3_MISCELLANEOUS_GROUP_PAGE_SIZE_OVERRIDE:
		listener->pageFormChange(m_pageHeight, m_pageWidth, m_pageOrientation);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
