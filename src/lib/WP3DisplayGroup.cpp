/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
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
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
