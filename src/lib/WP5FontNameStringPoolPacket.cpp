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
#include <string.h>

#include "WP5FontNameStringPoolPacket.h"
#include "WP5Parser.h"
#include "libwpd_internal.h"

WP5FontNameStringPoolPacket::WP5FontNameStringPoolPacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, uint32_t dataOffset, uint32_t dataSize) :
	WP5GeneralPacketData(),
	m_fontNameString()
{
	_read(input, encryption, dataOffset, dataSize);
}

WP5FontNameStringPoolPacket::~WP5FontNameStringPoolPacket()
{
}

void WP5FontNameStringPoolPacket::_readContents(WPXInputStream *input, WPXEncryption *encryption, uint32_t dataSize)
{
	long tmpInitialOffset = input->tell();
	while (input->tell() < (long)(tmpInitialOffset + dataSize))
	{
		unsigned offset = (unsigned)(input->tell() - tmpInitialOffset);
		WPXString fontName = readCString(input, encryption);
		m_fontNameString[offset] = fontName;
	}

	for (std::map<unsigned int, WPXString>::const_iterator Iter = m_fontNameString.begin(); Iter != m_fontNameString.end(); ++Iter)
		WPD_DEBUG_MSG(("WP5 Font Name String Pool Packet: offset: %i font name: %s\n", Iter->first, (Iter->second).cstr()));
}

WPXString WP5FontNameStringPoolPacket::getFontName(const unsigned int offset) const
{
	std::map<unsigned int, WPXString>::const_iterator Iter = m_fontNameString.find(offset);
	if (Iter != m_fontNameString.end())
		return Iter->second;
	// if the offset is not correct, return the default value
	return WPXString("Times New Roman");
}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
