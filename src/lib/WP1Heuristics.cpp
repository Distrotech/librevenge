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

#include "WP1Heuristics.h"
#include "WP1FileStructure.h"
#include "libwpd_internal.h"

WPDConfidence WP1Heuristics::isWP1FileFormat(WPXInputStream *input, bool partialContent)
{
	int functionGroupCount = 0;
	
	WPD_DEBUG_MSG(("WP1Heuristics::isWP1FileFormat()\n"));
	input->seek(0, WPX_SEEK_SET);
	WPD_DEBUG_MSG(("WP1Heuristics::isWP1FileFormat()\n"));
	
	while (!input->atEOS())
	{
		uint8_t readVal = readU8(input);

		WPD_DEBUG_MSG(("WP1Heuristics, Offset 0x%.8x, value 0x%.2x\n", input->tell() - 1, readVal));
		
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
			return WPD_CONFIDENCE_NONE;
		}
		else 
		{
			// multi character function group
			// check that the size constrains are valid, and that every group_member
			// is properly closed at the right place
		
			if (WP1_FUNCTION_GROUP_SIZE[readVal-0xC0] == -1)
			{
				// variable length function group

				// We are checking following structure:
				//   <function code>{function length}...{function length}<function code>
				//   that we observed in variable length WP1 functions 
				
				long functionLength = readU32(input, true);
				long closingFunctionLength = 0;
				WPD_DEBUG_MSG(("WP1Heuristics functionLength = 0x%.8x\n", functionLength));

				input->seek(functionLength, WPX_SEEK_CUR);
				closingFunctionLength = readU32(input, true);
				WPD_DEBUG_MSG(("WP1Heuristics closingFunctionLength = 0x%.8x\n", closingFunctionLength));
				if (functionLength != closingFunctionLength)
					return WPD_CONFIDENCE_NONE;
					
				uint8_t closingGate = 0;
				if (!input->atEOS())
				{
					closingGate = readU8(input);
					WPD_DEBUG_MSG(("WP1Heuristics closingGate = 0x%.2x\n", closingGate));
					if (closingGate != readVal)
						return WPD_CONFIDENCE_NONE;
				}

				// when passed the complete file, we don't allow for open groups when we've reached EOF
				if (!partialContent && input->atEOS() && (closingGate != readVal))
					return WPD_CONFIDENCE_NONE;
				
				functionGroupCount++;
			}
			else
			{
				// fixed length function group
				
				// seek to the position where the closing gate should be
				int res = input->seek(WP1_FUNCTION_GROUP_SIZE[readVal-0xC0]-2, WPX_SEEK_CUR);
				// when passed the complete file, we should be able to do that
				if (!partialContent && res)
					return WPD_CONFIDENCE_NONE;
				
				// read the closing gate
				uint8_t readNextVal = readU8(input);
				if (readNextVal != readVal)
					return WPD_CONFIDENCE_NONE;
				
				functionGroupCount++;
			}
		}
	}	

	/* When we get here, the document is in a format that we *could* import properly.
	However, if we didn't entcounter a single WP4.2 function group) we need to be more carefull:
	this would be the case when passed a plaintext file for example, which libwpd is not
	supposed to handle. */
	if (!functionGroupCount)
		return WPD_CONFIDENCE_POOR;
	
	return WPD_CONFIDENCE_EXCELLENT;
}
