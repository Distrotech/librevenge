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

#ifndef WP6FONTDESCRIPTORPACKET_H
#define WP6FONTDESCRIPTORPACKET_H

#include <libwpd/libwpd.h>
#include "WP6PrefixDataPacket.h"

class WP6FontDescriptorPacket : public WP6PrefixDataPacket
{
public:
	WP6FontDescriptorPacket(WPXInputStream *input, WPXEncryption *encryption, int id, uint32_t dataOffset, uint32_t dataSize);
	~WP6FontDescriptorPacket();
	void _readContents(WPXInputStream *input, WPXEncryption *encryption);
	const WPXString &getFontName() const
	{
		return m_fontName;
	}

private:
	void _readFontName(WPXInputStream *input, WPXEncryption *encryption);

	WP6FontDescriptorPacket(const WP6FontDescriptorPacket &);
	WP6FontDescriptorPacket &operator=(const WP6FontDescriptorPacket &);
	uint16_t m_characterWidth;
	uint16_t m_ascenderHeight;
	uint16_t m_xHeight;
	uint16_t m_descenderHeight;
	uint16_t m_italicsAdjust;
	uint8_t m_primaryFamilyId; // family id's are supposed to be one unified element, but I split them up to ease parsing
	uint8_t m_primaryFamilyMemberId;

	uint8_t m_scriptingSystem;
	uint8_t m_primaryCharacterSet;
	uint8_t m_width;
	uint8_t m_weight;
	uint8_t m_attributes;
	uint8_t m_generalCharacteristics;
	uint8_t m_classification;
	uint8_t m_fill; // fill byte
	uint8_t m_fontType;
	uint8_t m_fontSourceFileType;

	uint16_t m_fontNameLength;

	WPXString m_fontName;
};
#endif
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
