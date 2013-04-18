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

#include "WP5FontGroup.h"
#include "WPXListener.h"
#include "libwpd_internal.h"
#include "WP5FileStructure.h"
#include "WP5PrefixData.h"
#include "WP5ListFontsUsedPacket.h"
#include "WP5FontNameStringPoolPacket.h"
#include "WP5Listener.h"

WP5FontGroup::WP5FontGroup(WPXInputStream *input, WPXEncryption *encryption) :
	WP5VariableLengthGroup(),
	m_red(0),
	m_green(0),
	m_blue(0),
	m_fontNumber(0),
	m_fontSize(-1)
{
	_read(input, encryption);
}

void WP5FontGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	switch(getSubGroup())
	{
	case WP5_TOP_FONT_GROUP_COLOR:
		input->seek(3, WPX_SEEK_CUR);
		m_red = readU8(input, encryption);
		m_green = readU8(input, encryption);
		m_blue = readU8(input, encryption);
		break;
	case WP5_TOP_FONT_GROUP_FONT_CHANGE:
		input->seek(25, WPX_SEEK_CUR);
		m_fontNumber = readU8(input, encryption);
		if (getSize() >= 36)
		{
			input->seek(2, WPX_SEEK_CUR);
			m_fontSize = (double)(readU16(input, encryption) / 50);
		}
		break;
	default:
		break;
	}
}

void WP5FontGroup::parse(WP5Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling a Font group\n"));

	unsigned tmpFontNameOffset = 0;
	double tmpFontSize = 12.0;
	WPXString tmpFontName("Times New Roman");

	switch(getSubGroup())
	{
	case WP5_TOP_FONT_GROUP_COLOR:
		listener->characterColorChange(m_red, m_green, m_blue);
		break;
	case WP5_TOP_FONT_GROUP_FONT_CHANGE:
		if (listener->getGeneralPacketData(15))
		{
			tmpFontSize = static_cast<const WP5ListFontsUsedPacket *>(listener->getGeneralPacketData(15))->getFontSize(m_fontNumber);
			tmpFontNameOffset = static_cast<const WP5ListFontsUsedPacket *>(listener->getGeneralPacketData(15))->getFontNameOffset(m_fontNumber);

		}
		else if (listener->getGeneralPacketData(2))
		{
			tmpFontSize = static_cast<const WP5ListFontsUsedPacket *>(listener->getGeneralPacketData(2))->getFontSize(m_fontNumber);
			tmpFontNameOffset = static_cast<const WP5ListFontsUsedPacket *>(listener->getGeneralPacketData(2))->getFontNameOffset(m_fontNumber);
		}
		else
		{
			listener->setFont(tmpFontName, tmpFontSize);
			return;
		}

		if (listener->getGeneralPacketData(7))
			tmpFontName = static_cast<const WP5FontNameStringPoolPacket *>(listener->getGeneralPacketData(7))->getFontName(tmpFontNameOffset);
		if (m_fontSize >= 0)
			tmpFontSize = m_fontSize;

		WPD_DEBUG_MSG(("WP5 Parsing Font Change, fontNumber %i, fontName: %s, fontSize: %.4f\n", m_fontNumber, tmpFontName.cstr(), tmpFontSize));
		listener->setFont(tmpFontName, tmpFontSize);
		break;
	default:
		break;
	}
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
