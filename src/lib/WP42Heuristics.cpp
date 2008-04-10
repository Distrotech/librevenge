/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003 Marc Maurer (uwog@uwog.net)
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "WP42Heuristics.h"
#include "WP42FileStructure.h"
#include "libwpd_internal.h"

WPDPasswordMatch WP42Heuristics::verifyPassword(WPXInputStream *input, const char *password)
{
	if (!password)
		return WPD_PASSWORD_MATCH_DONTKNOW;

	input->seek(0, WPX_SEEK_SET);
	WPXEncryption *encryption = 0;
	try
	{
		if (readU8(input, 0) == 0xFE && readU8(input, 0) == 0xFF &&
			readU8(input, 0) == 0x61 && readU8(input, 0) == 0x61)
		{
			encryption = new WPXEncryption(password, 6);
			if (readU16(input, 0) == encryption->getCheckSum())
			{
				delete encryption;
				return WPD_PASSWORD_MATCH_OK;
			}
			else
			{
				delete encryption;
				return WPD_PASSWORD_MATCH_NONE;
			}
		}
		else
			return WPD_PASSWORD_MATCH_DONTKNOW;
	}
	catch (...)
	{
		if (encryption)
			delete encryption;
		return WPD_PASSWORD_MATCH_DONTKNOW;
	}
}

WPDConfidence WP42Heuristics::isWP42FileFormat(WPXInputStream *input, const char *password)
{
	input->seek(0, WPX_SEEK_SET);
	WPXEncryption *encryption = 0;
	try
	{
		if (readU8(input, 0) == 0xFE && readU8(input, 0) == 0xFF &&
			readU8(input, 0) == 0x61 && readU8(input, 0) == 0x61)
		{
			if (password)
			{
				encryption = new WPXEncryption(password, 6);
				if (readU16(input, 0) != encryption->getCheckSum())
				{
					delete encryption;
					return WPD_CONFIDENCE_SUPPORTED_ENCRYPTION;
				}
			}
			else
			{
				if (readU16(input,0) != 0x0000)
					return WPD_CONFIDENCE_SUPPORTED_ENCRYPTION;
			}
		}
				
		input->seek(0, WPX_SEEK_SET);
		if (password && encryption)
			input->seek(6, WPX_SEEK_SET);

		int functionGroupCount = 0;
	
		WPD_DEBUG_MSG(("WP42Heuristics::isWP42FileFormat()\n"));
	
		while (!input->atEOS())
		{
			uint8_t readVal = readU8(input, encryption);

			WPD_DEBUG_MSG(("WP42Heuristics, Offset 0x%.8x, value 0x%.2x\n", (unsigned int)(input->tell() - 1), readVal));
		
			if (readVal < (uint8_t)0x20)
			{
				// line breaks et al, skip
			}
			else if (readVal >= (uint8_t)0x20 && readVal <= (uint8_t)0x7F)
			{
				// normal ASCII characters, skip			
			}
			else if (readVal >= (uint8_t)0x80 && readVal <= (uint8_t)0xBF)
			{
				// single character function codes, skip
				functionGroupCount++;
			}
			else if (readVal >= (uint8_t)0xFF)
			{
				// special codes that should not be found as separate functions
				if (encryption)
					delete encryption;
				return WPD_CONFIDENCE_NONE;
			}
			else 
			{
				// multi character function group
				// check that the size constrains are valid, and that every group_member
				// is properly closed at the right place
		
				if (WP42_FUNCTION_GROUP_SIZE[readVal-0xC0] == -1)
				{
					// variable length function group
				
					// skip over all the bytes in the group, and scan for the closing gate
					uint8_t readNextVal = 0;
					while (!input->atEOS())
					{
						readNextVal = readU8(input, encryption);
						if (readNextVal == readVal)
							break;
					}

					// when passed the complete file, we don't allow for open groups when we've reached EOF
					if ((readNextVal == 0) || (input->atEOS() && (readNextVal != readVal)))
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}
				
					functionGroupCount++;
				}
				else
				{
					// fixed length function group
				
					// seek to the position where the closing gate should be
					int res = input->seek(WP42_FUNCTION_GROUP_SIZE[readVal-0xC0]-2, WPX_SEEK_CUR);
					// when passed the complete file, we should be able to do that
					if (res)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}
				
					// read the closing gate
					uint8_t readNextVal = readU8(input, encryption);
					if (readNextVal != readVal)
					{
						if (encryption)
							delete encryption;
						return WPD_CONFIDENCE_NONE;
					}

					functionGroupCount++;
				}
			}
		}	

		/* When we get here, the document is in a format that we *could* import properly.
		However, if we didn't entcounter a single WP4.2 function group) we need to be more carefull:
		this would be the case when passed a plaintext file for example, which libwpd is not
		supposed to handle. */
		if (!functionGroupCount)
		{
			if (encryption)
			{
				delete encryption;
				return WPD_CONFIDENCE_EXCELLENT;
			}
			return WPD_CONFIDENCE_NONE;
		}
		if (encryption)
			delete encryption;
		return WPD_CONFIDENCE_EXCELLENT;
	}
	catch (...)
	{
		if (encryption)
			delete encryption;
		return WPD_CONFIDENCE_NONE;
	}
}
