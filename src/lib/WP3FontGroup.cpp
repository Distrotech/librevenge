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
#include "WP3FontGroup.h"
#include "WP3FileStructure.h"
#include "libwpd_internal.h"
#include "libwpd_math.h"
#include "WP3Listener.h"

WP3FontGroup::WP3FontGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP3VariableLengthGroup(),
	m_fontColor(),
	m_fontName(),
	m_fontSize(0)
{
	_read(input, encryption);
}

WP3FontGroup::~WP3FontGroup()
{
}

void WP3FontGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// this group can contain different kinds of data, thus we need to read
	// the contents accordingly
	switch (getSubGroup())
	{
	case WP3_FONT_GROUP_SET_TEXT_COLOR:
		input->seek(6, WPX_SEEK_CUR);
		{
			uint16_t tmpRed = readU16(input, encryption, true);
			uint16_t tmpGreen = readU16(input, encryption, true);
			uint16_t tmpBlue = readU16(input, encryption, true);
			m_fontColor = RGBSColor(tmpRed, tmpGreen, tmpBlue);
		}
		break;
	case WP3_FONT_GROUP_SET_TEXT_FONT:
		input->seek(12, WPX_SEEK_CUR);
		m_fontName = readPascalString(input, encryption);
		break;
	case WP3_FONT_GROUP_SET_FONT_SIZE:
		input->seek(2, WPX_SEEK_CUR);
		m_fontSize = readU16(input, encryption, true);
		break;
	default: /* something else we don't support, since it isn't in the docs */
		break;
	}
}

void WP3FontGroup::parse(WP3Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Font group\n"));

	switch (getSubGroup())
	{
	case WP3_FONT_GROUP_SET_TEXT_COLOR:
		listener->setTextColor(&m_fontColor);
		break;
	case WP3_FONT_GROUP_SET_TEXT_FONT:
		listener->setTextFont(m_fontName);
		break;
	case WP3_FONT_GROUP_SET_FONT_SIZE:
		listener->setFontSize(m_fontSize);
		break;
	default: // something else we don't support, since it isn't in the docs
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
