/* libwpd2
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */
#include <string.h>

#include "WP6FontDescriptorPacket.h"
#include "libwpd_internal.h"

const char *FONT_WEIGHT_STRINGS[] = { "Black", "Regular", "Normal", "Extended", "Bold", "Light", "Medium", "Extra", "Standaard", "Standard" };
const char * USELESS_WP_POSTFIX = "-WP";
#define countElements(a) ((sizeof(a) / sizeof(a[0])))

WP6FontDescriptorPacket::WP6FontDescriptorPacket(GsfInput *input, int id, guint32 dataOffset, guint32 dataSize) 
	: WP6PrefixDataPacket(input)
{
	_read(input, dataOffset, dataSize);
}

WP6FontDescriptorPacket::~WP6FontDescriptorPacket()
{
	delete [] m_fontName;
}

void WP6FontDescriptorPacket::_readContents(GsfInput *input)
{
   // short sized characteristics
   m_characterWidth = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
   m_ascenderHeight = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
   m_xHeight = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
   m_descenderHeight = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
   m_italicsAdjust = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL);
   // byte sized characteristics
   m_primaryFamilyMemberId = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_primaryFamilyId = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_scriptingSystem = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_primaryCharacterSet = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_width = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_weight = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_attributes = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_generalCharacteristics = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_classification = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_fill = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_fontType = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);
   m_fontSourceFileType = *(const guint8 *)gsf_input_read(input, sizeof(guint8), NULL);

   m_fontNameLength = *(const guint16 *)gsf_input_read(input, sizeof(guint16), NULL); 

   // TODO: re-do sanity checking
   //if(m_fontNameLength < WP_FONT_NAME_MAX_LENGTH)
   //{	
   if (m_fontNameLength == 0) 
	   {
		   m_fontName = new gchar[1];
		   m_fontName[0]='\0';
	   }
   
   else 
	   {
		   guint16 const *tempFontName = (guint16 const *)gsf_input_read(input, sizeof(guint16)*(m_fontNameLength/2), NULL);
		   m_fontName = new gchar[m_fontNameLength];

		   guint16 tempLength=0;
		   int numTokens=0;
		   int lastTokenPosition=0;
		   for (guint16 i=0; i<(m_fontNameLength/2); i++) 
		   {
			   guint8 characterSet = (tempFontName[i] & 0xFF00) >> 8;
			   guint8 character = (tempFontName[i] & 0xFF);
			   guint16 ucs2Character = extendedCharacterToUCS2(character, characterSet);
			   
			   if (ucs2Character == 0x20) {
				   m_fontName[tempLength]=' ';
				   tempLength++;
				   numTokens++;
				   lastTokenPosition=tempLength;
			   }
			   else if (ucs2Character != 0x00 && ucs2Character < 0x7F) {
				   m_fontName[tempLength]=(gchar) ucs2Character;
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
			   int k;
			   for (k=0; k<countElements(FONT_WEIGHT_STRINGS); k++) 
			   {
				   if (!strcmp(FONT_WEIGHT_STRINGS[k], &m_fontName[stringPosition])) 
				   {
					   m_fontName[stringPosition-1]='\0';
					   tempLength = stringPosition-1;
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
					   tempLength = stringPosition - 1;
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

