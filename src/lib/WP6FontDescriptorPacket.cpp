/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
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
#include "WP6FontDescriptorPacket.h"
#include "libwpd_internal.h"

static char *fontWeightStrings[] = { "Regular", "Bold" };
static int numFontWeightStrings = 2;

WP6FontDescriptorPacket::WP6FontDescriptorPacket(FILE *stream, int id, guint32 dataOffset, guint32 dataSize) 
	: WP6PrefixDataPacket(stream, id)
{
	_read(stream, dataOffset, dataSize);
}

WP6FontDescriptorPacket::~WP6FontDescriptorPacket()
{
	delete(m_fontName);
}

void WP6FontDescriptorPacket::_readContents(FILE *stream)
{
   // short sized characteristics
   WPD_CHECK_FILE_READ_ERROR(fread(&m_characterWidth, sizeof(guint16), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_ascenderHeight, sizeof(guint16), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_xHeight, sizeof(guint16), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_descenderHeight, sizeof(guint16), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_italicsAdjust, sizeof(guint16), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_primaryFamilyMemberId, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_primaryFamilyId, sizeof(guint8), 1, stream), 1);
   // byte sized characteristics
   WPD_CHECK_FILE_READ_ERROR(fread(&m_scriptingSystem, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_primaryCharacterSet, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_width, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_weight, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_attributes, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_generalCharacteristics, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_classification, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_fill, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_fontType, sizeof(guint8), 1, stream), 1);
   WPD_CHECK_FILE_READ_ERROR(fread(&m_fontSourceFileType, sizeof(guint8), 1, stream), 1);

   WPD_CHECK_FILE_READ_ERROR(fread(&m_fontNameLength, sizeof(guint16), 1, stream), 1); // way more space than we actually need, oh well..

   // TODO: re-do sanity checking
   //if(m_fontNameLength < WP_FONT_NAME_MAX_LENGTH)
   //{	
   gchar *tempFontName = new char[m_fontNameLength];
   WPD_CHECK_FILE_READ_ERROR(fread(tempFontName, sizeof(char), m_fontNameLength, stream), m_fontNameLength);
   m_fontName = new char[m_fontNameLength];
   guint16 tempLength1=0;
   int numTokens=0;
   int lastTokenPosition=0;
   for (guint16 i=0; i<m_fontNameLength; i++) 
	   {
		   if (tempFontName[i] == 0x20) {
			   m_fontName[tempLength1]=' ';
			   tempLength1++;
			   numTokens++;
			   lastTokenPosition=tempLength1;
		   }
		   else if (tempFontName[i] != 0x00) {
			   m_fontName[tempLength1]=tempFontName[i];
			   tempLength1++;
		   }
	   }
   m_fontName[tempLength1]='\0';
   // TODO/HACK: probably should create a proper static function for doing this
   // consume the last token (by replacing the first char with a null-terminator) if its a weight signifier
   // (NB: not all wp fonts are terminated by weight, just enough of them to make this annoying
   for (int j=0; j<numFontWeightStrings; j++) 
	   {
		   if (!strcmp(fontWeightStrings[j], &m_fontName[lastTokenPosition])) 
			   {
				   if (lastTokenPosition > 0)
					   m_fontName[lastTokenPosition-1]='\0';
				   break;
			   }
	   }
   delete (tempFontName);

   //

   //}
   //    else
   //      return gIE_IMPORTERROR;
   
   WPD_DEBUG_MSG(("WordPerfect: Read Font (primary family id: %i, family member id: %i, font type: %i, font source file type: %i font name length: %i, font name: %s)\n", (int) m_primaryFamilyId, (int) m_primaryFamilyMemberId, (int) m_fontType, (int) m_fontSourceFileType, (int) m_fontNameLength, m_fontName));

}

