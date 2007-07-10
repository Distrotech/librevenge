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

const char *FONT_WEIGHT_STRINGS[] = {	"Bold", "bold", "Demi", "demi", "Extended", "extended",
					"Extra", "extra", "Headline", "headline", "Light", "light",
					"Medium", "medium", "Normal", "normal", "Regular", "regular",
					"Standaard", "standaard", "Standard", "standard" };
const char * USELESS_WP_POSTFIX = "-WP";
#define countElements(a) ((sizeof(a) / sizeof(a[0])))

WP6FontDescriptorPacket::WP6FontDescriptorPacket(WPXInputStream *input, int /* id */, uint32_t dataOffset, uint32_t dataSize) : 
	WP6PrefixDataPacket(input),
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
	m_fontName(0)
{
	_read(input, dataOffset, dataSize);
}

WP6FontDescriptorPacket::~WP6FontDescriptorPacket()
{
	delete [] m_fontName;
}

void WP6FontDescriptorPacket::_readContents(WPXInputStream *input)
{
   // short sized characteristics
   m_characterWidth = readU16(input);
   m_ascenderHeight = readU16(input);
   m_xHeight = readU16(input);
   m_descenderHeight = readU16(input);
   m_italicsAdjust = readU16(input);
   // byte sized characteristics
   m_primaryFamilyMemberId = readU8(input);
   m_primaryFamilyId = readU8(input);
   m_scriptingSystem = readU8(input);
   m_primaryCharacterSet = readU8(input);
   m_width = readU8(input);
   m_weight = readU8(input);
   m_attributes = readU8(input);
   m_generalCharacteristics = readU8(input);
   m_classification = readU8(input);
   m_fill = readU8(input);
   m_fontType = readU8(input);
   m_fontSourceFileType = readU8(input);

   m_fontNameLength = readU16(input); 


   if (m_fontNameLength > ((std::numeric_limits<uint16_t>::max)() / 2))
	m_fontNameLength = ((std::numeric_limits<uint16_t>::max)() / 2);
   if (m_fontNameLength == 0) 
	   {
		   m_fontName = new char[1];
		   m_fontName[0]='\0';
	   }
   
   else 
	   {
		   m_fontName = new char[m_fontNameLength];

		   uint16_t tempLength=0;
		   int numTokens=0;
		   int lastTokenPosition=0;
		   for (uint16_t i=0; i<(m_fontNameLength/2); i++) 
		   {
			   uint16_t charWord = readU16(input);

			   uint8_t characterSet = (uint8_t)((charWord >> 8) & 0x00FF);
			   uint8_t character = (uint8_t)(charWord & 0xFF);
			   if (character == 0x00 && characterSet == 0x00)
			   	break;
			   
			   const uint16_t *chars;
			   extendedCharacterWP6ToUCS2(character, characterSet, &chars);
			   /* We are only using ascii characters here, and
			    * if we have more than one character, that's
			    * going to be an international character, so
			    * we don't actually iterate through the list of
			    * characters returned - we assume that just one
			    * character was returned.
			    */
			   if (chars[0] == 0x20) {
				   m_fontName[tempLength]=' ';
				   tempLength++;
				   numTokens++;
				   lastTokenPosition=tempLength;
			   }
			   else if (chars[0] != 0x00 && chars[0] < 0x7F) {
				   m_fontName[tempLength]=(char) chars[0];
				   tempLength++;
			   }
		   }
		   m_fontName[tempLength]='\0';
		   // TODO/HACK: probably should create a proper static function for doing this
		   // consume the last token (by replacing the first char with a null-terminator) if its a weight signifier
		   // also remove annoying -WP postfix
		   // (NB: not all wp fonts are terminated by weight, just enough of them to make this annoying)
		   // NB: also this is O(n^2). Could be a performance hotspot.
		   WPD_DEBUG_MSG(("WordPerfect: stripping font name (original: %s)\n", m_fontName));
		   for (int stringPosition=(tempLength-1); stringPosition>=0; stringPosition--)
		   {
			   unsigned int k;			   
			   for (k=0; k<countElements(FONT_WEIGHT_STRINGS); k++) 
			   {
				   if (stringPosition > 0 && !strcmp(FONT_WEIGHT_STRINGS[k], &m_fontName[stringPosition])) 
				   {
					   m_fontName[stringPosition-1]='\0';
					   tempLength = (uint16_t)(stringPosition-1);
					   break;
				   }
			   }
			   // SPECIAL CASE: eliminate the -WP postfix (if it's there), which isn't spaced out from
			   // the rest of the font
			   if (k==countElements(FONT_WEIGHT_STRINGS))
			   {
				   if (!strcmp(USELESS_WP_POSTFIX, &m_fontName[stringPosition])) 
				   {
					   m_fontName[stringPosition]='\0';
					   tempLength = (uint16_t)(stringPosition - 1);
				   }
			   }
			   // also consume any whitespace at the end of the font..
			   while ((tempLength - 1) > 0 && m_fontName[tempLength-1] == ' ')
			   {
				   m_fontName[tempLength-1] = '\0';
			   }
		   }
		   WPD_DEBUG_MSG(("WordPerfect: stripping font name (final: %s)\n", m_fontName));
	   }
   WPD_DEBUG_MSG(("WordPerfect: Read Font (primary family id: %i, family member id: %i, font type: %i, font source file type: %i font name length: %i, font name: %s)\n", (int) m_primaryFamilyId, (int) m_primaryFamilyMemberId, (int) m_fontType, (int) m_fontSourceFileType, (int) m_fontNameLength, m_fontName));

}
