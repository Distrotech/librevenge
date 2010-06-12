/* libwpd
 * Copyright (C) 2002 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2002 Marc Maurer (uwog@uwog.net)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
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
#include <string.h>
#include <limits>
#include "WP6FontDescriptorPacket.h"
#include "libwpd_internal.h"
#include <string>

const char * FONT_WEIGHT_STRINGS [] = {	"Bold", "bold", "Demi", "demi", "Extended", "extended",
					"Extra", "extra", "Headline", "headline", "Light", "light",
					"Medium", "medium", "Normal", "normal", "Regular", "regular",
					"Standaard", "standaard", "Standard", "standard" };
const char * USELESS_WP_POSTFIX = "-WP";
#define countElements(a) ((sizeof(a) / sizeof(a[0])))

WP6FontDescriptorPacket::WP6FontDescriptorPacket(WPXInputStream *input, WPXEncryption *encryption, int /* id */, uint32_t dataOffset, uint32_t dataSize) : 
	WP6PrefixDataPacket(input, encryption),
	m_characterWidth(0),
	m_ascenderHeight(0),
	m_xHeight(0),
	m_descenderHeight(0),
	m_italicsAdjust(0),
	m_primaryFamilyId(0),
	m_primaryFamilyMemberId(0),
	m_scriptingSystem(0),
	m_primaryCharacterSet(0),
	m_width(0),
	m_weight(0),
	m_attributes(0),
	m_generalCharacteristics(0),
	m_classification(0),
	m_fill(0),
	m_fontType(0),
	m_fontSourceFileType(0),
	m_fontNameLength(0),
	m_fontName()
{
	_read(input, encryption, dataOffset, dataSize);
}

WP6FontDescriptorPacket::~WP6FontDescriptorPacket()
{
}

void WP6FontDescriptorPacket::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	// short sized characteristics
	m_characterWidth = readU16(input, encryption);
	m_ascenderHeight = readU16(input, encryption);
	m_xHeight = readU16(input, encryption);
	m_descenderHeight = readU16(input, encryption);
	m_italicsAdjust = readU16(input, encryption);
	// byte sized characteristics
	m_primaryFamilyMemberId = readU8(input, encryption);
	m_primaryFamilyId = readU8(input, encryption);
	m_scriptingSystem = readU8(input, encryption);
	m_primaryCharacterSet = readU8(input, encryption);
	m_width = readU8(input, encryption);
	m_weight = readU8(input, encryption);
	m_attributes = readU8(input, encryption);
	m_generalCharacteristics = readU8(input, encryption);
	m_classification = readU8(input, encryption);
	m_fill = readU8(input, encryption);
	m_fontType = readU8(input, encryption);
	m_fontSourceFileType = readU8(input, encryption);

	m_fontNameLength = readU16(input, encryption); 

	_readFontName(input, encryption);
}

void WP6FontDescriptorPacket::_readFontName(WPXInputStream *input, WPXEncryption *encryption)
{
	if (m_fontNameLength > ((std::numeric_limits<uint16_t>::max)() / 2))
	m_fontNameLength = ((std::numeric_limits<uint16_t>::max)() / 2);
	if (m_fontNameLength) 
	{
		for (uint16_t i=0; i<(m_fontNameLength/2); i++) 
		{
			uint16_t charWord = readU16(input, encryption);
			uint8_t characterSet = (uint8_t)((charWord >> 8) & 0x00FF);
			uint8_t character = (uint8_t)(charWord & 0xFF);

			if (character == 0x00 && characterSet == 0x00)
				break;

			const uint16_t *chars;
			int len = extendedCharacterWP6ToUCS2(character, characterSet, &chars);

			for (int j = 0; j < len; j++)
				appendUCS4(m_fontName, (uint32_t)chars[j]);
		}

		WPD_DEBUG_MSG(("WordPerfect: stripping font name (original: %s)\n", m_fontName.cstr()));
		std::string stringValue(m_fontName.cstr());
		std::string::size_type pos;
		for (unsigned k = 0; k < countElements(FONT_WEIGHT_STRINGS); k++)
		{
			if (!stringValue.empty())
				while ((pos = stringValue.find(FONT_WEIGHT_STRINGS[k])) != std::string::npos)
	  				stringValue.replace(pos, strlen(FONT_WEIGHT_STRINGS[k]),"");
		}
		// SPECIAL CASE: eliminate the -WP postfix (if it's there), which isn't spaced out from
		// the rest of the font
		if (!stringValue.empty())
			while ((pos = stringValue.find(USELESS_WP_POSTFIX)) != std::string::npos)
			stringValue.replace(pos, strlen(USELESS_WP_POSTFIX), "");
			// also consume any whitespace at the end of the font.
		if (!stringValue.empty())
			while ((pos = stringValue.find("  ")) != std::string::npos)
			stringValue.replace(pos, strlen("  "), " ");
		if (!stringValue.empty())
			while ((pos = stringValue.find(" ", stringValue.size() - 1)) != std::string::npos)
			stringValue.replace(pos, strlen(" "), "");
		if (!stringValue.empty())
			while ((pos = stringValue.find("-", stringValue.size() - 1)) != std::string::npos)
			stringValue.replace(pos, strlen("-"), "");
	
		m_fontName = WPXString(stringValue.c_str());
			WPD_DEBUG_MSG(("WordPerfect: stripping font name (final: %s)\n", m_fontName.cstr()));
	}
}
